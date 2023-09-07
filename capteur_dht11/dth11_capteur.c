#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define TEMPERATURE_HISTORY_SIZE 100


static int dht11_pin;
u8 temperature;
u8 humidity;

static struct task_struct *temperature_thread;


void SendStartSignal(void){
  gpio_direction_output(dht11_pin, 0);
  msleep(20);
  gpio_set_value(dht11_pin,1);
  gpio_direction_input(dht11_pin);/* en ecoute */

}

int WaitForLow(void){
  ktime_t start_time, end_time, duration;
  s64 microseconds;
  start_time = ktime_get();
  do{
    end_time = ktime_get();
    duration = ktime_sub(end_time, start_time);
    microseconds = ktime_to_us(duration);
  }while(gpio_get_value(dht11_pin)==1 && microseconds < 1000); 
  
  if(microseconds >= 1000)
    pr_info("timeout WaitForLow\n");
  
  return microseconds;
}

int WaitForHigh(void){
  ktime_t start_time, end_time, duration;
  s64 microseconds;
  start_time = ktime_get();
  do{
    end_time = ktime_get();
    duration = ktime_sub(end_time, start_time);
    microseconds = ktime_to_us(duration);
  }while(gpio_get_value(dht11_pin)==0 && microseconds < 1000); 
  
  if(microseconds >= 1000)
    pr_info("timeout WaitForHigh\n");
  
  return microseconds;
}

u8 CalculateParity(u8 HumidityHigh, u8 HumidityLow, u8 TemperatureHigh, u8 TemperatureLow) {
  return (HumidityHigh + HumidityLow + TemperatureHigh + TemperatureLow);
}

void ProcessData(u64 Data){
  u8 HumidityHigh =  (Data >> (4*8)) & 0xFF;
  u8 HumidityLow = (Data >> (3*8)) & 0xFF; // 0b11111111
  u8 TemperatureHigh = (Data >> (2*8)) & 0xFF; 
  u8 TemperatureLow = (Data >> (8)) & 0xFF;
  u8 Parity = Data  & 0xFF;

  if(CalculateParity(HumidityHigh, HumidityLow, TemperatureHigh, TemperatureLow) == Parity){
    humidity = HumidityHigh;
    temperature = TemperatureHigh;
  }
  else{
    pr_err("Error parity, HumidityHigh:%d, HumidityLow:%d, TemperatureHigh:%d, TemperatureLow:%d, Parity:%d \n"
        ,HumidityHigh,HumidityLow,TemperatureHigh,TemperatureLow, Parity);
  }

}


void Measure(void){
  u64 data=0;
  s64 low, high;
  int i;
  SendStartSignal();
  WaitForLow();
  WaitForHigh();
  WaitForLow();
  
 
  for(i=0; i<40; ++i){
    data <<=1; /* add 0 at the end */
    //data = ( WaitForHigh() < WaitForLow()) ? data : data | 1 ;
    low = WaitForHigh(); /* mesurer la duré de 0 */
    high = WaitForLow();

    if(high > low){
      data |= 1;
    }
    
    /* if (WaitForLow() > WaitForHigh()){
     *  data = data; pas de changemen car c'est 0 la valeur recu, 
     *  et on a ajouté 0 apres data dans la ligne précédente (data<<=1;
     *  }
     *  else{
     *  on a recu 1:
     *  data = data | 1; ( ajoute 1 à la fin)
     * */
  }

  WaitForHigh();
  gpio_direction_output(dht11_pin,1);
  ProcessData(data);

}



static int my_dth11_probe(struct platform_device *pdev) {
    //int ret_val, irq;
    struct device *dev = &pdev->dev;

    pr_info("my_dth11_probe enter\n");
    dev_info(dev, "my_dth11_probe() function is called.\n");
    of_property_read_u32(pdev->dev.of_node,"id_sensor",&dht11_pin); /* id_broche (propriété qui contien */
    gpio_request(dht11_pin,"data");
  
    gpio_direction_output(dht11_pin,1);

    pr_info(" dht11_pin = %d\n",dht11_pin);
    temperature_thread = kthread_run(temperature_monitor, NULL, "temperature_thread");
    if (IS_ERR(temperature_thread)) {
   	 pr_err("Failed to create temperature_thread\n");
   	 return PTR_ERR(temperature_thread);
    }

    return 0;
}

static int  my_dth11_remove(struct platform_device *pdev) {
    if (temperature_thread) {
    	kthread_stop(temperature_thread);
    	temperature_thread = NULL;
     }
    gpio_free(dht11_pin);
    return 0;
}

static const struct of_device_id my_dth11_of_ids[] = {
        { .compatible = "training,dht11platform"},
        {},
};


static ssize_t my_dev_read(struct file *flip, char *buf, size_t len, loff_t *off)
{
    char kbuf[250];
    size_t lenkbuf;
    pr_info("Read operation...\n");
    Measure();

    if(*off == 0){
      
      sprintf(kbuf,"%s \n",temperature);
      
      lenkbuf = strlen(kbuf);
      if(copy_to_user(buf,kbuf,lenkbuf) != 0) // return nb caractère non copier, si 0 c'est reussi
        return -EFAULT;
      ++(*off);
      return lenkbuf;
    }
    return 0;
}



/* Declare a file_operations structure */
/*
static const struct file_operations my_dev_fops = {
  .owner = THIS_MODULE,
  .read = my_dev_read,
};
*/

static const struct file_operations my_dev_fops = {
    .owner = THIS_MODULE,
    .read = my_dev_read_history,
    .llseek = noop_llseek, // Ajoutez cette ligne pour éviter les erreurs de llseek
};


MODULE_DEVICE_TABLE(of, my_dth11_of_ids);

/*
static struct miscdevice my_miscdevice = {
        .fops = &my_dev_fops,
        .minor = MISC_DYNAMIC_MINOR,
        .name = "my_dht11", //le nom dans /dev/
        .mode = 0666,
};
*/
static struct temperature_device temperature_dev = {
    .miscdev = {
        .fops = &my_dev_fops,
        .minor = MISC_DYNAMIC_MINOR,
        .name = "my_dht11",
        .mode = 0666,
    },
};


static struct platform_driver my_dth11_platform_driver = {
        .probe = my_dth11_probe,
        .remove = my_dth11_remove,
        .driver = {
                .name = "dht11",
                .of_match_table = my_dth11_of_ids,
                .owner = THIS_MODULE,
        }
};


static int __init my_init(void) {
    int ret_val;

    ret_val = platform_driver_register(&my_dth11_platform_driver);
    if (ret_val !=0)
    {
        pr_err("platform dht11_capt4 returned %d\n", ret_val);
        return ret_val;
    }


    pr_info("Misc registering...\n");
    //ret_val = misc_register(&my_miscdevice)
    ret_val = misc_register(&temperature_dev.miscdev);
    if (ret_val != 0) {
            pr_err("could not register the misc device mydev\n");
            return ret_val;
    }
    dht11_pin = 4;
    gpio_direction_output(dht11_pin,1);

    pr_info(" dht11_pin = %d\n",dht11_pin);

    return 0;
}

static void __exit my_exit(void) {
    pr_info("Deregister misc device...\n");
    misc_deregister(&temperature_dev.miscdev);
    platform_driver_unregister(&my_dth11_platform_driver);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fanasina");
MODULE_DESCRIPTION("dth11 module capture temperature and humidity");
