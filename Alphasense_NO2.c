/*
 * Alphasense_NO2.c
 *
 *  Created on: Jun 7, 2023
 *      Author: rztip
 */


#include "Alphasense_NO2.h"
#include "ADS1X15.h"

ADS1X15 ADS1115;

int16_t ntLookUpTable[] = {130,127,124,121,118,115,112,109,106,103,100,96,92,88,84,80,76,72,68,64,60,58,56,54,52,50,48,46,44,42,40,
		38,36,34,32,30,28,26,24,22,20,3,-14,-31,-48,-65,-82,-99,-116,-133,-150};


uint8_t ALPHA_NO2_Initialise(ALPHA_NO2 *dev, I2C_HandleTypeDef *i2cHandle, uint16_t we_zero_electronic,uint16_t we_zero_total,uint16_t ae_zero_electronic,uint16_t ae_zero_total, float sensitivity)
{
	ADS1X15_Initialise(&ADS1115, i2cHandle);
	dev->i2cHandle = i2cHandle;
	dev-> we_zero_electronic = we_zero_electronic;
	dev-> we_zero_total = we_zero_total;
	dev-> ae_zero_electronic = ae_zero_electronic;
	dev-> ae_zero_total = ae_zero_total;
	dev-> sensitivity = sensitivity;
	dev-> ppb = 0.0;

}

float getNO2(ALPHA_NO2 *dev, uint8_t temperature)
{
	float op1 = 0.0;
	float op2 = 0.0;
	uint8_t numberSamples = 30;
	ADS1115.pga = GAIN_6_144;

	for(uint8_t i=0;i<numberSamples;i++)
	{
		ADS1X15_ADC_SingleRead(&ADS1115, MUX_AIN0_AIN1);

		op1 += ADS1115.conversionReg;

		ADS1X15_ADC_SingleRead(&ADS1115, MUX_AIN2_AIN3);

		op2 += ADS1115.conversionReg;
	}



	op1 = (convertToMilliVolts(&ADS1115, (uint32_t)(op1/numberSamples))) - dev-> we_zero_electronic;
	op2 = (convertToMilliVolts(&ADS1115, (uint32_t)(op2/numberSamples))) - dev-> ae_zero_electronic;

	return (op1 - (ntLookUpTable[temperature] / 100) * op2) / dev-> sensitivity;

}
