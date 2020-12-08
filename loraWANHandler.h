/*
 * loraWANHandler.h
 *
 * Created: 11/14/2020 12:07:13 AM
 *  Author: semin
 */ 
 #pragma once
 #include <stddef.h>
 #include <stdio.h>
 #include <ATMEGA_FreeRTOS.h>
 #include <lora_driver.h>
 #include <status_leds.h>
 #include "humidity_temperature_task.h"
 #include "co2_task.h"
 #include "light_task.h"
 #include "shared_data.h"
 
 #define LORA_appEUI "689DF9DF68156742"
 #define LORA_appKEY "B09F779D3DF66B89B996955E3B4ED977"

 static lora_driver_payload_t _uplink_payload;
 static void _lora_setup(void);
 void lora_handler_task( void *pvParameters );
 void lora_handler_create(UBaseType_t lora_handler_task_priority,MessageBufferHandle_t mb);