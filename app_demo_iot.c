/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#include "sensor.h"
/* attribute initiative to report */
#define TAKE_THE_INITIATIVE_TO_REPORT
#define ONE_SECOND (1000)
/* oc request id */
#define CN_COMMADN_INDEX "commands/request_id="
#define CONTROL_NODE_1 "ControlNodeONE"
#define CONTROL_NODE_2 "ControlNodeTWO"
#define CONTROL_NODE_STATE_1 "1"
#define CONTROL_NODE_STATE_2 "2"
int uart_SENSOR1_state = -1;
int uart_SENSOR2_state = -1;
int g_ligthStatus = -1;
typedef void (*FnMsgCallBack)(hi_gpio_value val);

typedef struct FunctionCallback
{
    hi_bool stop;
    hi_u32 conLost;
    hi_u32 queueID;
    hi_u32 iotTaskID;
    FnMsgCallBack msgCallBack;
} FunctionCallback;
FunctionCallback g_functinoCallback;

/* CPU Sleep time Set */
unsigned int TaskMsleepa(unsigned int ms)
{
    if (ms <= 0)
    {
        return HI_ERR_FAILURE;
    }
    return hi_sleep((hi_u32)ms);
}


// < this is the callback function, set to the mqtt, and if any messages come, it will be called
// < The payload here is the json string


static void DemoMsgRcvCallBack(int qos, const char *topic, const char *payload)
{
    IOT_LOG_DEBUG("RCVMSG:QOS:%d TOPIC:%s PAYLOAD:%s\r\n", qos, topic, payload);
    /* 云端下发命令后，板端的操作处理 */

    IoTProfileService_t service;
    IoTProfileKV_t property;
    printf("%s\n", strstr(payload, CONTROL_NODE_2));
    if (strstr(payload, WECHAT_SUBSCRIBE_LIGHT) != NULL)
    {
        if (strstr(payload, WECHAT_SUBSCRIBE_LIGHT_OFF_STATE) != NULL)
        {
            wechatControlDeviceMsg(HI_GPIO_VALUE0);
            g_ligthStatus = HI_FALSE;
            ssd1306_printf("IN LOOP\n");
            memset_s(&property, sizeof(property), 0, sizeof(property));
            IotlightPulish();
        }
        else
        {
            wechatControlDeviceMsg(HI_GPIO_VALUE1);
            g_ligthStatus = HI_TRUE;
            ssd1306_printf("IN LOOP\n");
            IotlightPulish();
        }
    }
    else if (strstr(payload, CONTROL_NODE_1) != NULL)
    {
        if (strstr(payload, CONTROL_NODE_STATE_1) != NULL)
        {
            uart_SENSOR1_state = 1;
            // Iot_SENSOR1_MODE_1_ON();
        }
        else if (strstr(payload, CONTROL_NODE_STATE_2) != NULL)
        {
            printf("jinle\n");
            uart_SENSOR1_state = 2;
            // Iot_SENSOR1_MODE_2_ON();
        }
    }

    else if (strstr(payload, CONTROL_NODE_2) != NULL)
    {

        if (strstr(payload, CONTROL_NODE_STATE_1) != NULL)
        {
            uart_SENSOR2_state = 1;
            // printf("\nasdfdsfasdf%d\n",uart_SENSOR2_state);
            // Iot_SENSOR2_MODE_1_ON();
        }
        else if (strstr(payload, CONTROL_NODE_STATE_2) != NULL)
        {
            uart_SENSOR2_state = 2;
            // Iot_SENSOR2_MODE_2_ON();
        }
    }

    const char *requesID;
    char *tmp;
    IoTCmdResp_t resp;
    tmp = strstr(topic, CN_COMMADN_INDEX);
    if (tmp != NULL)
    {
        // /< now you could deal your own works here --THE COMMAND FROM THE PLATFORM
        // /< now er roport the command execute result to the platform
        requesID = tmp + strlen(CN_COMMADN_INDEX);
        resp.requestID = requesID;
        resp.respName = NULL;
        resp.retCode = 0; ////< which means 0 success and others failed
        resp.paras = NULL;
        (void)IoTProfileCmdResp(CONFIG_USER_ID, &resp);
    }

    return;
}

static hi_void *DemoEntry(const char *arg)
{
    WifiStaReadyWait();
    cJsonInit();
    Uart1_init();
    oled_init();
    IoTMain();
    ssd1306_printf("iot init success!\n");
    /* 云端下发回调 */
    IoTSetMsgCallback(DemoMsgRcvCallBack);
    /* 主动上报 */

#ifdef TAKE_THE_INITIATIVE_TO_REPORT
    while (1)
    {

        /* 用户可以在这调用发布函数进行发布，需要用户自己写调用函数 */

        printf("%d    %d\n", uart_SENSOR1_state, uart_SENSOR2_state);
        sensor_control(uart_SENSOR1_state, uart_SENSOR2_state);
        uart_SENSOR1_state = -1;
        uart_SENSOR2_state = -1;
        TaskMsleepa(ONE_SECOND);
    }
#endif
    return NULL;
}

// < This is the demo entry, we create a task here,
// and all the works has been done in the demo_entry
#define CN_IOT_TASK_STACKSIZE 0x1000
#define CN_IOT_TASK_PRIOR 25
#define CN_IOT_TASK_NAME "IOTDEMO"

static void AppDemoIot(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "IOTDEMO";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = CN_IOT_TASK_PRIOR;

    if (osThreadNew((osThreadFunc_t)DemoEntry, NULL, &attr) == NULL)
    {
        printf("[mqtt] Falied to create IOTDEMO!\n");
    }
}

SYS_RUN(AppDemoIot);
