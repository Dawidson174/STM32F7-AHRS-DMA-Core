/*
 * lsm6dso.c
 *
 *  Created on: Jan 3, 2026
 *      Author: dawid
 */

#include "lsm6dso.h"

uint8_t LSM6DSO_Init(LSM6DSO_t *dev, I2C_HandleTypeDef *i2cHandle) {
	dev->i2cHandle = i2cHandle;

	for(int i=0; i<3; i++) {
		dev->accel_raw[i] = 0;
		dev->accel_g[i] = 0.0f;
		dev->gyro_raw[i] = 0;
		dev->gyro_dps[i] = 0.0f;
	}

	uint8_t chip_id = 0;
	uint8_t data = 0;

	//checking id
	if (HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_WHO_AM_I_REG, 1, &chip_id, 1, 100) != HAL_OK) {
		return 1;
	}

	if (chip_id != LSM6DSO_WHO_AM_I_VAL) return 2;

	// configuration Accel
	data = 0x48;
	HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL1_XL, 1, &data, 1, 100);

	// configuration Gyro
	data = 0x4C;
	HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL2_G, 1, &data, 1, 100);

	data = 0x44;
	HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL3_C, 1, &data, 1, 100);

	return 0;
}

void LSM6DSO_Request_DMA(LSM6DSO_t *dev) {
	HAL_I2C_Mem_Read_DMA(dev->i2cHandle, LSM6DSO_I2C_ADDR, LSM6DSO_OUTX_L_G, 1, dev->dma_buffer, 12);
}

void LSM6DSO_Process_DMA_Data(LSM6DSO_t *dev) {

	/* 1. Assembling Gyroscope data */
	dev->gyro_raw[0] = (int16_t)(dev->dma_buffer[1] << 8 | dev->dma_buffer[0]);
	dev->gyro_raw[1] = (int16_t)(dev->dma_buffer[3] << 8 | dev->dma_buffer[2]);
	dev->gyro_raw[2] = (int16_t)(dev->dma_buffer[5] << 8 | dev->dma_buffer[4]);

	/* Assembling Accelerometer data */
	dev->accel_raw[0] = (int16_t)(dev->dma_buffer[7] << 8 | dev->dma_buffer[6]);
	dev->accel_raw[1] = (int16_t)(dev->dma_buffer[9] << 8 | dev->dma_buffer[8]);
	dev->accel_raw[2] = (int16_t)(dev->dma_buffer[11] << 8 | dev->dma_buffer[10]);

	/* Math */
	/*Accel: convert mg to g (division by 1000) */
	dev->accel_g[0] = (dev->accel_raw[0] * LSM6DSO_ACC_SENSITIVITY) / 1000.0f;
	dev->accel_g[1] = (dev->accel_raw[1] * LSM6DSO_ACC_SENSITIVITY) / 1000.0f;
	dev->accel_g[2] = (dev->accel_raw[2] * LSM6DSO_ACC_SENSITIVITY) / 1000.0f;

	/* Gyro: dps (degrees per second) */
	dev->gyro_dps[0] = (dev->gyro_raw[0] * LSM6DSO_GYRO_SENSITIVITY);
	dev->gyro_dps[1] = (dev->gyro_raw[1] * LSM6DSO_GYRO_SENSITIVITY);
	dev->gyro_dps[2] = (dev->gyro_raw[2] * LSM6DSO_GYRO_SENSITIVITY);

	dev->data_ready = 1;
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

	dev->gyro_dps[0] = (dev->gyro_raw[0] * LSM6DSO_GYRO_SENSITIVITY);
	dev->gyro_dps[1] = (dev->gyro_raw[1] * LSM6DSO_GYRO_SENSITIVITY);
	dev->gyro_dps[2] = (dev->gyro_raw[2] * LSM6DSO_GYRO_SENSITIVITY);
}

void LSM6DSO_ReadAll(LSM6DSO_t *dev) {
	LSM6DSO_ReadAccel(dev);
	LSM6DSO_ReadGyro(dev);
}
