# device_load.sh
#!/bin/bash

# 開機自動執行的腳本
# 掛載: IRS_90 感測器 + LED 驅動


# 1.定義模組路徑
IRS_MODULE_DIR="/root/rpi_project/kernel_space/irs_90"
LED_MODULE_DIR="/root/rpi_project/kernel_space/led"

# IRS_90 模組
IRS_HAL_MODULE="$IRS_MODULE_DIR/irs_90a_hal.ko"
IRS_DRIVER_MODULE="$IRS_MODULE_DIR/irs_90a_driver.ko"

# LED 模組
LED_DRIVER_MODULE="$LED_MODULE_DIR/led_driver.ko"

# 2. 載入 HAL 模組
echo ">>> Loading IRS_90a Device HAL..."
if ! (lsmod | grep -q irs_90a_hal); then
	insmod "$IRS_HAL_MODULE"			# 載入 hal.ko 模組
	echo "HAL loaded"
else 
	echo "HAL already loaded"		# 若已載入 顯示訊息
fi



# 3. 載入 Device Driver 模組
echo ">>> Loading IRS_90a Device Driver..."
if ! (lsmod | grep -q irs_90a_driver); then
	insmod "$IRS_DRIVER_MODULE"			# 載入 driver.ko 模組
	echo "Device Driver loaded"
else
	echo "Device Driver already loaded"	# 若已載入 顯示訊息
fi

# 4. 載入 LED HAL 模組
echo ">>> Loading LED HAL..."
if ! (lsmod | grep -q led_hal); then
    if [ -f "$LED_MODULE_DIR/led_hal.ko" ]; then
        insmod "$LED_MODULE_DIR/led_hal.ko"
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
echo " Loading LED Device Driver... "
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

# 5. 檢查所有模組是否有失敗的
if ! (lsmod | grep -q irs_90a_hal); then 
	echo "IRS_90a HAL load failed!Please check out"
fi
if ! (lsmod | grep -q irs_90a_driver); then
	echo "IRS_90a Device Driver load failed!Please check out"
fi
if ! (lsmod | grep -q led_driver); then
	echo "LED Device Driver load failed!Please check out"
fi



