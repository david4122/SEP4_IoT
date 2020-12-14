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
#include "configuration_defines.h"


void temp_hum_init(void) {
	hih8120_driverReturnCode_t ret;
	for(int i = 0; i < (RETRIES); i++) {
		if((ret = hih8120_create()) == HIH8120_OK) {
			puts("[*] TEMPHUM: Initialized");
			return;
		}
		printf("[!] TEMPHUM: Failed to initialize driver: %d, attempt %d\n", ret, i);
	}
}


void temp_hum_task(void *pvParameters) {
	shared_data_t *sd = (shared_data_t*) pvParameters;

	puts("[*] TEMPHUM: task started\n");

	const TickType_t xFrequency = (TEMPHUM_INTERVAL); //90ms
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for(;;) {

		hih8120_driverReturnCode_t ret;
		if(HIH8120_OK != (ret = hih8120_wakeup()))
		{
			printf("[!] TEMPHUM: Error in wake up temp sensor! %d\n", ret);
			continue;
		}

		vTaskDelay(pdMS_TO_TICKS(250));

		if(HIH8120_OK != (ret = hih8120_measure()))
		{
			printf("[!] TEMPHUM: Could not perform measurement: %d\n", ret);
			continue;
		}

		vTaskDelay(pdMS_TO_TICKS(100));

		sd_setTemp(sd, hih8120_getTemperature());
		sd_setHumid(sd, hih8120_getHumidity());

		//uxBits = xEventGroupSetBits(
		//sd->egroup,    /* The event group being updated. */
		//BIT_TASK_TEMP_HUMIDITY_READY);	/* The bits being set. */

		printf("[<] TEMPHUM: Measurement completed: t%d, h%d\n", (int) sd_getTemp(sd), (int) sd_getHumid(sd));

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
