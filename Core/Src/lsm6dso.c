/*
 * lsm6dso.c
 *
 *  Created on: Jan 3, 2026
 *      Author: dawid
 */

#include "lsm6dso.h"

// Function initializing sensor
uint8_t LSM6DSO_Init(LSM6DSO_t *dev, I2C_HandleTypeDef *i2cHandle) {
	dev->i2cHandle = i2cHandle;
	uint8_t chip_id = 0;
	uint8_t data = 0;

	// Sanity check
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_WHO_AM_I_REG, 1, &chip_id, 1, 100);

	if (status != HAL_OK || chip_id != LSM6DSO_WHO_AM_I_VAL) {
		return 1;
	}

	// Accelerometer configuration
	data = 0x48;
	HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL1_XL, 1, &data, 1, 100);

	return 0;
}

// Function reading accelerometer
HAL_StatusTypeDef LSM6DSO_ReadAccel(LSM6DSO_t *dev) {
	uint8_t raw_data[6];

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_OUTX_L_A, 1, raw_data, 6, 100);

	if (status == HAL_OK) {
		dev->accel_raw[0] = (int16_t)((raw_data[1] << 8) | raw_data[0]); // X axis
		dev->accel_raw[1] = (int16_t)((raw_data[3] << 8) | raw_data[2]); // Y axis
		dev->accel_raw[2] = (int16_t)((raw_data[5] << 8) | raw_data[4]); // Z axis
	}

	return status;
}
