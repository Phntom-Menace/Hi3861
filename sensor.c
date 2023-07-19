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
void Alarm(void)
{
    unsigned char uartReadBuff[3] = {0};
    IoTProfileService_t service;
    IoTProfileKV_t property;

    IoTUartRead(HI_UART_IDX_1, uartReadBuff, 1);
    if (uartReadBuff[0] == 0xFA || uartReadBuff[0] == 0xFB)
    {
        IoTUartRead(HI_UART_IDX_1, &uartReadBuff[1], 1);

        printf("Uart Read Data is:  %x %x \r\n", uartReadBuff[0], uartReadBuff[1]);
        if (uartReadBuff[0] == 0xFA && uartReadBuff[1] == 0xCC)
        {
            printf("warning1.");
            memset_s(&property, sizeof(property), 0, sizeof(property));
            property.type = EN_IOT_DATATYPE_INT;
            property.key = "Flag_Invision_1";
            property.iValue = 1;
            memset_s(&service, sizeof(service), 0, sizeof(service));
            service.serviceID = "Flag_Invision";
            service.serviceProperty = &property;
            IoTProfilePropertyReport(CONFIG_USER_ID, &service);
        }
        if (uartReadBuff[0] == 0xFB && uartReadBuff[1] == 0xCC)
        {
            printf("warning2.");
            memset_s(&property, sizeof(property), 0, sizeof(property));
            property.type = EN_IOT_DATATYPE_INT;
            property.key = "Flag_Invision_2";
            property.iValue = 1;
            memset_s(&service, sizeof(service), 0, sizeof(service));
            service.serviceID = "Flag_Invision";
            service.serviceProperty = &property;
            IoTProfilePropertyReport(CONFIG_USER_ID, &service);
        }
        if (uartReadBuff[0] == 0xFA && uartReadBuff[1] == 0xDD)
        {
            printf("raining1.");
            memset_s(&property, sizeof(property), 0, sizeof(property));
            property.type = EN_IOT_DATATYPE_INT;
            property.key = "Flag_Raining_1";
            property.iValue = 1;
            memset_s(&service, sizeof(service), 0, sizeof(service));
            service.serviceID = "Flag_Raining";
            service.serviceProperty = &property;
            IoTProfilePropertyReport(CONFIG_USER_ID, &service);
        }
        if (uartReadBuff[0] == 0xFB && uartReadBuff[1] == 0xDD)
        {
            printf("raining2.");
            memset_s(&property, sizeof(property), 0, sizeof(property));
            property.type = EN_IOT_DATATYPE_INT;
            property.key = "Flag_Raining_2";
            property.iValue = 1;
            memset_s(&service, sizeof(service), 0, sizeof(service));
            service.serviceID = "Flag_Raining";
            service.serviceProperty = &property;
            IoTProfilePropertyReport(CONFIG_USER_ID, &service);
        }
    }
    else
    {
        usleep(U_SLEEP_TIME);
        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_INT;
        property.key = "Flag_Invision_1";
        property.iValue = 0;
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "Flag_Invision";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_USER_ID, &service);

        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_INT;
        property.key = "Flag_Raining_1";
        property.iValue = 0;
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "Flag_Raining";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_USER_ID, &service);
        memset(uartReadBuff, 0, sizeof(uartReadBuff));

        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_INT;
        property.key = "Flag_Invision_2";
        property.iValue = 0;
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "Flag_Invision";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_USER_ID, &service);
        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_INT;
        property.key = "Flag_Raining_2";
        property.iValue = 0;
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "Flag_Raining";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_USER_ID, &service);
    }
}
void UART_SENSOR_MODE(int sensor_id, int mode)
{
    IoTProfileService_t service;
    IoTProfileKV_t property;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "ControlNode";
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    unsigned char uartWriteBuff[2] = {0};
    unsigned char uartReadBuff[3] = {0};
    if (sensor_id == Sensor_1)
    {
        uartWriteBuff[0] = 0xFA;
        property.key = "ControlNodeONE";
    }

    else if (sensor_id == Sensor_2)
    {
        uartWriteBuff[0] = 0xFB;
        property.key = "ControlNodeTWO";
    }

    if (mode == Mode_1)
    {
        uartWriteBuff[1] = 0xAB;
    }

    else if (mode == Mode_2)
    {
        uartWriteBuff[1] = 0xAC;
    }

    printf(" %d  %x  %x", mode, uartWriteBuff[0], uartWriteBuff[1]);
    while (1)
    {
        IoTUartWrite(HI_UART_IDX_1, uartWriteBuff, sizeof(uartWriteBuff));
        IoTUartRead(HI_UART_IDX_1, uartReadBuff, 1);
        if (uartReadBuff[0] == 0xFA || uartReadBuff[0] == 0xFB)
        {
            IoTUartRead(HI_UART_IDX_1, &uartReadBuff[1], 1);
            printf("Uart Read Data is:  %x %x \r\n", uartReadBuff[0], uartReadBuff[1]);
            break;
        }
        // iotpub
        usleep(U_SLEEP_TIME);
    }
    if (sensor_id == Sensor_1)
    {
        if (mode == Mode_1)
        {
            if (uartReadBuff[1] == Open_state)
                property.value = "Iot_SENSOR1_MODE_1_ON";
            else if (uartReadBuff[1] == Close_state)
                property.value = "Iot_SENSOR1_MODE_1_OFF";
        }
        else if (mode == Mode_2)
        {
            if (uartReadBuff[1] == Open_state)
                property.value = "Iot_SENSOR1_MODE_2_ON";
            else if (uartReadBuff[1] == Close_state)
                property.value = "Iot_SENSOR1_MODE_2_OFF";
        }
    }
    else if (sensor_id == Sensor_2)
    {
        if (mode == Mode_1)
        {
            if (uartReadBuff[1] == Open_state)
                property.value = "Iot_SENSOR2_MODE_1_ON";
            else if (uartReadBuff[1] == Close_state)
                property.value = "Iot_SENSOR2_MODE_1_OFF";
        }
        else if (mode == Mode_2)
        {
            if (uartReadBuff[1] == Open_state)
                property.value = "Iot_SENSOR2_MODE_2_ON";
            else if (uartReadBuff[1] == Close_state)
                property.value = "Iot_SENSOR2_MODE_2_OFF";
        }
    }
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_USER_ID, &service);
    memset(uartReadBuff, 0, sizeof(uartReadBuff));
}

void sensor_control(int sensor_1_state, int sensor_2_state)
{
    if (sensor_1_state == -1 && sensor_2_state == -1)
    {
        Alarm();
        // IOTpub
    }
    if (sensor_1_state != -1 && sensor_2_state == -1)
        UART_SENSOR_MODE(Sensor_1, sensor_1_state);

    if (sensor_2_state != -1 && sensor_1_state == -1)
        UART_SENSOR_MODE(Sensor_2, sensor_2_state);
    if (sensor_2_state != -1 && sensor_1_state != -1)
    {
        UART_SENSOR_MODE(Sensor_1, sensor_1_state);
        usleep(U_SLEEP_TIME);
        UART_SENSOR_MODE(Sensor_2, sensor_2_state);
        usleep(U_SLEEP_TIME);
    }
}