# device_load.sh
#!/bin/bash

# 開機自動執行的腳本
# 掛載: IRS_90 感測器 + LED 驅動 + UART 驅動 + WS2812 控制器

# 取得專案根目錄
PROJECT_ROOT="$(cd "$(dirname "$0")/../../" && pwd)"
MODULE_DIR="$PROJECT_ROOT/modules"

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
    insmod "$IRS_HAL_MODULE"
    echo "HAL loaded"
else
    echo "HAL already loaded"
fi

# 3. 載入 IRS_90 Device Driver 模組
echo ">>> Loading IRS_90 Device Driver..."
if ! (lsmod | grep -q irs_90_driver); then
    insmod "$IRS_DRIVER_MODULE"
    echo "Device Driver loaded"
else
    echo "Device Driver already loaded"
fi

# 4. 載入 LED HAL 模組
echo ">>> Loading LED HAL..."
if ! (lsmod | grep -q led_hal); then
    [ -f "$LED_HAL_MODULE" ] && insmod "$LED_HAL_MODULE" && echo "LED HAL loaded successfully"
else
    echo "LED HAL already loaded"
fi

# 5. 載入 LED Driver 模組
echo ">>> Loading LED Device Driver..."
if ! (lsmod | grep -q led_driver); then
    [ -f "$LED_DRIVER_MODULE" ] && insmod "$LED_DRIVER_MODULE" && echo "LED Device loaded successfully"
else
    echo "LED Device Driver already loaded"
fi

# 6. 載入 UART Driver 模組
echo ">>> Loading UART Driver..."
if ! (lsmod | grep -q '^uart '); then
    [ -f "$UART_MODULE" ] && insmod "$UART_MODULE" && echo "UART Driver loaded successfully"
else
    echo "UART Driver already loaded"
fi

# 7. 創建 UART 裝置節點
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

# 8. 啟動使用者空間程式 (背景執行)
echo ">>> Starting user-space applications..."

BIN_DIR="$PROJECT_ROOT/bin"
LOG_DIR="$PROJECT_ROOT/logs"
mkdir -p "$LOG_DIR"

# IRS90 → MQTT
[ -x "$BIN_DIR/irs90_mqtt_publisher" ] && \
"$BIN_DIR/irs90_mqtt_publisher" >"$LOG_DIR/irs90_mqtt_publisher.log" 2>&1 & \
echo "irs90_mqtt_publisher started (PID=$!)"

# IRS90 UART
[ -x "$BIN_DIR/irs90_uart" ] && \
"$BIN_DIR/irs90_uart" >"$LOG_DIR/irs90_uart.log" 2>&1 & \
echo "irs90_uart started (PID=$!)"

# Sensor → LED Controller
[ -x "$BIN_DIR/sensor_led_controller" ] && \
"$BIN_DIR/sensor_led_controller" >"$LOG_DIR/sensor_led_controller.log" 2>&1 & \
echo "sensor_led_controller started (PID=$!)"

# IRS90 → WS2812 LED Controller (Python + 虛擬環境)
VENV_DIR="$PROJECT_ROOT/src/ws2812_5050/rpi_ws2812_env"
WS2812_PY="$PROJECT_ROOT/src/ws2812_5050/irs90_ws2812_controller.py"
if [ -f "$WS2812_PY" ]; then
    "$VENV_DIR/bin/python3" "$WS2812_PY" >"$LOG_DIR/irs90_ws2812.log" 2>&1 &
    echo "irs90_ws2812_controller started (PID=$!)"
else
    echo "irs90_ws2812_controller.py not found"
fi
