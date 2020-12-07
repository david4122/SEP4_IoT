/*
 * CO2Sensor.c
 *
 * Created: 2020/11/20 12:46:07
 *  Author: Taiquan Sui
 */ 

#include "co2_task.h"

static uint16_t ppm;
static mh_z19_returnCode_t rc;
static EventGroupHandle_t event_group;
static EventBits_t uxBits;

static void CO2Callback(uint16_t ppm){
	//Remove this when deployment.
	printf("the value of CO2 is: %d\n",ppm);

}

void CO2_create(EventGroupHandle_t eg){
	event_group = eg
	void (*callback)(uint16_t)= &CO2Callback; 
	
	// The first parameter is the USART port the MH-Z19 sensor is connected to - in this case USART3
	// The second parameter is the address of the call back function
	mh_z19_create(ser_USART3, callback);
}



uint16_t getCO2FromSensor(){
	
	rc = mh_z19_takeMeassuring();
	if (rc != MHZ19_OK)
	{
		printf("failed to take measure\n");
	}else{
		printf("ready to take measure\n");
	}
	
    vTaskDelay(pdMS_TO_TICKS(100));
	
	rc = mh_z19_getCo2Ppm(&ppm);
	if(rc != MHZ19_OK){
		printf("failed to get CO2\n");
	}
	
	printf("CO2: %d\n",ppm);
	return ppm;
}

//I have done it like that cause I have no idea which one will work.
//Just to be prepared.

void getCo2FromSensor_Task_inClass(void *pvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 900/portTICK_PERIOD_MS;
	shared_data_t *p_sd = (shared_data_t*) pvParameters;
	xLastWakeTime = xTaskGetTickCount();
	for (;;) 
			{
				vTaskDelayUntil(&xLastWakeTime,xFrequency);
				rc = mh_z19_takeMeassuring();
				if (rc != MHZ19_OK)
				{
					printf("failed to take measure\n");
				}else{
					//printf("ready to take measure\n");
				}
	
				vTaskDelay(pdMS_TO_TICKS(100));
	
				rc = mh_z19_getCo2Ppm(&ppm);
				if(rc != MHZ19_OK){
					printf("failed to get CO2\n");
				}
				
				sd_setCo2(p_sd,&ppm);
				uxBits = xEventGroupSetBits(
				event_group,    /* The event group being updated. */
				BIT_TASK_CO2_READY );/* The bits being set. */
				
				
				if( ( uxBits & BIT_TASK_CO2_READY ) == BIT_TASK_CO2_READY )
				{
					//printf("co2 bit is set\n");
					/* Bit remained set when the function returned. */
				}
				else
				{
					/* .  It might be that a task was waiting for both of the bit to be set, and the bits were cleared
					as the task left the Blocked state. */
				}	
	}
}