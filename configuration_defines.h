/*
 * configuration_defines.h
 *
 * Created: 12/10/2020 7:34:12 PM
 *  Author: mmate
 */ 
#pragma once


#define	LORA_appEUI	"689DF9DF68156742"
#define	LORA_appKEY	"B09F779D3DF66B89B996955E3B4ED977"

#define	RETRIES	5

// #define LORA_SETUP	// Uncomment to setup loraWAN module and persist settings - necessary only on first run on device

#define	DIAG_INTERVAL	600	// Diagnostic output interval
#define	TEMPHUM_INTERVAL	500
#define	CO2_INTERVAL	400
#define LIGHT_INTERVAL	500
// #define LORA_INTERVAL	pdMS_TO_TICKS(300000UL)
#define LORA_INTERVAL	500
#define SERVO_INTERVAL	420

#define	SERVO_PORT	0
#define	LORA_RETRY_CONNECTION_DELAY	100

#define	TEMPHUM_READY_BIT	(1 << 0)
#define	CO2_READY_BIT	(1 << 1)
#define	LIGHT_READY_BIT	(1 << 2)
#define	LORA_READY_BIT	(1 << 3)
#define	SERVO_READY_BIT	(1 << 4)

#define	SENSORS_READY	((TEMPHUM_READY_BIT) | (CO2_READY_BIT) | (LIGHT_READY_BIT))
#define	SYSTEM_READY	((SENSORS_READY) | (LORA_READY_BIT) | (SERVO_READY_BIT))