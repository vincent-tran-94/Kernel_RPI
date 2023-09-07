obj-m := dht11_capteur_v1.o
KERNEL_DIR ?= $(HOME)/Alten/Linux_noyau/rpi3b64/linux

all:
	make -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- \
	M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- \
	M=$(PWD) clean

deploy:
	scp *.ko pi@192.168.1.117:/home/pi
