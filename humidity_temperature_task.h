/*
 * temperature_task.h
 *
 * Created: 11/13/2020 12:56:59 PM
 *  Author: Matey Matev
 */ 
#pragma once

#include <stdint.h>
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include "event_groups.h"
#include "shared_data.h"

#define BIT_TASK_TEMP_HUMIDITY_READY (1 << 0)

void temp_humid_create(EventGroupHandle_t eg);
void getTempAndHumFromSensor_Task_inClass(void *PvParameters);

