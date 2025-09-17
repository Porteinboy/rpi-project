// irs90_hal.h
#ifndef IRS90_HAL_H
#define IRS90_HAL_H

// IRS-90 感測器 ID 定義
typedef enum {
    IRS_90_A = 0,
    IRS_90_B = 1,
    IRS_90_C = 2,
    IRS_90_MAX
} irs_90_sensor_t;

// 感測器狀態定義
typedef enum {
    IRS_90_NO_OBJECT = 0,
    IRS_90_OBJECT_DETECT = 1
} irs_90_status_t;

// 函數原型定義
int irs_90_init(irs_90_sensor_t sensor);
int irs_90_read(irs_90_sensor_t sensor);
int irs_90_read_all(irs_90_status_t status_array[]);
int irs_90_cleanup(irs_90_sensor_t sensor);
int irs_90_cleanup_all(void);
int irs_90_get_gpio_pin(irs_90_sensor_t sensor);

#endif // IRS90_HAL_H

