/*
 * lsm6dso.c
 *
 *  Created on: Jan 3, 2026
 *      Author: dawid
 */

#include "lsm6dso.h"

uint8_t LSM6DSO_Init(LSM6DSO_t *dev, I2C_HandleTypeDef *i2cHandle) {
	dev->i2cHandle = i2cHandle;
	uint8_t chip_id = 0;
	uint8_t data = 0;

	//checking id
	HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_WHO_AM_I_REG, 1, &chip_id, 1, 100);
	if (chip_id != LSM6DSO_WHO_AM_I_VAL) return 1;

	// configuration Accel
	data = 0x48;
	HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL1_XL, 1, &data, 1, 100);

	// configuration Gyro
	data = 0x4C;
	HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL2_G, 1, &data, 1, 100);

	return 0;
}

void LSM6DSO_ReadAccel(LSM6DSO_t *dev) {
	uint8_t raw[6];
	HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_OUTX_L_A, 1, raw, 6, 100);

	dev->accel_raw[0] = (int16_t)(raw[1] << 8 | raw[0]);
	dev->accel_raw[1] = (int16_t)(raw[3] << 8 | raw[2]);
	dev->accel_raw[2] = (int16_t)(raw[5] << 8 | raw[4]);

	dev->accel_g[0] = (dev->accel_raw[0] * LSM6DSO_ACC_SENSITIVITY) / 1000.0f;
	dev->accel_g[1] = (dev->accel_raw[1] * LSM6DSO_ACC_SENSITIVITY) / 1000.0f;
	dev->accel_g[2] = (dev->accel_raw[2] * LSM6DSO_ACC_SENSITIVITY) / 1000.0f;
}

void LSM6DSO_ReadGyro(LSM6DSO_t *dev) {
	uint8_t raw[6];
	HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_OUTX_L_G, 1, raw, 6, 100);

	dev->gyro_raw[0] = (int16_t)(raw[1] << 8 | raw[0]);
	dev->gyro_raw[1] = (int16_t)(raw[3] << 8 | raw[2]);
	dev->gyro_raw[2] = (int16_t)(raw[5] << 8 | raw[4]);

	dev->gyro_dps[0] = (dev->gyro_raw[0] * LSM6DSO_GYRO_SENSITIVITY) / 1000.0f;
	dev->gyro_dps[1] = (dev->gyro_raw[1] * LSM6DSO_GYRO_SENSITIVITY) / 1000.0f;
	dev->gyro_dps[2] = (dev->gyro_raw[2] * LSM6DSO_GYRO_SENSITIVITY) / 1000.0f;
}

void LSM6DSO_ReadAll(LSM6DSO_t *dev) {
	LSM6DSO_ReadAccel(dev);
	LSM6DSO_ReadGyro(dev);
}
