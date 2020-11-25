/*
 * light_task.c
 *
 * Created: 11/20/2020 4:48:43 PM
 *  Author: Matey Matev
 */ 

#include "light_task.h"
#include <tsl2591.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>

static uint16_t ppm;
static tsl2591_returnCode_t rc;

//TODO Include structs that are in documentation so we can start cahching the data. so we can show from which time was the data fetched.Useful for user stories that
//include history or changing settings.



void tsl2591Callback(tsl2591_returnCode_t rc)
{
	uint16_t _tmp;
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
			if ( TSL2591_OK == (rc = tsl2591_getFullSpectrumRaw(&_tmp)) )
			{
				printf("\nFull Raw:%04X\n", _tmp);
			}
			else if( TSL2591_OVERFLOW == rc )
			{
				printf("\nFull spectrum overflow - change gain and integration time\n");
			}

			if ( TSL2591_OK == (rc = tsl259_getVisibleRaw(&_tmp)) )
			{
				printf("Visible Raw:%04X\n", _tmp);
			}
			else if( TSL2591_OVERFLOW == rc )
			{
				printf("Visible overflow - change gain and integration time\n");
			}

			if ( TSL2591_OK == (rc = tsl2591_getInfraredRaw(&_tmp)) )
			{
				printf("Infrared Raw:%04X\n", _tmp);
			}
			else if( TSL2591_OVERFLOW == rc )
			{
				printf("Infrared overflow - change gain and integration time\n");
			}

			if ( TSL2591_OK == (rc = tsl2591_getLux(&_lux)) )
			{
				printf("Lux: %5.4f\n", _lux);
			}
			else if( TSL2591_OVERFLOW == rc )
			{
				printf("Lux overflow - change gain and integration time\n");
			}
			break;

		case TSL2591_OK:
			// Last command performed successful
			break;

		case TSL2591_DEV_ID_READY:
			// Dev ID now fetched
			break;

		default:
			break;
	}
}

void tsl2591_light_create() {
	void (*callback)(uint16_t) = &tsl2591Callback;

	tsl2591_create(callback);

}


uint16_t getLightFromSensor() {
	rc = tsl2591_enable();
	if(rc == TSL2591_OK)
	{
		vTaskDelay(pdMS_TO_TICKS(60));
		rc = tsl2591_fetchData(&ppm);
		(if rc == TSL2591_OK) {
			printf("Light Data : %d\r\n",ppm)
				return ppm;
		}

	}
	rc = tsl2591_disable();
}

