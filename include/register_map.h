/*
 * GPIO暫存器位址對應定義檔 (register_map.h)
 * 適用於 Raspberry Pi 4 BCM2711 晶片
 * 
 * 此檔案定義了BCM2711 GPIO控制器的所有暫存器位址偏移量
 * 基於Broadcom BCM2711官方文件規格
 */

#ifndef __GPIO_ADDRESS__
#define __GPIO_ADDRESS__

// ==================== GPIO控制器基底位址 ====================
// BCM2711 (Raspberry Pi 4) GPIO控制器的實體記憶體位址
#define GPIO_BASE 0xFE200000
/*
 * 注意：不同Raspberry Pi版本的基底位址不同
 * - Pi 1: 0x20200000
 * - Pi 2/3: 0x3F200000  
 * - Pi 4: 0xFE200000 (當前定義)
 */

// ==================== GPIO功能選擇暫存器 (GPFSEL) ====================
// 每個暫存器控制10個GPIO腳位的功能設定
// 每個GPIO使用3個位元來定義功能：000=輸入, 001=輸出, 010-111=特殊功能

#define GPFSEL0 0x00    // GPIO功能選擇暫存器0 (控制GPIO 0-9)
                        // 位元配置：GPIO0[2:0], GPIO1[5:3], GPIO2[8:6], ..., GPIO9[29:27]

#define GPFSEL1 0x04    // GPIO功能選擇暫存器1 (控制GPIO 10-19)
                        // 位元配置：GPIO10[2:0], GPIO11[5:3], ..., GPIO19[29:27]
                        // TCRT5000感測器使用的GPIO16,17,18就在此暫存器中

#define GPFSEL2 0x08    // GPIO功能選擇暫存器2 (控制GPIO 20-29)

#define GPFSEL3 0x0c    // GPIO功能選擇暫存器3 (控制GPIO 30-39)

#define GPFSEL4 0x10    // GPIO功能選擇暫存器4 (控制GPIO 40-49)

#define GPFSEL5 0x14    // GPIO功能選擇暫存器5 (控制GPIO 50-57)
                        // Pi 4只有54個GPIO，所以只使用部分位元

// ==================== GPIO輸出設定暫存器 (GPSET) ====================
// 寫入1到對應位元可將該GPIO設為高電位
// 寫入0則無作用（不會改變GPIO狀態）

#define GPSET0 0x1c     // GPIO輸出設定暫存器0 (控制GPIO 0-31)
                        // 每個位元對應一個GPIO：bit0=GPIO0, bit1=GPIO1, ...

#define GPSET1 0x20     // GPIO輸出設定暫存器1 (控制GPIO 32-53)
                        // bit0=GPIO32, bit1=GPIO33, ..., bit21=GPIO53

// ==================== GPIO輸出清除暫存器 (GPCLR) ====================
// 寫入1到對應位元可將該GPIO設為低電位
// 寫入0則無作用（不會改變GPIO狀態）

#define GPCLR0 0x28     // GPIO輸出清除暫存器0 (控制GPIO 0-31)
                        // 與GPSET0對應，用於設定低電位

#define GPCLR1 0x2c     // GPIO輸出清除暫存器1 (控制GPIO 32-53)
                        // 與GPSET1對應，用於設定低電位

// ==================== GPIO電位讀取暫存器 (GPLEV) ====================
// 讀取GPIO的即時輸入/輸出狀態
// 1表示高電位，0表示低電位（TCRT5000感測器主要使用此暫存器）

#define GPLEV0 0x34     // GPIO電位讀取暫存器0 (讀取GPIO 0-31狀態)
                        // TCRT5000感測器(GPIO16,17,18)的狀態就從此暫存器讀取

#define GPLEV1 0x38     // GPIO電位讀取暫存器1 (讀取GPIO 32-53狀態)

// ==================== GPIO事件偵測狀態暫存器 (GPEDS) ====================
// 記錄GPIO是否有偵測到指定的事件（上升緣、下降緣等）
// 讀取後會自動清除，用於中斷處理

#define GPEDS0 0x40     // GPIO事件偵測狀態暫存器0 (GPIO 0-31)

#define GPEDS1 0x44     // GPIO事件偵測狀態暫存器1 (GPIO 32-53)

// ==================== GPIO上升緣偵測致能暫存器 (GPREN) ====================
// 設定哪些GPIO要偵測上升緣事件（0→1轉換）
// 用於中斷觸發，可實作感測器狀態變化的即時回應

#define GPREN0 0x4c     // GPIO上升緣偵測致能暫存器0 (GPIO 0-31)

#define GPREN1 0x50     // GPIO上升緣偵測致能暫存器1 (GPIO 32-53)

// ==================== GPIO下降緣偵測致能暫存器 (GPFEN) ====================
// 設定哪些GPIO要偵測下降緣事件（1→0轉換）

