#ifndef SENSOR_H
#define sensor_h
#include <hi_task.h>
#include <string.h>
#include <hi_wifi_api.h>
#include <hi_mux.h>

#include <hi_io.h>
#include <hi_gpio.h>
#include "iot_config.h"
#include "iot_log.h"
#include "iot_main.h"
#include "iot_profile.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include <unistd.h>
#include "hi_timer.h"
#include "hi_time.h"
#include "iot_watchdog.h"
#include "iot_errno.h"
#include "hi_errno.h"
#include "iot_gpio.h"
#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include <unistd.h>
#include <stdint.h>
#include "hi_uart.h"
#include "uart.h"

#define TAKE_THE_INITIATIVE_TO_REPORT
#define ONE_SECOND (1000)
/* oc request id */
#define CN_COMMADN_INDEX "commands/request_id="
#define WECHAT_SUBSCRIBE_LIGHT "light"
#define WECHAT_SUBSCRIBE_LIGHT_ON_STATE "1"
#define WECHAT_SUBSCRIBE_LIGHT_OFF_STATE "0"
#define CONTROL_NODE_1 "ControlNodeONE"
#define CONTROL_NODE_2 "ControlNodeTWO"
#define CONTROL_NODE_STATE_1 "1"
#define CONTROL_NODE_STATE_2 "2"

void AlarmONE(void);
void UART_SENSOR1_MODE_1(void);
void Iot_SENSOR1_MODE_1_ON(void);
void Iot_SENSOR1_MODE_1_OFF(void);
void UART_SENSOR1_MODE_2(void);
void Iot_SENSOR1_MODE_2_ON(void);
void Iot_SENSOR1_MODE_2_OFF(void);
void AlarmTWO(void);
void UART_SENSOR2_MODE_1(void);
void Iot_SENSOR2_MODE_1_ON(void);
void Iot_SENSOR2_MODE_1_OFF(void);
void UART_SENSOR2_MODE_2(void);
void Iot_SENSOR2_MODE_2_ON(void);
void Iot_SENSOR2_MODE_2_OFF(void);

#define Sensor_1 1
#define Sensor_2 2
#define Mode_1 1
#define Mode_2 2
#define Open_state 0x77
#define Close_state 0x66
void Alarm(void);
void sensor_control(int sensor_1_state, int sensor_2_state);
void UART_SENSOR_MODE(int sensor_id, int mode);

#endif