# device_unload.sh
#!/bin/bash

# 關機自動執行的腳本 
# 卸載: 紅外線循跡 + 馬達控制器 + 超聲波測距


# 卸載 IRS_90a Device Driver 模組
echo ">>> Unloading IRS_90a Device Driver..."
if lsmod | grep -q irs_90a_driver; then
	rmmod irs_90a_driver
	echo "IRS_90a Device Driver Unload Successful"
else 
	echo "Warning! IRS_90a Device Driver was not unloaded"
fi




# 卸載 IRS_90a HAL 模組 
echo ">>> Unloading IRS_90a HAL..."
if lsmod | grep -q irs_90a_hal; then
	rmmod irs_90a_hal
	echo "IRS_90a HAL Unload Successful"
else
	echo "Warning! IRS_90a HAL was not unloaded"
fi

echo ">>> Unload script finished"
