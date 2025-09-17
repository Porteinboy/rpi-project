// led_hal.h
#ifndef LED_HAL_H
#define LED_HAL_H

#include <linux/kernel.h>

// LED 狀態定義
#define LED_OFF 0
#define LED_ON  1

// LED 編號定義
#define LED_A   0
#define LED_B   1
#define LED_C   2
#define LED_COUNT 3

/**
 * @brief 初始化所有LED硬體
 * @return 0 成功，負值表示錯誤
 */
int led_hal_init(void);

/**
 * @brief 清理LED硬體資源
 */
void led_hal_cleanup(void);

/**
 * @brief 設置單顆LED狀態
 * @param led_num LED編號 (0=A, 1=B, 2=C)
 * @param state LED狀態 (0=關閉, 1=開啟)
 * @return 0 成功，負值表示錯誤
 */
int led_hal_set_led(int led_num, int state);

/**
 * @brief 獲取單顆LED狀態
 * @param led_num LED編號 (0=A, 1=B, 2=C)
 * @return LED狀態 (0=關閉, 1=開啟)，負值表示錯誤
 */
int led_hal_get_led(int led_num);

/**
 * @brief 設置所有LED狀態
 * @param states 三位數字字串，如 "101" 表示 A開B關C開
 * @return 0 成功，負值表示錯誤
 */
int led_hal_set_all_leds(const char *states);

/**
 * @brief 獲取所有LED狀態
 * @param buffer 輸出緩衝區，至少4字節 (包含結束符)
 * @return 0 成功，負值表示錯誤
 */
int led_hal_get_all_leds(char *buffer);

/**
 * @brief 關閉所有LED
 */
void led_hal_turn_off_all(void);

/**
 * @brief 開啟所有LED
 */
void led_hal_turn_on_all(void);

#endif // LED_HAL_H
