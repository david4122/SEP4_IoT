/*
 * shared_data.c
 *
 * Created: 11/22/2020 6:12:15 PM
 *  Author: IoT Group
 */
 
#include "shared_data.h"

struct shared_data {
	uint16_t co2;
};

void sd_setCo2(shared_data_t* self, uint16_t c) {self->co2= c;}

uint16_t sd_getCo2(shared_data_t* self) {return self->co2;}


