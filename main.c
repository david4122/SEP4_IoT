/*
 * main.c
 *
 * Author : IoT Group
 *
 */


/*Drivers, FreeRTOS, LoRaWAN definition ------------------*/
#include <FreeRTOSConfig.h>

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

/*-----------------------------------------------------------*/
void init_task(void* pvParams)
{
	shared_data_t* sd = (shared_data_t*) pvParams;

	//lora_driver_resetRn2483(1);
	//vTaskDelay(2);
	//lora_driver_resetRn2483(0);
	//vTaskDelay(150);
	//lora_driver_flushBuffers(); 
	//vTaskDelay(150);
	//lora_setup();

	puts("[*] INIT STARTED");
	printf("[*] FREE MEM: %d FREERTOS FREE: %d\n", freeMem(), xPortGetFreeHeapSize());


	//xTaskCreate(
	//lora_handler_task,
	//(const portCHAR *)"LRHand",
	//configMINIMAL_STACK_SIZE+200,
	//sd,
	//tskIDLE_PRIORITY+2,
	//NULL);

	/* printf("SHARED DATA: %d\n", (int)sd); */

	xTaskCreate( 
			temp_hum_task, 
			(const portCHAR *)"Get Temperature & Humidity", 
			configMINIMAL_STACK_SIZE, 
			sd, 
			tskIDLE_PRIORITY+1, 
			NULL); 

	//CO2

	xTaskCreate(
			co2_task
			,  (const portCHAR *)"Get CO2"  // A name just for humans
			,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
			,  sd
			,  tskIDLE_PRIORITY+1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
			,  NULL );

	////Light
	//xTaskCreate(
	//getLightFromSensor_Task_inClass
	//,  (const portCHAR *)"Get Visible Raw"  // A name just for humans
	//,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	//,  sd
	//,  tskIDLE_PRIORITY+1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	//,  NULL );

	puts("[*] INIT FINISHED");

	while(1) {
		printf("[*] FREE MEM: %d FREERTOS FREE: %d\n", freeMem(), xPortGetFreeHeapSize());
		printf("[*] CURRENT DATA: temp: %d, hum %d, co2: %d, light: %d\n",
				(int) sd_getTemp(sd),
				(int) sd_getHumid(sd),
				(int) sd_getCo2(sd),
				(int) sd_getLight(sd));
		vTaskDelay(100);
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

	printf("[*] MAIN FREE MEM: %d\n", freeMem());

	shared_data_t* sd = sd_create();

	temp_hum_init();
	co2_init();

	xTaskCreate(
			init_task,
			(const portCHAR *) "Initialise System",
			configMINIMAL_STACK_SIZE,
			sd,
			3,
			NULL);

	vTaskStartScheduler();

	while (1) {}
}
