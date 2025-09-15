# Makefile for IRS_90 kernel module

# 你的 kernel module 名稱，跟 irs_90a_driver.c 對應
obj-m := irs_90a_driver.o irs_90a_hal.o

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
