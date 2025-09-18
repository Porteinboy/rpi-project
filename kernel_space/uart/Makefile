# Makefile for UART kernel driver

# UART 驅動模組名稱
obj-m += uart.o

MODULES_DIR := $(PWD)/../../kernel_modules

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	# 將編譯出來的 .ko 複製到 modules 資料夾
	@mkdir -p $(MODULES_DIR)
	@cp -v *.ko $(MODULES_DIR)/

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
