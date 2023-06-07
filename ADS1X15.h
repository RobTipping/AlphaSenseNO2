/*
 * ADS1X15.h
 *
 *  Created on: Jun 6, 2023
 *      Author: rztip
 */

#ifndef INC_ADS1X15_H_
#define INC_ADS1X15_H_

#include "stm32L0xx_hal.h" // Needed for I2C

#define ADS1X15_ADDRESS_0 	0x48 	//ADDR connected to GND
#define ADS1X15_ADDRESS_1 	0x49	//ADDR connected to VIN
#define ADS1X15_ADDRESS_2 	0x4A	//ADDR connected to SDA
#define ADS1X15_ADDRESS_3 	0x4B	//ADDR connected to SCL

#define CONVERSION_REG		0x00
#define CONFIG_REG			0x01
#define LO_THRESH_REG		0x02
#define HI_THRESH_REG		0x03

#define ADC_READY			0x01
#define ADC_BUSY			0x00
#define START_CONVERSION	0X01

#define MUX_AIN0_AIN1		0X00	//Default
#define MUX_AIN0_AIN3		0X01
#define MUX_AIN1_AIN3		0X02
#define MUX_AIN2_AIN3		0X03
#define MUX_AIN0			0X04
#define MUX_AIN1			0X05
#define MUX_AIN2			0X06
#define MUX_AIN3			0X07

#define GAIN_6_144			0X00
#define GAIN_4_096			0X01
#define GAIN_2_048			0X02	//Default
#define GAIN_1_024			0X03
#define GAIN_0_512			0X04
#define GAIN_0_256			0X05

#define CONTINUOUS			0X00
#define SINGLE_SHOT			0X01	//Default

#define SPS_8				0X00
#define SPS_16				0X01
#define SPS_32				0X02
#define SPS_64				0X03
#define SPS_128				0X04	//Default
#define SPS_250				0X05
#define SPS_475				0X06
#define SPS_860				0X07

#define TRADITONAL_COMP		0x00	//Default
#define WINDOW_COMP			0x01

#define ACTIVE_LOW			0x00	//Default
#define ACTIVE_HIGH			0x01

#define NON_LATCHING		0x00	//Default
#define LATCHING			0X01

#define ASSERT_ONE			0x00
#define ASSERT_TWO			0x01
#define ASSERT_THREE		0x02
#define DISABLE				0x03	//Default



typedef struct{
	I2C_HandleTypeDef *i2cHandle;
	uint16_t conversionReg;
	uint16_t configReg;
	uint16_t loThreshReg;
	uint16_t hiThreshReg;
	uint8_t os;
	uint8_t mux;
	uint8_t pga;
	uint8_t mode;
	uint8_t dr;
	uint8_t compMode;
	uint8_t compPol;
	uint8_t compLat;
	uint8_t compQue;
	float milliVoltageRead;

}ADS1X15;


//Initialisation
uint8_t ADS1X15_Initialise(ADS1X15 *dev, I2C_HandleTypeDef *i2cHandle);

uint16_t buildConfigReg();

HAL_StatusTypeDef ADS1X15_ADC_SingleRead(ADS1X15 *dev, uint8_t mux);

float convertToMilliVolts(ADS1X15 *dev, uint32_t regValue);

uint8_t ADS1X15_busy(ADS1X15 *dev);

//low level functions
HAL_StatusTypeDef ADS1X15_ReadRegister(ADS1X15 *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef ADS1X15_ReadRegisters(ADS1X15 *dev, uint8_t reg, uint8_t *data, uint8_t length);

HAL_StatusTypeDef ADS1X15_WriteRegister(ADS1X15 *dev, uint8_t reg, uint8_t *data);


#endif /* INC_ADS1X15_H_ */
