# Makefile for UART kernel module

# UART 驅動模組名稱
obj-m := uart_hal.o uart_driver.o

EXTRA_CFLAGS += -I$(PWD)/../include

# Kernel source path
KDIR ?= /lib/modules/$(shell uname -r)/build

# 當前目錄
PWD := $(shell pwd)

MODULES_DIR := $(PWD)/../../kernel_modules

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	# 將編譯出來的 .ko 複製到 modules 資料夾
	@mkdir -p $(MODULES_DIR)
	@cp -v *.ko $(MODULES_DIR)/

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install INSTALL_MOD_PATH=$(MODULES_DIR)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
