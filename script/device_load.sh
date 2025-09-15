# device_load.sh
#!/bin/bash

# 開機自動執行的腳本 
# 掛載:


# 1.定義模組路徑
MODULE_DIR="/root/rpi_project/kernel_space/irs_90"
HAL_MODULE="$MODULE_DIR/irs_90a_hal.ko"
DRIVER_MODULE="$MODULE_DIR/irs_90a_driver.ko"


# 2. 載入 HAL 模組
echo ">>> Loading IRS_90a Device HAL..."
if ! (lsmod | grep -q irs_90a_hal); then
	insmod "$HAL_MODULE"			# 載入 hal.ko 模組
	echo "HAL loaded"
else 
	echo "HAL already loaded"		# 若已載入 顯示訊息
fi



# 3. 載入 Device Driver 模組
echo ">>> Loading IRS_90a Device Driver..."
if ! (lsmod | grep -q irs_90a_driver); then
	insmod "$DRIVER_MODULE"			# 載入 driver.ko 模組
	echo "Device Driver loaded"
else
	echo "Device Driver already loaded"	# 若已載入 顯示訊息
fi



# 4. 檢查所有模組是否有失敗的
if ! (lsmod | grep -q irs_90a_hal); then 
	echo "IRS_90a HAL load failed!Please check out"
elif ! (lsmod | grep -q irs_90a_driver); then
	echo "IRS_90a Device Driver load failed!Please check out"
else
	echo ">>> All modules load successed"
fi



