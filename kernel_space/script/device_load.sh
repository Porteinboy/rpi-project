# device_load.sh
#!/bin/bash

# 開機自動執行的腳本
# 掛載: IRS_90 感測器 + LED 驅動 + UART 驅動

# 1.定義模組路徑
MODULE_DIR="/root/rpi_project/kernel_modules"

# IRS_90 模組
IRS_HAL_MODULE="$MODULE_DIR/irs_90_hal.ko"
IRS_DRIVER_MODULE="$MODULE_DIR/irs_90_driver.ko"

# LED 模組
LED_HAL_MODULE="$MODULE_DIR/led_hal.ko"
LED_DRIVER_MODULE="$MODULE_DIR/led_driver.ko"

# UART 模組
UART_MODULE="$MODULE_DIR/uart.ko"
UART_DEVICE="uart"

# 2. 載入 IRS_90 HAL 模組
echo ">>> Loading IRS_90 Device HAL..."
if ! (lsmod | grep -q irs_90_hal); then
	insmod "$IRS_HAL_MODULE"			# 載入 hal.ko 模組
	echo "HAL loaded"
else 
	echo "HAL already loaded"		# 若已載入 顯示訊息
fi

# 3. 載入 IRS_90 Device Driver 模組
echo ">>> Loading IRS_90 Device Driver..."
if ! (lsmod | grep -q irs_90_driver); then
	insmod "$IRS_DRIVER_MODULE"			# 載入 driver.ko 模組
	echo "Device Driver loaded"
else
	echo "Device Driver already loaded"	# 若已載入 顯示訊息
fi

# 4. 載入 LED HAL 模組
echo ">>> Loading LED HAL..."
if ! (lsmod | grep -q led_hal); then
    if [ -f "$LED_HAL_MODULE" ]; then
        insmod "$LED_HAL_MODULE"
        if [ $? -eq 0 ]; then
            echo "LED HAL loaded successfully"
        else
            echo "Failed to load LED HAL"
        fi
    else
        echo "LED HAL module not found"
    fi
else
    echo "LED HAL already loaded"
fi

# 5. 載入 LED Driver 模組
echo ">>> Loading LED Device Driver... "
if ! (lsmod | grep -q led_driver); then
	if [ -f "$LED_DRIVER_MODULE" ]; then
		insmod "$LED_DRIVER_MODULE"
		if [ $? -eq 0 ]; then
			echo "LED Device loaded successfully"
		else
			echo "Failed to load LED Device Driver"
		fi
	else
		echo "LED Driver module not found: $LED_DRIVER_MODULE"
	fi
else
	echo "LED Device Driver already loaded"
fi

#6. 載入 UART Driver 模組
echo ">>> Loading UART Driver... "
if ! (lsmod | grep -q uart); then
	if [ -f "$UART_MODULE" ]; then
		insmod "$UART_MODULE"
		if [ $? -eq 0 ]; then
			echo "UART Driver loaded successfully"
		else
			echo "Failed to load UART Driver"
		fi
	else
		echo "UART Driver module not found: $UART_MODULE"
	fi
else
	echo "UART Driver already loaded"
fi

#7. 創建 UART 裝置節點
UART_MAJOR=$(awk "\$2==\"$UART_DEVICE\" {print \$1}" /proc/devices)
if [ -n "$UART_MAJOR" ]; then
	echo "UART Major number = $UART_MAJOR"
	rm -f /dev/${UART_DEVICE}*
	mknod /dev/uart2 c $UART_MAJOR 0
	mknod /dev/uart3 c $UART_MAJOR 1
	chmod 664 /dev/uart2 /dev/uart3
	echo "/dev/uart2 and /dev/uart3 created"
else
	echo "Failed to find UART major number"
fi
