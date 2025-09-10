// Car Sensors GPIO Definitions
// 自動駕駛車輛感測器 GPIO 腳位對應定義檔
// 此檔案定義所有硬體元件與 Raspberry Pi GPIO 腳位的對應關係

// ==================== 通訊介面 ====================
// UART 串列通訊 ( Raspberry Pi 4 <-> Pico W 微控制器 )
// 用於 Pi 與 Pico W 之間的資料交換，例如感測器數據、控制指令等
// GPIO 14 TXD  - 傳送資料線 (Transmit Data)
// GPIO 15 RXD  - 接收資料線 (Receive Data)

// ==================== TCRT5000 循跡感測器 ====================
// 紅外線反射式感測器，用於偵測地面黑線進行循跡
// 感測器配置：三個感測器呈一直線排列，提供左中右的路線偵測

// 左側感測器 (GPIO16)
#define TCRT5000_LEFT 16               // 左側TCRT5000感測器連接到GPIO16
#define GPIO16_FSEL  GPFSEL1           // GPIO16功能選擇暫存器為GPFSEL1
#define GPIO16_BIT_SHIFT 18            // GPIO16在GPFSEL1中的位元位置 (16*3=48, 48%32=16, 但實際是18)
                                       // 計算方式：GPIO16使用GPFSEL1的第18-20位元來設定功能

// 右側感測器 (GPIO17)  
#define TCRT5000_RIGHT 17              // 右側TCRT5000感測器連接到GPIO17
#define GPIO17_FSEL  GPFSEL1           // GPIO17功能選擇暫存器為GPFSEL1
#define GPIO17_BIT_SHIFT 21            // GPIO17在GPFSEL1中的位元位置
                                       // 計算方式：GPIO17使用GPFSEL1的第21-23位元來設定功能

// 中間感測器 (GPIO18)
#define TCRT5000_MIDDLE 18             // 中間TCRT5000感測器連接到GPIO18
#define GPIO18_FSEL  GPFSEL1           // GPIO18功能選擇暫存器為GPFSEL1
#define GPIO18_BIT_SHIFT 24            // GPIO18在GPFSEL1中的位元位置
                                       // 計算方式：GPIO18使用GPFSEL1的第24-26位元來設定功能

/*
 * TCRT5000 工作原理：
 * - 發射紅外線並接收反射訊號
 * - 白色/淺色表面：強反射 → 輸出低電位 (0) → 無物體偵測
 * - 黑色/深色表面：弱反射 → 輸出高電位 (1) → 偵測到黑線
 * 
 * 循跡邏輯範例：
 * - "0 1 0" → 黑線在中間，直行
 * - "1 0 0" → 黑線偏左，向右修正
 * - "0 0 1" → 黑線偏右，向左修正
 * - "0 0 0" → 無黑線，可能脫軌或到達終點
 * - "1 1 1" → 全黑，可能是起點/終點標記
 */

// ==================== L298N 馬達控制器 ====================
// 雙H橋馬達驅動晶片，用於控制兩個直流馬達的轉向和速度
// 支援前進、後退、左轉、右轉、煞車等動作
// TODO: 定義馬達控制腳位
// 建議腳位配置：
// #define MOTOR_LEFT_EN   12     // 左馬達PWM速度控制
// #define MOTOR_LEFT_IN1  5      // 左馬達方向控制1
// #define MOTOR_LEFT_IN2  6      // 左馬達方向控制2
// #define MOTOR_RIGHT_EN  13     // 右馬達PWM速度控制  
// #define MOTOR_RIGHT_IN1 19     // 右馬達方向控制1
// #define MOTOR_RIGHT_IN2 26     // 右馬達方向控制2

