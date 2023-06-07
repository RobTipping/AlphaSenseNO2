/*
 * ADS1X15.c
 *
 *  Created on: Jun 6, 2023
 *      Author: rztip
 */


#include "ADS1X15.h"

#define	ADS1X15_ADDRESS	 (ADS1X15_ADDRESS_0 << 1) // Define sensor address and bit shift by 1


float resolutionTable[] = {0.1875, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125};

uint8_t ADS1X15_Initialise(ADS1X15 *dev, I2C_HandleTypeDef *i2cHandle)
{
	dev-> i2cHandle = i2cHandle;
	dev-> conversionReg = 0;
	dev-> loThreshReg = 0;
	dev-> hiThreshReg = 0;
	dev-> os = 0;
	dev-> mux = MUX_AIN0_AIN1;
	dev-> pga = GAIN_2_048;
	dev-> mode = SINGLE_SHOT;
	dev-> dr = SPS_128;
	dev-> compMode = TRADITONAL_COMP;
	dev-> compPol = ACTIVE_LOW;
	dev-> compLat = NON_LATCHING;
	dev-> compQue = DISABLE;
	dev-> milliVoltageRead = 0.0;
	dev-> configReg = buildConfigReg(dev);
}

uint16_t buildConfigReg(ADS1X15 *dev)
{
	return (dev->os <<15)+(dev->mux <<12)+(dev->pga <<9)+(dev->mode <<8)+(dev->dr <<5)+(dev->compMode <<4)+(dev->compPol <<3)+(dev->compLat <<2)+dev->compQue;
}

HAL_StatusTypeDef ADS1X15_ADC_SingleRead(ADS1X15 *dev, uint8_t mux)
{
	dev-> os = START_CONVERSION;
	dev-> mux = mux;
	dev-> configReg = buildConfigReg(dev);
	uint8_t data[2] = { dev-> configReg>>8, dev-> configReg};
	HAL_StatusTypeDef status = ADS1X15_WriteRegister(dev, CONFIG_REG, data);
	if(status != HAL_OK)
		return status;

	while (ADS1X15_busy(dev)== ADC_BUSY){}

	status = ADS1X15_ReadRegister(dev, CONVERSION_REG, data);

	dev-> conversionReg = ((data[0]<<8)+data[1]);

	dev-> milliVoltageRead = ((data[0]<<8)+data[1])*resolutionTable[dev->pga];


	return status;
}

float convertToMilliVolts(ADS1X15 *dev, uint32_t regValue)
{
	return regValue * resolutionTable[dev->pga];
}


uint8_t ADS1X15_busy(ADS1X15 *dev)
{
	uint8_t data[2];
	ADS1X15_ReadRegister(dev, CONFIG_REG, data);
	return data[0]>>7;
}

//low level functions
HAL_StatusTypeDef ADS1X15_ReadRegister(ADS1X15 *dev, uint8_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read( dev->i2cHandle, ADS1X15_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
}
HAL_StatusTypeDef ADS1X15_ReadRegisters(ADS1X15 *dev, uint8_t reg, uint8_t *data, uint8_t length)
{
	return HAL_I2C_Mem_Read( dev->i2cHandle, ADS1X15_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

HAL_StatusTypeDef ADS1X15_WriteRegister(ADS1X15 *dev, uint8_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Write( dev->i2cHandle, ADS1X15_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
}
