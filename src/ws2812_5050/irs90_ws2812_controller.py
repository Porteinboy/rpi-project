# irs90_ws2812_controller.py
import time
import board
import neopixel
import os
import signal
import sys

# ------------------------------
# WS2812 LED 設定
# ------------------------------
LED_COUNT = 8
LED_PIN = board.D18       # BCM 18 對應 PWM0
ORDER = neopixel.GRB
BRIGHTNESS = 0.5

pixels = neopixel.NeoPixel(
    LED_PIN, LED_COUNT, brightness=BRIGHTNESS, auto_write=False, pixel_order=ORDER
)

# ------------------------------
# IRS90 sensor 裝置
# ------------------------------
SENSOR_DEVICE = "/dev/irs90_all"

# 對應規則：A sensor 控制左側 3 顆，B sensor 中間 2 顆，C sensor 右側 3 顆
SENSOR_MAP = {
    0: range(0, 3),  # A sensor -> LED 0~2
    1: range(3, 5),  # B sensor -> LED 3~4
    2: range(5, 8),  # C sensor -> LED 5~7
}

# 顏色設定
COLOR_ON = (255, 0, 0)   # 感測到物體時紅燈
COLOR_OFF = (0, 255, 0)  # 初始與未偵測 → 綠燈
COLOR_EXIT = (0, 0, 0)   # 程式結束 → 全關

# ------------------------------
# 讀取 IRS90 感測器
# ------------------------------
def read_sensors():
    try:
        with open(SENSOR_DEVICE, "r") as f:
            data = f.read(3)
            data = data.strip()
            if len(data) < 3:
                return "000"
            return data
    except Exception as e:
        print(f"Failed to read sensor: {e}")
        return "000"

# ------------------------------
# 更新 LED
# ------------------------------
def update_leds(sensor_status):
    # 預設全部為未偵測 → 綠燈
    pixels.fill(COLOR_OFF)

    for idx, s in enumerate(sensor_status):
        if s == "1":
            for led_idx in SENSOR_MAP[idx]:
                pixels[led_idx] = COLOR_ON

    pixels.show()

# ------------------------------
# 安全關燈函式
# ------------------------------
def safe_exit(signum=None, frame=None):
    print(f"Received signal {signum}, turning off LEDs...")
    pixels.fill(COLOR_EXIT)
    pixels.show()
    sys.exit(0)

# ------------------------------
# 主程式
# ------------------------------
def main():
    print("IRS90 -> WS2812 Controller started")

    # 設置信號處理器
    signal.signal(signal.SIGINT, safe_exit)   # Ctrl+C
    signal.signal(signal.SIGTERM, safe_exit)  # kill

    # 啟動時顯示綠燈
    pixels.fill(COLOR_OFF)
    pixels.show()

    try:
        while True:
            status = read_sensors()
            update_leds(status)
            time.sleep(0.1)
    except KeyboardInterrupt:
        safe_exit()

# ------------------------------
if __name__ == "__main__":
    main()
