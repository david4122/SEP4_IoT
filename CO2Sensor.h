/*
 * CO2Sensor.h
 *
 * Created: 2020/11/20 12:46:27
 *  Author: Taiquan Sui
 */ 

#pragma once
#include <mh_z19.h>
#include <stdint.h>

void CO2_create();
uint16_t getCO2FromSensor();

