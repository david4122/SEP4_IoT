
#include "light_task.h"

#include "configuration_defines.h"

#include <stdlib.h>
#include <stdio.h>

#include <tsl2591.h>

#include "shared_data.h"

void light_init(void) {
	tsl2591_create(NULL);

	tsl2591_returnCode_t ret;
	for(int i = 0; i < (RETRIES); i++) {
		if((ret = tsl2591_create(NULL)) == TSL2591_OK)
			break;
		printf("[!] LIGHT: Failed to initialize driver: %d, attempt %d\n", ret, i);
	}

	for(int i = 0; i < (RETRIES); i++) {
		if((ret = tsl2591_enable()) == TSL2591_OK)
			break;
		printf("[!] LIGHT: Failed to enable driver: %d, attempt %d\n", ret, i);
	}

	puts("[*] LIGHT: Initialized");
}

void light_task(void* pvParams) {
	shared_data_t* sd = (shared_data_t*) pvParams;

	const TickType_t xFrequency = (LIGHT_INTERVAL);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	xEventGroupSetBits(sd_getEgroup(sd), LIGHT_READY_BIT);
	EventBits_t bits;
	while((bits = xEventGroupWaitBits(sd_getEgroup(sd), SYSTEM_READY, pdFALSE, pdTRUE, portMAX_DELAY)) != SYSTEM_READY);

	puts("[*] LIGHT: Task started");

	tsl2591_returnCode_t ret;
	float res;
	while(1) {
		while((ret = tsl2591_fetchData()) != TSL2591_OK) {
			printf("[!] LIGHT: Failed to fetch data: %d\n", ret);
			vTaskDelay(50);
		}

		vTaskDelay(50);

		if((ret = tsl2591_getLux(&res))) {
			printf("[!] LIGHT: measuring is in overflow: %d\n", ret);
			continue;
		}

		sd_setLight(sd, res);
		printf("[<] LIGHT: Measurement completed: %d\n", (int) res);

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
