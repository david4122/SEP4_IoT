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

<<<<<<< HEAD
void sd_setTemp(shared_data_t* self, uint16_t t) {self->temp = t;}
void sd_setCo2(shared_data_t* self, uint16_t c) {self->co2= c;}
void sd_setHumid(shared_data_t* self, uint16_t h) {self->humid = h;}
void sd_setLight(shared_data_t* self, uint16_t l) {self->light = l;}

uint16_t sd_getTemp(shared_data_t* self) {return self->temp;}
uint16_t sd_getCo2(shared_data_t* self) {return self->co2;}
uint16_t sd_getHumid(shared_data_t* self) {return self->humid;}
uint16_t sd_getLight(shared_data_t* self) {return self->light;}
=======
void sd_setCo2(shared_data_t* self, uint16_t c) {self->co2= c;}

uint16_t sd_getCo2(shared_data_t* self) {return self->co2;}

=======
	uint16_t temp;
	uint16_t humid;
};
>>>>>>> master

void sd_setTemp(shared_data_t* self, uint16_t t) {self->temp = t;}
void sd_setHumid(shared_data_t* self, uint16_t h) {self->humid = h;}

uint16_t sd_getTemp(shared_data_t* self) {return self->temp;}
uint16_t sd_getHumid(shared_data_t* self) {return self->humid;}
