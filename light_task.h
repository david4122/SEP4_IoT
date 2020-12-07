/*
 * light_task.h
 *
 * Created: 11/20/2020 4:49:00 PM
 *  Author: mmate
 */ 


#pragma once
#include <stdint.h>
#include <tsl2591.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "shared_data.h"

#define BIT_TASK_LIGHT_READY (1 << 2)

void tsl2591_light_create(EventGroupHandle_t eg);
uint16_t getLightFromSensor();
void getLightFromSensor_Task_inClass(void *pvParameters);





