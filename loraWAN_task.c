/*
 * loraWANHandler.c
 *
 * Created: 12/04/2019 10:09:05
 *  Author: IHA
 */
#include "loraWAN_task.h"

#include <stdio.h>

#include <lora_driver.h>

#include "configuration_defines.h"
#include "shared_data.h"



void lora_handler_task(void *pvParameters);


void lora_setup(void)
{
	lora_driver_returnCode_t rc;

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));

	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	static char hw_eiu[17];
	rc = lora_driver_getRn2483Hweui(hw_eiu);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), hw_eiu);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", hw_eiu, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(hw_eiu)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n",
			LORA_appEUI,
			LORA_appKEY,
			hw_eiu, 
			lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI, LORA_appKEY, hw_eiu)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;

	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
	}
	else
	{
		// Something went wrong
	}
}


/*-----------------------------------------------------------*/
void lora_handler_task(void *pvParameters)
{

	shared_data_t *sd = (shared_data_t*) pvParameters;

	lora_driver_payload_t uplink_payload;
	uplink_payload.port_no = 1;
	uplink_payload.len = 4;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		// I think we just need mutex to keep the pointer to the struct of not writing some info on other methods while doing the same to others.

		uint16_t hum = sd_getHumid(sd);
		int16_t temp = sd_getTemp(sd);
		uint16_t co2_ppm = sd_getCo2(sd);
		uint16_t ligth_ppm = sd_getLight(sd);

		uplink_payload.bytes[0] = hum >> 8;
		uplink_payload.bytes[1] = hum & 0xFF;
		uplink_payload.bytes[2] = temp >> 8;
		uplink_payload.bytes[3] = temp & 0xFF;
		//uplink_payload.bytes[4] = co2_ppm >> 8;
		//uplink_payload.bytes[5] = co2_ppm & 0xFF;
		//uplink_payload.bytes[6] = ligth_ppm >> 8;
		//uplink_payload.bytes[7] = ligth_ppm & 0xFF;

		printf("[*] Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &uplink_payload)));
	}
}