#define GPFEN0 0x58     // GPIO下降緣偵測致能暫存器0 (GPIO 0-31)

#define GPFEN1 0x5C     // GPIO下降緣偵測致能暫存器1 (GPIO 32-53)

// ==================== GPIO高電位偵測致能暫存器 (GPHEN) ====================
// 設定哪些GPIO要偵測高電位事件（持續為1的狀態）

#define GPHEN0 0x64     // GPIO高電位偵測致能暫存器0 (GPIO 0-31)

#define GPHEN1 0x68     // GPIO高電位偵測致能暫存器1 (GPIO 32-53)

// ==================== GPIO低電位偵測致能暫存器 (GPLEN) ====================
// 設定哪些GPIO要偵測低電位事件（持續為0的狀態）

#define GPLEN0 0x70     // GPIO低電位偵測致能暫存器0 (GPIO 0-31)

#define GPLEN1 0x74     // GPIO低電位偵測致能暫存器1 (GPIO 32-53)

// ==================== GPIO非同步上升緣偵測致能暫存器 (GPAREN) ====================
// 非同步偵測上升緣，不受系統時脈影響
// 適用於高頻訊號或對時序要求嚴格的應用

#define GPAREN0 0x7c    // GPIO非同步上升緣偵測致能暫存器0 (GPIO 0-31)

#define GPAREN1 0x80    // GPIO非同步上升緣偵測致能暫存器1 (GPIO 32-53)

// ==================== GPIO非同步下降緣偵測致能暫存器 (GPAFEN) ====================
// 非同步偵測下降緣，不受系統時脈影響

#define GPAFEN0 0x88    // GPIO非同步下降緣偵測致能暫存器0 (GPIO 0-31)

#define GPAFEN1 0x8c    // GPIO非同步下降緣偵測致能暫存器1 (GPIO 32-53)

// ==================== GPIO上拉/下拉控制暫存器 (PUP_PDN_CNTRL) ====================
// BCM2711新增的上拉/下拉控制暫存器（取代舊版的GPPUD/GPPUDCLK）
// 每個GPIO使用2個位元：00=無, 01=上拉, 10=下拉, 11=保留

#define GPIO_PUP_PDN_CNTRL_REG0 0xe4    // GPIO上拉下拉控制暫存器0 (GPIO 0-15)
                                        // 每個GPIO佔2位元：GPIO0[1:0], GPIO1[3:2], ...

#define GPIO_PUP_PDN_CNTRL_REG1 0xe8    // GPIO上拉下拉控制暫存器1 (GPIO 16-31)
                                        // TCRT5000感測器的上拉下拉設定在此暫存器

#define GPIO_PUP_PDN_CNTRL_REG2 0xec    // GPIO上拉下拉控制暫存器2 (GPIO 32-47)

#define GPIO_PUP_PDN_CNTRL_REG3 0xf0    // GPIO上拉下拉控制暫存器3 (GPIO 48-57)

#endif // __GPIO_ADDRESS__

/*
 * ==================== 暫存器使用範例 ====================
 * 
 * 1. 設定GPIO為輸出模式：
 *    val = readl(gpio_base + GPFSEL1);           // 讀取GPFSEL1
 *    val |= (1 << 18);                          // 設定GPIO16為輸出(001)
 *    writel(val, gpio_base + GPFSEL1);          // 寫回暫存器
 * 
 * 2. 設定GPIO輸出高電位：
 *    writel(1 << 16, gpio_base + GPSET0);       // GPIO16輸出高電位
 * 
 * 3. 設定GPIO輸出低電位：
 *    writel(1 << 16, gpio_base + GPCLR0);       // GPIO16輸出低電位
 * 
 * 4. 讀取GPIO輸入狀態：
 *    val = readl(gpio_base + GPLEV0);           // 讀取狀態暫存器
 *    bit = (val >> 16) & 1;                     // 提取GPIO16的值
 * 
 * 5. 設定上拉電阻：
 *    val = readl(gpio_base + GPIO_PUP_PDN_CNTRL_REG1);
 *    val |= (1 << 0);                           // GPIO16設定上拉(01)
 *    writel(val, gpio_base + GPIO_PUP_PDN_CNTRL_REG1);
 */

/*
 * ==================== TCRT5000應用說明 ====================
 * 
 * TCRT5000循跡感測器使用的暫存器：
 * - GPIO16,17,18功能設定：GPFSEL1暫存器
 * - 感測器狀態讀取：GPLEV0暫存器
 * - 上拉下拉設定：GPIO_PUP_PDN_CNTRL_REG1暫存器
 * 
 * 感測器接線建議：
 * - VCC → 3.3V/5V（依感測器規格）
 * - GND → Ground
 * - OUT → GPIO16/17/18（數位輸出腳位）
 * 
 * 通常建議設定輕微上拉電阻以確保訊號穩定性
 */
