#!/bin/bash
# 關機自動執行的腳本
# 卸載: IRS_90, LED, UART 模組

# 卸載 LED Device Driver
echo ">>> Unloading LED Device Driver..."
if lsmod | grep -q led_driver; then
    rmmod led_driver && echo "LED Device Driver Unload Successful" || echo "Failed to unload LED Device Driver"
else
    echo "Warning! LED Device Driver was not loaded"
fi

# 卸載 LED HAL
echo ">>> Unloading LED HAL..."
if lsmod | grep -q led_hal; then
    rmmod led_hal && echo "LED HAL Unload Successful" || echo "Failed to unload LED HAL"
else
    echo "Warning! LED HAL was not loaded"
fi

# 卸載 IRS_90 Device Driver
echo ">>> Unloading IRS_90 Device Driver..."
if lsmod | grep -q irs_90_driver; then
    rmmod irs_90_driver && echo "IRS_90 Device Driver Unload Successful" || echo "Failed to unload IRS_90 Driver"
else
    echo "Warning! IRS_90 Device Driver was not loaded"
fi

# 卸載 IRS_90 HAL
echo ">>> Unloading IRS_90 HAL..."
if lsmod | grep -q irs_90_hal; then
    rmmod irs_90_hal && echo "IRS_90 HAL Unload Successful" || echo "Failed to unload IRS_90 HAL"
else
    echo "Warning! IRS_90 HAL was not loaded"
fi

# 卸載 UART Driver
echo ">>> Unloading UART Driver..."
if lsmod | grep -q uart; then
    rmmod uart && echo "UART Driver Unload Successful" || echo "Failed to unload UART Driver"
else
    echo "Warning! UART Driver was not loaded"
fi

echo ">>> Unload script finished"
