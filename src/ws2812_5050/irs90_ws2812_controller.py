# irs90_ws2812_controller.py
import time
import board
import neopixel
import os

# WS2812 設定
LED_COUNT = 8
LED_PIN = board.D18
ORDER = neopixel.GRB
BRIGHTNESS = 0.5

pixels = neopixel.NeoPixel(
    LED_PIN, LED_COUNT, brightness=BRIGHTNESS, auto_write=False, pixel_order=ORDER
)

# IRS90 sensor 裝置
SENSOR_DEVICE = "/dev/irs90_all"

# 對應規則：A sensor 控制左側 3 顆，B sensor 中間 2 顆，C sensor 右側 3 顆
SENSOR_MAP = {
    0: range(0, 3),  # A sensor -> LED 0~2
    1: range(3, 5),  # B sensor -> LED 3~4
    2: range(5, 8),  # C sensor -> LED 5~7
}

COLOR_ON = (255, 0, 0)   # 感測到物體時紅燈
COLOR_OFF = (0, 255, 0)  # 未偵測物體時綠燈

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

def update_leds(sensor_status):
    # 初始化全部為關燈顏色（這裡改為綠色）
    pixels.fill(COLOR_OFF)

    for idx, s in enumerate(sensor_status):
        if s == "1":
            for led_idx in SENSOR_MAP[idx]:
                pixels[led_idx] = COLOR_ON

    pixels.show()

def main():
    print("IRS90 -> WS2812 Controller started")
    try:
        while True:
            status = read_sensors()
            update_leds(status)
            time.sleep(0.1)
    except KeyboardInterrupt:
        # Ctrl+C 結束時將 LED 全部設為關燈顏色（綠色）
        pixels.fill(COLOR_OFF)
        pixels.show()
        print("Exiting...")

if __name__ == "__main__":
    main()
