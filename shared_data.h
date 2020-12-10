/*
 * shared_data.h
 *
 * Created: 11/22/2020 6:19:08 PM
 *  Author: IoT Group
 */ 
#pragma once


#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>

typedef struct shared_data shared_data_t;

shared_data_t* sd_create();

EventGroupHandle_t get_event_group(shared_data_t* self);

float sd_getTemp(shared_data_t* self);
float sd_getCo2(shared_data_t* self);
float sd_getHumid(shared_data_t* self);
float sd_getLight(shared_data_t* self);

void sd_setTemp(shared_data_t* self, float t);
void sd_setCo2(shared_data_t* self, float c);
void sd_setHumid(shared_data_t* self, float h);
void sd_setLight(shared_data_t* self, float l);
