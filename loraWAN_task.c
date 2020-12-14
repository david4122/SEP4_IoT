/*
 * loraWANHandler.c
 *
 * Created: 12/04/2019 10:09:05
 *  Author: IHA
 */
#include "loraWAN_task.h"

#include "configuration_defines.h"

#include <stdio.h>

#include <ihal.h>
#include <lora_driver.h>

#include "shared_data.h"


void print_arr(char* prefix, uint8_t* arr, int len);

void lora_init(void) {
	hal_create(configMAX_PRIORITIES - 1);
	lora_driver_create(ser_USART1, NULL);
	puts("[*] LORA: Initialized");
}

void lora_task(void *pvParameters) {
	shared_data_t *sd = (shared_data_t*) pvParameters;

	lora_driver_resetRn2483(1); // Activate reset line
	vTaskDelay(2);
	lora_driver_resetRn2483(0); // Release reset line
	vTaskDelay(150); // Wait for tranceiver module to wake up after reset
	lora_driver_flushBuffers();
	
	lora_driver_returnCode_t ret;
	
#ifdef LORA_SETUP
	
	static char devEui[17];
	while(1) {
		vTaskDelay(100);
		
		if((ret = lora_driver_rn2483FactoryReset()) != LORA_OK) {
			printf("[!] LORA: Failed to factory reset: %d\n", ret);
			continue;
		}
	
		if((ret = lora_driver_configureToEu868()) != LORA_OK) {
			printf("[!] LORA: Failed to configure EU868: %d\n", ret);
			continue;
		}

		if((ret = lora_driver_getRn2483Hweui(devEui)) != LORA_OK) {
			printf("[!] LORA: Failed to fetch dev EUI: %d\n", ret);
			continue;
		}
	
		if((ret = lora_driver_setOtaaIdentity(LORA_appEUI, LORA_appKEY, devEui)) != LORA_OK) {
			printf("[!] LORA: Failed to onfigure OTAA: %d\n", ret);
			continue;
		}
	
		if ((ret = lora_driver_saveMac()) != LORA_OK) {
			printf("[!] LORA: Failed to persist settings: %d\n", ret);
		}
		
		puts("[*] LORA: Module configured");

		break;
	}
	
#endif

	char attempt = 1;
	while((ret = lora_driver_join(LORA_OTAA)) != LORA_ACCEPTED) {
		printf("[!] LORA: Could not joint network: %d (attempt %d)\n", ret, (int) attempt++);
		vTaskDelay(200);
	}
	puts("[*] LORA: Joined network");
	
	vTaskDelay(50);
	
	lora_driver_payload_t uplink_payload;
	uplink_payload.port_no = 1;
	uplink_payload.len = 8;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = (LORA_INTERVAL); // Upload message every 5 minutes
	xLastWakeTime = xTaskGetTickCount();

	while(1) {
		uint16_t hum = sd_getHumid(sd);
		int16_t temp = sd_getTemp(sd);
		uint16_t co2_ppm = sd_getCo2(sd);
		uint16_t ligth_ppm = sd_getLight(sd);

		uplink_payload.bytes[0] = hum >> 8;
		uplink_payload.bytes[1] = hum & 0xFF;
		uplink_payload.bytes[2] = temp >> 8;
		uplink_payload.bytes[3] = temp & 0xFF;
		uplink_payload.bytes[4] = co2_ppm >> 8;
		uplink_payload.bytes[5] = co2_ppm & 0xFF;
		uplink_payload.bytes[6] = ligth_ppm >> 8;
		uplink_payload.bytes[7] = ligth_ppm & 0xFF;

		ret = lora_driver_sendUploadMessage(false, &uplink_payload);
		if(ret != LORA_MAC_TX_OK && ret != LORA_MAC_RX) {
			printf("[!] LORA: Failed to upload payload: %d\n", ret);
		}
		print_arr("[>] LORA: Uploaded: ", uplink_payload.bytes, 8);
		
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void print_arr(char* prefix, uint8_t* arr, int len) {
	printf("%s ", prefix);
	for(int i = 0; i < len; i++) {
		printf("%02X ", arr[i]);
	}
	puts("");
}
