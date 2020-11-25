/*
 * shared_data.h
 *
 * Created: 11/22/2020 6:19:08 PM
 *  Author: IoT Group
 */ 
#include <stdint.h>
#pragma once

typedef struct shared_data shared_data_t;

void sd_setTemp(shared_data_t* self, uint16_t t);
void sd_setCo2(shared_data_t* self, uint16_t c);
void sd_setHumid(shared_data_t* self, uint16_t h);
void sd_setLight(shared_data_t* self, uint16_t l);

uint16_t sd_getTemp(shared_data_t* self);
uint16_t sd_getCo2(shared_data_t* self);
uint16_t sd_setHumid(shared_data_t* self);
uint16_t sd_setLight(shared_data_t* self);