/*
 * CO2Sensor.c
 *
 * Created: 2020/11/20 12:46:07
 *  Author: Taiquan Sui
 */ 

#include "CO2Sensor.h"
#include <mh_z19.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>

static uint16_t ppm;
static mh_z19_returnCode_t rc;

static void CO2Callback(uint16_t ppm){
	printf("the value of CO2 is: %d\n",ppm);
}

void CO2_create(){
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