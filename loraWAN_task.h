/*
 * loraWANHandler.h
 *
 * Created: 11/14/2020 12:07:13 AM
 *  Author: semin
 */
#include "shared_data.h"
#pragma once


void lora_init(shared_data_t* sd);

void lora_task(void *pvParameters );