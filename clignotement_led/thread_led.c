#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kthread.h>

#define YELLOW_LED 22
#define RED_LED 16

static char *KEYS_NAME = "PB_KEY";
int red_led_state;
int button_pushed = 0;

static int pin_button;  //27 
static DECLARE_WAIT_QUEUE_HEAD(my_wq);

int led_sparkle(void *data) {
    pr_info("my led start to sparkle \n");

    while(!kthread_should_stop()) {
	while(button_pushed == 1 && !kthread_should_stop()) {
        	gpio_set_value(RED_LED, !red_led_state);
        	red_led_state = !red_led_state;
        	msleep(500);
    	}
	wait_event_interruptible(my_wq, button_pushed == 1);
    }   

    return 0;
}



static struct task_struct *t1;
static irqreturn_t hello_keys_isr(int irq, void *data)
{
    struct device *dev = data;
	button_pushed = !button_pushed;
	if (button_pushed == 1) {
		wake_up_interruptible(&my_wq);
	}

    return IRQ_HANDLED;
}

static ssize_t my_read(struct file *filp, char *buf, size_t count, loff_t *pos)
{
    return 0;
}

/*
static ssize_t my_led_write(struct file *filp, const char *buf, size_t len, loff_t *off) {
    char my_buffer[len + 1];
    copy_from_user(my_buffer, buf, len);
    my_buffer[len] = '\0';

    printk("before conditions, input length: %ld \n", strlen(buf));

    if (strcmp(my_buffer, "of") == 0) {
        while(!kthread_should_stop()) {
            gpio_set_value(RED_LED, !red_led_state);
            red_led_state = !red_led_state;
            msleep(500);
        }
        wait_event_interruptible(my_wq, strcmp(my_buffer, "on") == 0);
         }   
     else if (strcmp(my_buffer, "on") == 0) {
            wake_up_interruptible(&my_wq);
    }   
    return len;
}
*/

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_led_write,
};

static struct miscdevice my_miscdevice = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "mydev",
    .fops = &fops,
    .mode = 0666,
};

static int my_probe(struct platform_device *pdev)
{
    int ret_val, irq;
    struct device *dev = &pdev->dev;

    pr_info("my_probe enter\n");
    dev_info(dev, "my_probe() function is called.\n");
    irq = platform_get_irq(pdev, 0);
    if (irq < 0) {
        dev_err(dev, "irq is not available\n");
        return -EINVAL;
    }
    dev_info(dev, "IRQ_using_platform_get_irq: %d\n", irq);
    ret_val = devm_request_irq(dev, irq, hello_keys_isr, IRQF_SHARED, KEYS_NAME, dev);
    if (ret_val != 0) {
        dev_err(dev, "Error when request irq\n");
        return ret_val;
    }

    pr_info("Pin interruption %d",pin_button);


    dev_info(dev, "my_probe() function is exited.\n");

    return 0;
}

static int  my_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Remove called\n");
    return 0;
}

static const struct of_device_id my_of_ids[] = {
    { .compatible = "training,intbutton"},
    {},
};

MODULE_DEVICE_TABLE(of, my_of_ids);
static struct platform_driver my_platform_driver = {
    .probe = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "int_button",
        .of_match_table = my_of_ids,
        .owner = THIS_MODULE,
    }
};

static int __init my_init(void)
{
    int ret_val;

    pr_info("Push button driver enter\n");
    ret_val = platform_driver_register(&my_platform_driver);
    if (ret_val != 0) {
        pr_err("platform value returned %d\n", ret_val);
        return ret_val;
    }
    ret_val = misc_register(&my_miscdevice);
    if (ret_val != 0) {
        pr_err("misc register value returned %d\n", ret_val);
        return ret_val;
    }

    gpio_request(RED_LED, "red");
    gpio_direction_output(RED_LED, 1);
    red_led_state = 1;
    button_pushed = 1;
    t1 = kthread_run(led_sparkle, NULL, "red led thread");

    gpio_direction_input(pin_button);
    pr_info("Pin interruption %d",pin_button);

    pr_info("");

    return 0;
}

static void __exit my_exit(void)
{
    pr_info("Push button driver exit\n");
    kthread_stop(t1);
    gpio_set_value(RED_LED, 0);
    gpio_set_value(YELLOW_LED, 0);
    gpio_free(RED_LED);
    gpio_free(YELLOW_LED);
    misc_deregister(&my_miscdevice);
    platform_driver_unregister(&my_platform_driver);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("C.A. ABID");
MODULE_DESCRIPTION("Using interrupts with a push button");
