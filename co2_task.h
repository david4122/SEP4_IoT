/*
 * CO2Sensor.h
 *
 * Created: 2020/11/20 12:46:27
 *  Author: Taiquan Sui
 */ 

#pragma once

#define BIT_TASK_CO2_READY (1 << 1)	// TODO move that into config file, has to be visible in other tasks for sync purposes

void co2_init();

void co2_task(void *pvParameters);

