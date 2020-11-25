/*
 * humidity_task.c
 *
 * Created: 2020/11/20 14:15:57
 *  Author: Zhang
 */ 
#include "humidity_task.h"
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>

float get_humidityFromSensor(){
	float humidity = 0;
	// print out for testing
	uint16_t humidityToPrint = 0;
	
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
	humidity = hih8120_getHumidity();
	humidityToPrint = hih8120_getHumidityPercent_x10();
	int inte = humidityToPrint/10;
	int decm = humidityToPrint%10; 
	printf("Humidity is : %d.%d",inte,decm);
	
	return humidity;
	
}