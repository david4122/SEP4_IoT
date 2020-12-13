/*
 * shared_data.c
 *
 * Created: 11/22/2020 6:12:15 PM
 *  Author: IoT Group
 */

#include "shared_data.h"

#include <stdlib.h>
#include <stdio.h>

#include<ATMEGA_FreeRTOS.h>

#include<semphr.h>
#include<event_groups.h>

struct shared_data {
	float temp;
	float co2;
	float humid;
	float light;

	SemaphoreHandle_t lock;		// Thread-safety
	EventGroupHandle_t egroup;	// For synchronization between tasks
};
//Pointer is defined in header file, by including the header file you have access to it.


shared_data_t* sd_create() {
	shared_data_t* sd = malloc(sizeof(shared_data_t));
	sd->co2 = 0.0;
	sd->humid = 0.0;
	sd->temp = 0.0;
	sd->light = 0.0;

	sd->lock = xSemaphoreCreateMutex();
	sd->egroup = xEventGroupCreate();

	if(sd->lock == NULL || sd->egroup == NULL) {
		puts("[!] Could not initialize shared_data");
		return NULL;
	}

	return sd;
}


EventGroupHandle_t sd_getEgroup(shared_data_t* self) {
	return self->egroup;
}


void sd_setTemp(shared_data_t* self, float t) {
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		self->temp = t;

		xSemaphoreGive(self->lock);
	}
}

void sd_setCo2(shared_data_t* self, float c) {
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		self->co2= c;

		xSemaphoreGive(self->lock);
	}
}

void sd_setHumid(shared_data_t* self, float h) {
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		self->humid = h;

		xSemaphoreGive(self->lock);
	}
}

void sd_setLight(shared_data_t* self, float l) {
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		self->light = l;

		xSemaphoreGive(self->lock);
	}
}

float sd_getTemp(shared_data_t* self) {
	float res = 0.0;
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		res = self->temp;

		xSemaphoreGive(self->lock);
	}

	return res;
}

float sd_getCo2(shared_data_t* self) {
	float res = 0.0;
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		res = self->co2;

		xSemaphoreGive(self->lock);
	}
	return res;
}

float sd_getHumid(shared_data_t* self) {
	float res = 0.0;
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		res = self->humid;

		xSemaphoreGive(self->lock);
	}
	return res;
}

float sd_getLight(shared_data_t* self) {
	float res = 0.0;
	if(xSemaphoreTake(self->lock, (TickType_t) 10) == pdTRUE) {
		res = self->light;

		xSemaphoreGive(self->lock);
	}
	return res;
}
