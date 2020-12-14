/*
 * temperature_task.h
 *
 * Created: 11/13/2020 12:56:59 PM
 *  Author: Matey Matev
 */ 
#pragma once

#define BIT_TASK_TEMP_HUMIDITY_READY (1 << 0) 	// TODO move that into config file, has to be visible in other tasks for sync purposes


void temp_hum_init(void);

void temp_hum_task(void *pvParameters);