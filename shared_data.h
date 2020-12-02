/*
 * shared_data.h
 *
 * Created: 11/22/2020 6:19:08 PM
 *  Author: IoT Group
 */ 
#include <stdint.h>
#pragma once

typedef struct shared_data shared_data_t;

void sd_setCo2(shared_data_t* self, uint16_t c);

uint16_t sd_getCo2(shared_data_t* self);
=======
void sd_setTemp(shared_data_t* self, uint16_t t);
void sd_setHumid(shared_data_t* self, uint16_t h);

uint16_t sd_getTemp(shared_data_t* self);
<<<<<<< HEAD
uint16_t sd_getCo2(shared_data_t* self);
uint16_t sd_getHumid(shared_data_t* self);
uint16_t sd_getLight(shared_data_t* self);
=======
uint16_t sd_getHumid(shared_data_t* self);
>>>>>>> master
