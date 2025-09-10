/*
 * TCRT5000 硬體抽象層標頭檔 (tcrt5000_hal.h)
 * 
 * 功能說明：
 * 1. 定義HAL層與驅動層之間的介面
 * 2. 提供函數原型宣告，避免編譯時的未定義錯誤
 * 3. 建立模組間的標準化通訊協定
 * 
 * 設計目的：
 * - 分離硬體操作與邏輯控制
 * - 提供統一的GPIO讀取介面
 * - 支援模組化開發架構
 */

// TCRT5000 HAL與驅動層間使用的標頭檔
// 避免read_gpio()函數在編譯時出現未定義錯誤

#ifndef __TCRT5000_HAL_H__
#define __TCRT5000_HAL_H__

/*
 * ==================== 標頭檔保護說明 ====================
 * 
 * #ifndef __TCRT5000_HAL_H__：檢查巨集是否未定義
 * #define __TCRT5000_HAL_H__：定義保護巨集
 * #endif：結束條件編譯區塊
 * 
 * 作用：
 * - 防止標頭檔被重複包含 (Multiple Inclusion Guard)
 * - 避免重複定義錯誤
 * - 提升編譯效率
 * 
 * 當多個檔案都 #include "tcrt5000_hal.h" 時，
 * 只有第一次包含會實際處理檔案內容，
 * 後續的包含會被跳過。
 */

// ==================== 函數原型宣告 ====================

/**
 * read_gpio - 讀取指定GPIO腳位的輸入狀態
 * @gpio: GPIO腳位編號，使用pin_mapping.h中定義的常數
 *        - TCRT5000_LEFT   (GPIO16) - 左側感測器
 *        - TCRT5000_MIDDLE (GPIO18) - 中間感測器  
 *        - TCRT5000_RIGHT  (GPIO17) - 右側感測器
 * 
 * 回傳值:
 *   0 - GPIO為低電位 (感測器未偵測到物體/偵測到白線)
 *   1 - GPIO為高電位 (感測器偵測到物體/偵測到黑線)
 *  -1 - 錯誤：無效的GPIO編號
 * 
 * 使用範例:
 *   int left_sensor = read_gpio(TCRT5000_LEFT);
 *   if (left_sensor == 1) {
 *       // 左側感測器偵測到黑線，執行右轉修正
 *   }
 * 
 * 注意事項:
 *   1. 此函數由HAL層模組(tcrt5000_hal.ko)提供實作
 *   2. 驅動層模組(tcrt5000_driver.ko)透過此介面讀取感測器
 *   3. 必須先載入HAL層模組，驅動層才能正常運作
 *   4. 函數透過EXPORT_SYMBOL()匯出給其他核心模組使用
 */
int read_gpio(int gpio);

#endif // __TCRT5000_HAL_H__

/*
 * ==================== 模組相依性說明 ====================
 * 
 * 依賴關係：
 *   tcrt5000_driver.c → #include "tcrt5000_hal.h" → read_gpio()函數
 *   tcrt5000_hal.c    → 實作read_gpio()函數並匯出符號
 * 
 * 編譯時：
 *   - 驅動層編譯時透過此標頭檔得知read_gpio()的函數簽章
 *   - 避免"implicit function declaration"警告
 *   - 確保函數呼叫的參數和回傳值型別正確
 * 
 * 執行時：
 *   - HAL層載入後，read_gpio()符號被註冊到核心符號表
 *   - 驅動層載入時，動態連結找到並使用該函數
 *   - 如果HAL層未載入，驅動層載入會失敗
 */

/*
 * ==================== 設計模式：硬體抽象層 ====================
 * 
 * 優點：
 * 1. 硬體隔離：驅動層不需直接操作GPIO暫存器
 * 2. 介面統一：所有感測器讀取都使用相同函數
 * 3. 易於測試：可以替換HAL實作進行單元測試
 * 4. 平台移植：更換硬體平台時只需修改HAL層
 * 5. 程式碼複用：多個驅動可共用同一HAL層
 * 
 * 層級架構：
 *   應用程式
 *       ↓ (系統呼叫)
 *   裝置驅動層 (tcrt5000_driver.ko)
 *       ↓ (函數呼叫)
 *   硬體抽象層 (tcrt5000_hal.ko)
 *       ↓ (暫存器操作)
 *   GPIO控制器硬體
 * 
 * 標頭檔作用：
 *   定義各層間的介面契約(Interface Contract)
 */

/*
 * ==================== 擴展性考量 ====================
 * 
 * 未來可能的函數擴展：
 * 
 * // GPIO寫入函數
 * int write_gpio(int gpio, int value);
 * 
 * // GPIO方向設定
 * int set_gpio_direction(int gpio, int direction);
 * 
 * // 中斷處理相關
 * int enable_gpio_interrupt(int gpio, int edge_type);
 * int disable_gpio_interrupt(int gpio);
 * 
 * // 多感測器批次讀取
 * int read_multiple_gpio(int *gpios, int *values, int count);
 * 
 * // 感測器狀態解析
 * typedef struct {
 *     int left;
 *     int middle; 
 *     int right;
 *     unsigned long timestamp;
 * } tcrt5000_status_t;
 * 
 * int read_tcrt5000_status(tcrt5000_status_t *status);
 */

/*
 * ==================== 使用建議 ====================
 * 
 * 在其他核心模組中使用此HAL：
 * 
 * 1. 包含標頭檔：
 *    #include "tcrt5000_hal.h"
 * 
 * 2. 呼叫函數：
 *    int sensor_value = read_gpio(TCRT5000_LEFT);
 * 
 * 3. 錯誤處理：
 *    if (sensor_value < 0) {
 *        printk(KERN_ERR "Failed to read sensor\n");
 *        return -EIO;
 *    }
 * 
 * 4. Makefile相依性：
 *    obj-m += your_driver.o
 *    your_driver-objs := your_driver.o
 *    # 確保HAL模組先編譯
 */
