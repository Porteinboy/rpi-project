import time
import board
import neopixel

# 設定 WS2812 LED
LED_COUNT = 8          # LED 顆數
LED_PIN = board.D18    # BCM 18 對應 PWM0 (必須硬體 PWM)
ORDER = neopixel.GRB   # WS2812 順序

pixels = neopixel.NeoPixel(LED_PIN, LED_COUNT, brightness=0.5, auto_write=False, pixel_order=ORDER)

# 顏色循環函數
def color_wipe(color, wait=0.1):
    for i in range(LED_COUNT):
        pixels[i] = color
        pixels.show()
        time.sleep(wait)

# 簡單 RGB 循環
try:
    while True:
        color_wipe((255, 0, 0))  # 紅
        color_wipe((0, 255, 0))  # 綠
        color_wipe((0, 0, 255))  # 藍
except KeyboardInterrupt:
    pixels.fill((0, 0, 0))  # 關掉 LED
    pixels.show()
