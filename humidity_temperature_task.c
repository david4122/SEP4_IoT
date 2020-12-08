/*
 * temperature_task.c
 *
 * Created: 11/13/2020 12:55:59 PM
 *  Author: IoT
 */ 


#include "humidity_temperature_task.h"

static uint16_t temperature;
static uint16_t humidity;
static EventGroupHandle_t event_group;
static EventBits_t uxBits;


void temp_humid_create(EventGroupHandle_t eg) {
	event_group = eg;
	
	if ( HIH8120_OK != hih8120_create())
	{
		printf("Temperature driver was failed to initialized. Result: %s\n",hih8120_create());
	}
}

void getTempAndHumFromSensor_Task_inClass(void *PvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 900/portTICK_PERIOD_MS; //90ms
	shared_data_t *p_sd = (shared_data_t*) PvParameters;
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
		temperature = (uint16_t)hih8120_getTemperature();
		humidity = (uint16_t)hih8120_getHumidity();
		sd_setTemp(p_sd,temperature);
		sd_setHumid(p_sd,humidity);
		printf("Temperature: %d.%d \n",temperature,humidity);

}
}