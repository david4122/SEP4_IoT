/*
 * smart_green_smart_house_iot.c
 * Created: 04/11/2020 8:22:59 AM
 * Author : Matey Matev //ADD YOUR NAMES HERE.
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <ihal.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>

//Drivers -------------------------------------------------------------------------------
#include <stdio_driver.h>
#include <serial.h>
#include <FreeRTOSTraceDriver.h>
#include <hih8120.h> //Need to be init();
#include <rc_servo.h> //Need to be init();
#include <lora_driver.h> //Need to be init();
#include <semphr.h>
#include <message_buffer.h>
#include <event_groups.h>
#include <temperature_task.h>


//Task Definition -------------------------------------------------------------------
void init_task(void * param); //CREATE();
void temp_sensor(void * param);
void window_controller(void * param);
void down_link_handler(void * param);
void up_link_handler(void * param);
void smart_green_smart_house(void * param);

//Define Semaphores/Mutexes/EventGroups/MessageBuffers-------------------------------------------------------
//SemaphoreHandle_t v_mutex;
MessageBufferHandle_t down_link_message_buffer; //  those needs to be created
MessageBufferHandle_t up_link_message_buffer;
EventGroupHandle_t event_group;

//LoRaWAN Handler---------------------------------------------------------------------
void lora_handler_create(UBaseType_t lora_handler_task_priority);
//yes
/*-----------------------------------------------------------------------------------------------------------*/

int main(void)
{
	puts("Program starting...");
	TaskHandle_t xHandle = NULL;
	/* Create the task, storing the handle. */
	xTaskCreate(
					init_task,       /* Function that implements the task. */
					"Initializing the system",          /* Text name for the task. */
					configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					(void*) 1,    /* Parameter passed into the task. */
					tskIDLE_PRIORITY,/* Priority at which the task is created. */
					&xHandle );      /* Used to pass out the created task's handle. */

	vTaskStartScheduler();
	
    /* Replace with your application code */
    while (1) {}
}

void init_task(void* param) {
	#if (configUSE_APPLICATION_TASK_TAG == 1)
	// Set task no to be used for tracing with R2R-Network
	vTaskSetApplicationTaskTag(NULL, (void*) 1 );
	#endif
		
	trace_init();
	stdio_create(ser_USART0);
	lora_driver_create(1, NULL);
	lora_handler_create(3);
	
	hih8120_create();
	if ( HIH8120_OK == hih8120_create() )
	{
		puts("Temperature Driver Successfully Created.")
	}
	printf("Temperature Task Driver Return Code: %d\n", hih8120_create());
	
	
	xTaskCreate(
					get_temp(),       /* Function that implements the task. */
					"getting temp",          /* Text name for the task. */
					configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					(void*) 1,    /* Parameter passed into the task. */
					tskIDLE_PRIORITY,/* Priority at which the task is created. */
					&xHandle );      /* Used to pass out the created task's handle. */
	
	// TODO init all sensors
	
	// TODO start all tasks
	
	
	
	vTaskDelete(NULL);
}
