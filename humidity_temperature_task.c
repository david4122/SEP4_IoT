/*
 * temperature_task.c
 *
 * Created: 11/13/2020 12:55:59 PM
 *  Author: IoT
 */ 

#include "humidity_temperature_task.h"

#include<stdio.h>
#include <stdlib.h>

#include<hih8120.h>

#include "shared_data.h"


void temp_hum_task(void *pvParameters) {
	shared_data_t *sd = (shared_data_t*) pvParameters;

	puts("[*] TEMP HUM START\n");

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 900 / portTICK_PERIOD_MS; //90ms
	xLastWakeTime = xTaskGetTickCount();


	hih8120_driverReturnCode_t ret;
	while((ret = hih8120_create()) != HIH8120_OK){
		printf("[!] ERROR CREATING TEMPHUM SENSOR: %d\n", ret);
		vTaskDelay(50);
	}

	for(;;) {

		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		hih8120_driverReturnCode_t ret;
		if(HIH8120_OK != (ret = hih8120_wakeup()))
		{
			printf("[!] HUMTEMP: Error in wake up temp sensor! %d\n", ret);
			continue;
		}

		vTaskDelay(pdMS_TO_TICKS(250));

		if(HIH8120_OK != (ret = hih8120_measure()))
		{
			printf("[!] HUMTEMP: Could not perform measurement: %d\n", ret);
			continue;
		}

		vTaskDelay(pdMS_TO_TICKS(100));

		sd_setTemp(sd, hih8120_getTemperature());
		sd_setHumid(sd, hih8120_getHumidity());

		printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>%f\n", hih8120_getTemperature());

		//uxBits = xEventGroupSetBits(
		//sd->egroup,    /* The event group being updated. */
		//BIT_TASK_TEMP_HUMIDITY_READY);	/* The bits being set. */

		printf("[+] TEMPHUM: Measurement complete: Temp: %f, Hum: %f\n", sd_getTemp(sd), sd_getHumid(sd));
	}
}
