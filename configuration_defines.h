/*
 * configuration_defines.h
 *
 * Created: 12/10/2020 7:34:12 PM
 *  Author: mmate
 */ 
#pragma once

//Define EVENT BITS HERE. 

#define	LORA_appEUI	"689DF9DF68156742"
#define	LORA_appKEY	"B09F779D3DF66B89B996955E3B4ED977"

#define	RETRIES	5

// #define LORA_SETUP	// Uncomment to setup loraWAN module and persist settings - necessary only on first run on device

#define DIAG_INTERVAL	600	// Diagnostic output interval
#define	TEMPHUM_INTERVAL	500
#define	CO2_INTERVAL	400
#define LIGHT_INTERVAL	500
#define LORA_INTERVAL	pdMS_TO_TICKS(300000UL)