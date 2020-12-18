/*
 * servo_task.c
 *
 * Created: 12/15/2020 10:41:11 PM
 *  Author: IoT
 */ 
#include "configuration_defines.h"

#include "servo_task.h"

#include <stdlib.h>
#include <stdio.h>

#include <ihal.h>
#include <rc_servo.h>
#include <lora_driver.h>

#include "shared_data.h"

void servo_init(void) {
	rc_servo_create(); //taken from detailed description
}

void servo_task(void *pvParams) {
	shared_data_t* sd = (shared_data_t*) pvParams;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = (SERVO_INTERVAL);
	xLastWakeTime = xTaskGetTickCount();

	xEventGroupSetBits(sd_getEgroup(sd), SERVO_READY_BIT);
	EventBits_t bits;
	while((bits = xEventGroupWaitBits(sd_getEgroup(sd),
					SYSTEM_READY, pdFALSE, pdTRUE, portMAX_DELAY)) != SYSTEM_READY);

	puts("[*] SERVO: Task started");

	lora_driver_payload_t payload;
	while(1) {

		xMessageBufferReceive(sd_getMessageBuffer(sd), &payload, sizeof(payload), portMAX_DELAY);

		if(payload.len == 1) {
			if(payload.bytes[0]) {
				rc_servo_setPosition(SERVO_PORT, 100);
			} else {
				rc_servo_setPosition(SERVO_PORT, -100);
			}
		}	
	}
}
