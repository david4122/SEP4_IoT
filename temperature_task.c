/*
 * temperature_task.c
 *
 * Created: 11/13/2020 12:55:59 PM
 *  Author: Matey Matev
 */ 

#include "temperature_task.h"
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>

float getTemperatureFromSensor()
{
	uint16_t temperature = 0;
	
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
	printf("Temperature: %.2f",temperature);
	return temperature;
}