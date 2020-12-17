#include "CO2Sensor.h"
#include "mh_z19.h"
//#include "ATMEGA_FreeRTOS.h"
#include <stdio.h>

void co2_task(void* pvParams) {
	getCO2FromSensor();
}

void  getCO2FromSensor() {

	shared_data_t* sd = (shared_data_t*) NULL;
	mh_z19_returnCode_t ret;
	uint16_t co2;

	while ((ret = mh_z19_takeMeassuring()) != MHZ19_OK) {
		printf("[!] CO2: Could not take measuring: %d\n", ret);
		//vTaskDelay(50);
	}

	//vTaskDelay(100);

	if ((ret = mh_z19_getCo2Ppm(&co2)) != MHZ19_OK) {
		printf("[!] CO2: Could not get PPM: %d\n", ret);
		//vTaskDelay(50);
	}

	sd_setCo2(sd, co2);
	printf("[<] CO2: Measurement completed: %d\n", (int)sd_getCo2(sd));

}



