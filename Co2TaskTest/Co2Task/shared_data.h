
#pragma once



typedef struct shared_data shared_data_t;

shared_data_t* sd_create(void);

float sd_getCo2(shared_data_t* self);

void sd_setCo2(shared_data_t* self, float c);