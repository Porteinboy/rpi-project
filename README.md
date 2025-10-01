## **1. 專案概述**

* **專案名稱**：IRS90 感測器與 WS2812 LED 控制系統
* **硬體平台**：Raspberry Pi 4 (BCM2711)
* **核心功能**：

  1. IRS90 偵測車位 / 物體狀態
  2. LED (WS2812/5050) 即時反饋感測狀態
  3. UART 通訊與 MQTT 發布資料
* **目標**：

  * Kernel 驅動穩定管理感測器、LED 與 UART
  * User-space 程式讀取感測器、控制 LED、發送資料
  * Python 控制 WS2812 LED，可背景執行並支援 log
  * pptx: https://docs.google.com/presentation/d/1WiRNUzjZaT-H-pkTQES2LoDNdEh6kiOz/edit?usp=sharing&ouid=107129721443512939476&rtpof=true&sd=true

---

## **2. 專案檔案架構**

```
rpi_project/
├── bin/                          # 編譯完成的可執行檔
│   ├── irs90_mqtt_publisher
│   ├── irs90_uart
│   ├── sensor_led_controller
├── build/                        # 編譯產物
├── docs/                         # 文件
├── kernel_space/                 # kernel driver 原始碼
│   ├── irs_90/                   # IRS90 HAL + Driver
│   ├── led/                      # LED HAL + Driver
│   ├── uart/                     # UART driver
│   └── script/                   # device_load/unload.sh
├── modules/                      # 編譯好的 ko 模組
│   ├── irs_90_driver.ko
│   ├── irs_90_hal.ko
│   ├── led_driver.ko
│   ├── led_hal.ko
│   └── uart.ko
└── src/
    ├── led/
    │   └── sensor_led_controller.c
    ├── mqtt/
    │   └── irs90_mqtt_publisher.c
    ├── uart/
    │   └── irs90_uart.c
    └── ws2812_5050/
        ├── irs90_ws2812_controller.py
        └── rpi_ws2812_env/       # Python 虛擬環境
```

---

## **3. Kernel Space 模組**

| 模組                 | 功能                |
| ------------------ | ----------------- |
| irs\_90\_hal.ko    | IRS90 硬體抽象層 (HAL) |
| irs\_90\_driver.ko | IRS90 核心驅動        |
| led\_hal.ko        | LED HAL           |
| led\_driver.ko     | LED Device Driver |
| uart.ko            | UART 驅動           |

* **device\_load.sh**：

  * 載入 HAL → Driver → UART
  * 建立 `/dev/uart2`、`/dev/uart3`
  * 背景啟動 user-space 程式與 Python 控制 LED
* **device\_unload.sh**：

  * 停止所有背景程式
  * 依序卸載 LED、IRS90、UART 驅動

---

## **4. User-Space 程式**

| 程式                           | 功能                         |
| ---------------------------- | -------------------------- |
| irs90\_mqtt\_publisher       | 讀取 IRS90 資料並發送 MQTT        |
| irs90\_uart                  | UART 封裝，與 IRS90 driver 通訊  |
| sensor\_led\_controller      | 控制 LED 反映 IRS90 感測狀態       |
| irs90\_ws2812\_controller.py | Python 控制 WS2812 LED (8 顆) |

---

## **5. Python WS2812 控制邏輯**

### **功能**

1. 初始狀態 → LED 全綠
2. 感測到物體 → 對應 LED 變紅
3. 未偵測 → 對應 LED 綠色
4. Ctrl+C 或程式被 kill → LED 全關

### **程式核心**

```python
COLOR_ON = (255, 0, 0)    # 紅燈
COLOR_OFF = (0, 255, 0)   # 綠燈
COLOR_EXIT = (0, 0, 0)    # 全關

def read_sensors():
    with open("/dev/irs90_all", "r") as f:
        return f.read(3).strip().ljust(3, "0")

def update_leds(sensor_status):
    pixels.fill(COLOR_OFF)
    for idx, s in enumerate(sensor_status):
        if s == "1":
            for led_idx in SENSOR_MAP[idx]:
                pixels[led_idx] = COLOR_ON
    pixels.show()

def safe_exit(signum=None, frame=None):
    pixels.fill(COLOR_EXIT)
    pixels.show()
    exit(0)
```

* 信號處理：

```python
signal.signal(signal.SIGINT, safe_exit)
signal.signal(signal.SIGTERM, safe_exit)
```

---

## **6. 背景執行 & Log 管理**

* Python 程式透過 device\_load.sh 背景執行：

```bash
source /root/rpi_project/src/ws2812_5050/rpi_ws2812_env/bin/activate
python3 irs90_ws2812_controller.py > /root/rpi_project/logs/irs90_ws2812_controller.log 2>&1 &
```

* 可由 device\_unload.sh 停止：

```bash
pkill -TERM -x irs90_ws2812_controller.py
```

---

## **7. 開發最佳實踐**

* 使用虛擬環境管理 Python 套件，避免 PEP 668 限制
* 背景程式有 log 紀錄
* Signal 捕捉，保證程式安全退出，LED 全關
* Kernel Driver 與 User-space 程式解耦，方便維護

---

## **8. 專案運作流程**

```
IRS90 Sensor → /dev/irs90_all → User-Space 程式 → LED Driver → LED
                                         → MQTT 發布
Python WS2812 Controller → 背景讀取 /dev/irs90_all → 控制 WS2812 LED
```

* 核心驅動提供穩定硬體存取
* User-space 程式控制硬體行為
* Python 控制 LED 視覺化反饋
* 背景執行可開機自動啟動並記錄 log