// ==================== SRF05 超音波測距感測器 ====================
// 用於前方障礙物偵測，避障功能實作
// 工作原理：發射超音波脈衝，計算回波時間來測量距離
// TODO: 定義超音波感測器腳位
// 建議腳位配置：
// #define ULTRASONIC_TRIG 23     // 觸發腳位 (Trigger) - 發射超音波
// #define ULTRASONIC_ECHO 24     // 回音腳位 (Echo) - 接收反射波
// 測距範圍：約2cm-400cm
// 精度：±3mm

// ==================== LED 狀態指示燈 ====================
// 多色LED燈號系統，用於顯示車輛當前狀態
// 模擬真實車輛的燈號系統，提供視覺化的狀態回饋

// 主要狀態燈
//#define GREEN_LED    xx        // 綠燈 - 車輛正常行駛狀態
                                 // 表示：循跡正常、無障礙物、系統運作正常

//#define YELLOW_LED   xx        // 黃燈 - 異常狀況警示
                                 // 表示：偵測到障礙物、感測器異常、需要人工介入

//#define RED_LED      xx        // 紅燈 - 車輛停止狀態  
                                 // 表示：緊急停止、系統錯誤、等待指令

// 方向指示燈
//#define ORANGE_LEFT_LED  xx    // 左橘燈 - 左轉方向燈
                                 // 表示：車輛準備或正在左轉

//#define ORANGE_RIGHT_LED xx    // 右橘燈 - 右轉方向燈
                                 // 表示：車輛準備或正在右轉

// 功能燈號
//#define WHITE_LED    xx        // 白燈 - 倒車燈
                                 // 表示：車輛正在後退

/*
 * LED 控制邏輯建議：
 * 
 * 正常循跡：GREEN 恆亮
 * 左轉修正：GREEN + ORANGE_LEFT 閃爍
 * 右轉修正：GREEN + ORANGE_RIGHT 閃爍  
 * 障礙物偵測：YELLOW 閃爍
 * 緊急停止：RED 恆亮，其他燈號關閉
 * 倒車避障：WHITE + RED 同時亮起
 * 系統初始化：所有燈號依序閃爍測試
 */

// ==================== 腳位分配注意事項 ====================
/*
 * Raspberry Pi 4 GPIO 腳位使用限制：
 * 
 * 已使用腳位：
 * - GPIO 14, 15: UART 通訊 (與Pico W通訊)
 * - GPIO 16, 17, 18: TCRT5000 循跡感測器
 * 
 * 建議避免的腳位：
 * - GPIO 0, 1: I2C ID EEPROM (系統保留)
 * - GPIO 2, 3: I2C1 SDA/SCL (可用但建議保留給I2C裝置)
 * - GPIO 4: 1-Wire (溫度感測器等)
 * - GPIO 7, 8, 9, 10, 11: SPI0 (可用於其他SPI裝置)
 * 
 * 可用腳位推薦：
 * - PWM腳位: GPIO 12, 13 (硬體PWM，適合馬達控制)
 * - 一般IO: GPIO 5, 6, 19, 20, 21, 22, 23, 24, 25, 26, 27
 * 
 * 電源與接地：
 * - 3.3V: Pin 1, 17
 * - 5V: Pin 2, 4 (L298N馬達驅動器需要5V供電)
 * - GND: Pin 6, 9, 14, 20, 25, 30, 34, 39
 */

// ==================== 系統整合建議 ====================
/*
 * 模組載入順序：
 * 1. tcrt5000_hal.ko     (硬體抽象層)
 * 2. tcrt5000_driver.ko  (循跡感測器驅動)
 * 3. motor_driver.ko     (馬達控制驅動)
 * 4. ultrasonic_driver.ko (超音波感測器驅動)
 * 5. led_driver.ko       (LED控制驅動)
 * 6. car_control.ko      (主控制程式)
 * 
 * 裝置節點建議：
 * - /dev/tcrt5000        (循跡感測器)
 * - /dev/motor           (馬達控制)
 * - /dev/ultrasonic      (超音波測距)
 * - /dev/car_leds        (LED狀態燈)
 */
