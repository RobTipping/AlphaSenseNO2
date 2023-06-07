/*
 * Alphasense_NO2.h
 *
 *  Created on: 30 May 2023
 *      Author: rztip
 */

#ifndef INC_ALPHASENSE_NO2_H_
#define INC_ALPHASENSE_NO2_H_

#include "stm32L0xx_hal.h" // Needed for I2C

typedef struct{
	I2C_HandleTypeDef *i2cHandle;
	uint16_t we_zero_electronic;
	uint16_t we_zero_total;
	uint16_t ae_zero_electronic;
	uint16_t ae_zero_total;
	float sensitivity;
	float ppb;


}ALPHA_NO2;

uint8_t ALPHA_NO2_Initialise(ALPHA_NO2 *dev, I2C_HandleTypeDef *i2cHandle, uint16_t we_zero_electronic,uint16_t we_zero_total,uint16_t ae_zero_electronic,uint16_t ae_zero_total, float sensitivity);

float getNO2(ALPHA_NO2 *dev);


#endif /* INC_ALPHASENSE_NO2_H_ */
