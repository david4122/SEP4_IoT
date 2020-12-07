/*
* main.c
*
* Author : IoT Group
*
*/


/*Drivers, FreeRTOS, LoRaWAN definition ------------------*/
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <hal_defs.h>
#include <ihal.h>
#include <ATMEGA_FreeRTOS.h>
#include <FreeRTOSTraceDriver.h>
#include <lora_driver.h>
#include <stdio_driver.h>

#include <semphr.h>
#include <event_groups.h>
#include <stdio_driver.h>
#include <serial.h>

#include "humidity_temperature_task.h"
#include "co2_task.h"
#include "light_task.h"
#include "loraWANHandler.h"

#define configTOTAL_HEAP_SIZE 16384

/*Task definition ------------------------------------------*/
//Task definition is done in their own header files
//body is in corresponding classes
//Maybe we should also take this and put it on shared_data as 
//definition and body in the class




// define semaphore handle
SemaphoreHandle_t xTestSemaphore;
EventGroupHandle_t measure_EventGroup;
MessageBufferHandle_t xMessageBuffer;
const size_t xMessageBufferSizeBytes = 100;
EventBits_t uxBits;

void receiveAllData( void *pvParameters );

 
void receiveAllData( void *pvParameters ){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 500 ms
    xLastWakeTime = xTaskGetTickCount();
    size_t xBytesSent;

	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		// Initialize the xLastWakeTime variable with the current time.
		
		
			uxBits = xEventGroupWaitBits(measure_EventGroup, /* The event group being tested. */
	        BIT_TASK_CO2_READY | BIT_TASK_TEMP_HUMIDITY_READY |BIT_TASK_LIGHT_READY, /* The bits to wait for. */
	        pdTRUE, /* Bits will be cleared before return*/
	        pdTRUE, /* Wait for bits to be set */
	        pdMS_TO_TICKS(1000)); /* Maximum time to wait*/
	
	        if( ( uxBits & ( BIT_TASK_CO2_READY | BIT_TASK_TEMP_HUMIDITY_READY |BIT_TASK_LIGHT_READY ) ) == ( BIT_TASK_CO2_READY | BIT_TASK_TEMP_HUMIDITY_READY |BIT_TASK_LIGHT_READY) )
            {
               /* xEventGroupWaitBits() returned because all bits were set. */
	           printf("all data measured\n");
			  
			  
               /* Send the data package to the message buffer, blocking for a maximum of 100ms to
               wait for enough space to be available in the message buffer. */
               xBytesSent = xMessageBufferSend( xMessageBuffer,
                                     getDataPackage(),
                                     sizeof( getDataPackage() ),
                                     pdMS_TO_TICKS( 100 ) );

               if( xBytesSent = sizeof( shared_data_t* ) )
               {
                   /* The call to xMessageBufferSend() times out before there was enough
                   space in the buffer for the data to be written. */
				   printf("successfully send to buffer\n");
               }
			   
            }else{
               /* xEventGroupWaitBits() returned because xTicksToWait ticks passed
               without all bits becoming set. */
	           printf("not all data received\n");
            }
			
			
		
	}
}
/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

	xTaskCreate(
	getTempAndHumFromSensor_Task_inClass
	,  (const portCHAR *)"Get Temperature & Humidity"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	//CO2
	
	xTaskCreate(
	getCo2FromSensor_Task_inClass
	,  (const portCHAR *)"Get CO2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	//Light
	
	xTaskCreate(
	getLightFromSensor_Task_inClass
	,  (const portCHAR *)"Get Visible Raw"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	//Event Group Task (mb consider we put the definition, task body in shared_data?)
	
	xTaskCreate(
	receiveAllData
	,  (const portCHAR *)"Get Visible Raw"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
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
	
	/* Create Event Groups */
	measure_EventGroup= xEventGroupCreate();
	temp_humid_create(measure_EventGroup);
	CO2_create(measure_EventGroup);
	tsl2591_light_create(measure_EventGroup);
	
    /* Create a message buffer that can hold 100 bytes.  The memory used to hold
    both the message buffer structure and the data in the message buffer is
    allocated dynamically. */
    xMessageBuffer = xMessageBufferCreate( xMessageBufferSizeBytes );

    if( xMessageBuffer == NULL )
    {
        /* There was not enough heap memory space available to create the
        message buffer. */
    }
	else
	{
		printf("The message buffer was created successfully\n");
        /* The message buffer was created successfully and can now be used. */
    }

	
	// Initialise the HAL layer and use 5 for LED driver priority
	hal_create(5);
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_create(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_create(3,xMessageBuffer);
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

