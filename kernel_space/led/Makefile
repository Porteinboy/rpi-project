# Makefile for LED kernel module

# LED 驅動模組名稱
obj-m := led_driver.o led_hal.o

EXTRA_CFLAGS += -I$(PWD)/../../include

# Kernel source path
KDIR ?= /lib/modules/$(shell uname -r)/build

# 當前目錄
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install INSTALL_MOD_PATH=$(PWD)/install

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
