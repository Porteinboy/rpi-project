#!/bin/bash
# 關機自動執行的腳本
# 先停止背景程式，再卸載 IRS_90, LED, UART 模組

# 1. 停止背景程式
echo ">>> Stopping background processes..."

# 背景程式列表
BG_PROGS=(
    irs90_mqtt_publisher
    irs90_uart
    sensor_led_controller
    irs90_ws2812_controller.py
)

for prog in "${BG_PROGS[@]}"; do
    PID=$(pgrep -f "$prog")
    if [ -n "$PID" ]; then
        echo "Stopping $prog (PID=$PID)..."
        # 用 SIGINT 讓程式安全結束
        kill -2 $PID
        # 等待程式退出
        sleep 1
    else
        echo "$prog not running"
    fi
done

# 2. 卸載 LED Device Driver
echo ">>> Unloading LED Device Driver..."
if lsmod | grep -q led_driver; then
    rmmod led_driver && echo "LED Device Driver Unload Successful" || echo "Failed to unload LED Device Driver"
else
    echo "Warning! LED Device Driver was not loaded"
fi

# 3. 卸載 LED HAL
echo ">>> Unloading LED HAL..."
if lsmod | grep -q led_hal; then
    rmmod led_hal && echo "LED HAL Unload Successful" || echo "Failed to unload LED HAL"
else
    echo "Warning! LED HAL was not loaded"
fi

# 4. 卸載 IRS_90 Device Driver
echo ">>> Unloading IRS_90 Device Driver..."
if lsmod | grep -q irs_90_driver; then
    rmmod irs_90_driver && echo "IRS_90 Device Driver Unload Successful" || echo "Failed to unload IRS_90 Driver"
else
    echo "Warning! IRS_90 Device Driver was not loaded"
fi

# 5. 卸載 IRS_90 HAL
echo ">>> Unloading IRS_90 HAL..."
if lsmod | grep -q irs_90_hal; then
    rmmod irs_90_hal && echo "IRS_90 HAL Unload Successful" || echo "Failed to unload IRS_90 HAL"
else
    echo "Warning! IRS_90 HAL was not loaded"
fi

# 6. 卸載 UART Driver
echo ">>> Unloading UART Driver..."
if lsmod | grep -q uart; then
    rmmod uart && echo "UART Driver Unload Successful" || echo "Failed to unload UART Driver"
else
    echo "Warning! UART Driver was not loaded"
fi

echo ">>> Unload script finished"
