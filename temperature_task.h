/*
 * temperature_task.h
 *
 * Created: 11/13/2020 12:56:59 PM
 *  Author: mmate
 */ 


#ifndef TEMPERATURE_TASK_H_
#define TEMPERATURE_TASK_H_

#include <stdint.h>
#include <hih8120.h>

float hih8120_getHumidity();
float hih8120_getTemperature();
void temperature_measure_task(void * p);


#endif /* TEMPERATURE_TASK_H_ */