/*
 * CO2Sensor.h
 *
 * Created: 2020/11/20 12:46:27
 *  Author: Taiquan Sui
 */ 

#pragma once

#include <mh_z19.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdint.h>
#include "event_groups.h"
#include "shared_data.h"

#define BIT_TASK_CO2_READY (1 << 1)	// TODO move that into config file, has to be visible in other tasks for sync purposes

void CO2_create(EventGroupHandle_t eg);
uint16_t getCO2FromSensor();
void getCo2FromSensor_Task_inClass(void *pvParameters);

