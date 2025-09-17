// irs90_mqtt_publisher.h
#ifndef __IRS90_MQTT_PUBLISHER_H__
#define __IRS90_MQTT_PUBLISHER_H__

#include "mqtt_config.h"

// IRS-90 sensor device path
#define SENSOR_DEVICE "/dev/irs90_all"

// 讀取 IRS-90 資料並發布到MQTT
int irs90_mqtt_run(void);

#endif
