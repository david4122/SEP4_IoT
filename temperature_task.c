/*
 * temperature_task.c
 *
 * Created: 11/13/2020 12:55:59 PM
 *  Author: Matey Matev
 */ 
#include "shared_data.h"
#include "temperature_task.h"
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>

void temperature_task(void *pvParameters)
{
	uint16_t temperature = 0;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 900/portTICK_PERIOD_MS; //90ms
	shared_data_t *p_sd = (shared_data_t*) pvParameters 
	
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;) {
		
				vTaskDelayUntil(&xLastWakeTime,xFrequency);
	
				if(HIH8120_OK != hih8120_wakeup())
				{
					printf("Error in wake up temp sensor! %s\n", hih8120_wakeup());
				}
				vTaskDelay(pdMS_TO_TICKS(250));
	
				if(HIH8120_OK != hih8120_measure())
				{
					printf("Error in measure temp sensor! %s\n", hih8120_measure());
				}
				vTaskDelay(pdMS_TO_TICKS(100));	
				temperature = hih8120_getTemperature_x10();
				int dec = temperature%10;
				temperature = temperature/10;
				sd_setTemp(temperature);
				printf("Temperature: %d.%d \n",temperature,dec);
	}
}