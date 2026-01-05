/*
 * lis3mdl.h
 *
 *  Created on: Jan 5, 2026
 *      Author: dawid
 */

#ifndef INC_LIS3MDL_H_
#define INC_LIS3MDL_H_

#include "stm32f7xx_hal.h"

#define LIS3MDL_I2C_ADDR		(0x1E << 1)
#define LIS3MDL_WHO_AM_I		0x0F
#define LIS3MDL_CTRL_REG1		0x20
#define LIS3MDL_CTRL_REG2		0x21
#define LIS3MDL_CTRL_REG3		0x22
#define LIS3MDL_OUT_X_L			0x28

#define LIS3MDL_WHO_AM_I_VAL	0x3D

#define LIS3MDL_SENSITIVITY		(1.0f / 6842.0f)

typedef struct {
	I2C_HandleTypeDef *i2cHandle;

	int16_t mag_raw[3];

	float mag_gauss[3];
} LIS3MDL_t;

uint8_t LIS3MDL_Init(LIS3MDL_t *dev, I2C_HandleTypeDef *i2cHandle);
void LIS3MDL_ReadMag(LIS3MDL_t *dev);

#endif /* INC_LIS3MDL_H_ */
