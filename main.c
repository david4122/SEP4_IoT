/*
 * main.c
 *
 * Author : IoT Group
 *
 */


/*Drivers, FreeRTOS, LoRaWAN definition ------------------*/
#include <FreeRTOSConfig.h>

#include "configuration_defines.h"

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <event_groups.h>
#include <stdio_driver.h>
#include <serial.h>

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <hal_defs.h>
#include <ihal.h>
#include <lora_driver.h>
#include <stdio_driver.h>

#include "hih8120.h"
#include "shared_data.h"
#include "humidity_temperature_task.h"
#include "co2_task.h"
#include "light_task.h"
#include "loraWAN_task.h"

int freeMem() {
	int size = 8 * 1024;
	uint8_t *b;
	while(size > 0 && (b = malloc(--size)) == NULL);
	free(b);
	return size;
}

void init_task(void* pvParams)
{
	shared_data_t* sd = (shared_data_t*) pvParams;

	vTaskDelay(100);
	puts("[*] INIT STARTED");
	
	xTaskCreate( 
			temp_hum_task, 
			(const portCHAR *)"Get Temperature & Humidity", 
			configMINIMAL_STACK_SIZE, 
			sd, 
			tskIDLE_PRIORITY+1, 
			NULL); 


	xTaskCreate(
			co2_task
			,  (const portCHAR *)"Get CO2"
			,  configMINIMAL_STACK_SIZE
			,  sd
			,  tskIDLE_PRIORITY+1
			,  NULL );

	//Light
	xTaskCreate(
		light_task
	,  (const portCHAR *)"Get Visible Raw"
	,  configMINIMAL_STACK_SIZE
	,  sd
	,  tskIDLE_PRIORITY+1
	,  NULL );

	xTaskCreate(
			lora_task,
			(const portCHAR *)"LRHand",
			configMINIMAL_STACK_SIZE+200,
			sd,
			tskIDLE_PRIORITY+2,
			NULL);

	puts("[*] INIT FINISHED");

	while(1) {
		printf("[*] CURRENT DATA: temp: %d, hum %d, co2: %d, light: %d\n",
				(int) sd_getTemp(sd),
				(int) sd_getHumid(sd),
				(int) sd_getCo2(sd),
				(int) sd_getLight(sd));
		vTaskDelay((DIAG_INTERVAL));
	}
}


int main(void)
{
	// Set output ports for leds
	DDRA |= _BV(DDA0) | _BV(DDA7);
	// Serial IO - Setting 57600,8,N,1
	stdio_create(ser_USART0);

	// LoraWAN
	hal_create(5);
	lora_driver_create(1, NULL);

	shared_data_t* sd = sd_create();

	temp_hum_init();
	co2_init();
	light_init();
	
	lora_init();

	xTaskCreate(
			init_task,
			(const portCHAR *) "Initialise System",
			configMINIMAL_STACK_SIZE,
			sd,
			configMAX_PRIORITIES - 1,
			NULL);

	vTaskStartScheduler();

	while (1) {}
}
