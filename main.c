/*
* main.c
* Created: 04/11/2020 8:22:59 AM
* Author: IoT Group
*/

/*Drivers, FreeRTOS, LoRaWAN definition ------------------*/
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <hal_defs.h>
#include <ihal.h>
#include <ATMEGA_FreeRTOS.h>
#include <FreeRTOSTraceDriver.h>

#include <semphr.h>
#include <event_groups.h>
#include <stdio_driver.h>
#include <serial.h>

#include "temperature_task.h"

#include <lora_driver.h>


/*Task definition ------------------------------------------------------------------*/
void getTemperature( void *pvParameters );

void task2( void *pvParameters );

/*Handles Definition ---------------------------------------------------------------*/
SemaphoreHandle_t xTestSemaphore;
EventGroupHandle_t measurement_event_group;

void lora_handler_create(UBaseType_t lora_handler_task_priority);

/*----------------------------------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	/* Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	* because it is sharing a resource, such as the Serial port.
	* Semaphores should only be used whilst the scheduler is running, but we can set it up here.*/
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex(); 
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
	
	if (measurement_event_group == NULL) //Check to confirm that it is not already created.
	{
		measurement_event_group = xEventGroupCreate();
		if ((measurement_event_group) != NULL)
		{
			xEventGroupClearBits(); //Clearing bits to make sure there's no dump in it
		}
	}

	xTaskCreate(
	getTemperature
	,  (const portCHAR *)"Get Temperature"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

/*-----------------------------------------------------------*/
void getTemperature( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 5000/portTICK_PERIOD_MS; // 500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Calling getTemperature method.\n"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		getTemperatureFromSensor();
	}
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);
	// Initialise the trace-driver to be used together with the R2R-Network
	trace_init();
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_create(ser_USART0);
	// Let's create some tasks
	create_tasks_and_semaphores();
	
	if ( HIH8120_OK != hih8120_create())
	{
		printf("Temperature driver was failed to initialized. Result: %s\n",hih8120_create());
	}

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Initialise the HAL layer and use 5 for LED driver priority
	hal_create(5);
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_create(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_create(3);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}

