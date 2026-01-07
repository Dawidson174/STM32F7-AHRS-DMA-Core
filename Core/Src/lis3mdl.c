/*
 * lis3mdl.c
 *
 *  Created on: Jan 5, 2026
 *      Author: dawid
 */

#include "lis3mdl.h"

uint8_t LIS3MDL_Init(LIS3MDL_t *dev, I2C_HandleTypeDef *i2cHandle) {
	dev->i2cHandle = i2cHandle;

	dev->mag_raw[0] = 0; dev->mag_raw[1] = 0; dev->mag_raw[2] = 0;
	dev->mag_gauss[0] = 0.0f; dev->mag_gauss[1] = 0.0f; dev->mag_gauss[2] = 0.0f;

	uint8_t chip_id = 0;
	uint8_t ctrl_data = 0;

	HAL_I2C_Mem_Read(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_WHO_AM_I, 1, &chip_id, 1, 100);

	if (chip_id != LIS3MDL_WHO_AM_I_VAL) {
		return 1;
	}

	ctrl_data = 0x7C;
	HAL_I2C_Mem_Write(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_CTRL_REG1, 1, &ctrl_data, 1, 100);

	ctrl_data = 0x00;
	HAL_I2C_Mem_Write(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_CTRL_REG2, 1, &ctrl_data, 1, 100);

	ctrl_data = 0x00;
	HAL_I2C_Mem_Write(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_CTRL_REG3, 1, &ctrl_data, 1, 100);

	ctrl_data = 0x0C;
	HAL_I2C_Mem_Write(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_CTRL_REG4, 1, &ctrl_data, 1, 100);

	ctrl_data = 0x40;
	HAL_I2C_Mem_Write(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_CTRL_REG5, 1, &ctrl_data, 1, 100);

	return 0;
}

void LIS3MDL_Request_DMA(LIS3MDL_t *dev) {
	HAL_I2C_Mem_Read_DMA(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_OUT_X_L | 0x80, 1, dev->dma_buffer, 6);
}

void LIS3MDL_Process_DMA_Data(LIS3MDL_t *dev) {
	dev->mag_raw[0] = (int16_t)(dev->dma_buffer[1] << 8 | dev->dma_buffer[0]);
	dev->mag_raw[1] = (int16_t)(dev->dma_buffer[3] << 8 | dev->dma_buffer[2]);
	dev->mag_raw[2] = (int16_t)(dev->dma_buffer[5] << 8 | dev->dma_buffer[4]);

	dev->mag_gauss[0] = dev->mag_raw[0] * LIS3MDL_SENSITIVITY;
	dev->mag_gauss[1] = dev->mag_raw[1] * LIS3MDL_SENSITIVITY;
	dev->mag_gauss[2] = dev->mag_raw[2] * LIS3MDL_SENSITIVITY;

	dev->data_ready = 1;
}

void LIS3MDL_ReadMag(LIS3MDL_t *dev) {
	uint8_t raw[6];

	HAL_I2C_Mem_Read(dev->i2cHandle, LIS3MDL_I2C_ADDR, LIS3MDL_OUT_X_L, 1, raw, 6, 100);

	dev->mag_raw[0] = (int16_t)(raw[1] << 8 | raw[0]);
	dev->mag_raw[1] = (int16_t)(raw[3] << 8 | raw[2]);
	dev->mag_raw[2] = (int16_t)(raw[5] << 8 | raw[4]);

	dev->mag_gauss[0] = dev->mag_raw[0] * LIS3MDL_SENSITIVITY;
	dev->mag_gauss[1] = dev->mag_raw[1] * LIS3MDL_SENSITIVITY;
	dev->mag_gauss[2] = dev->mag_raw[2] * LIS3MDL_SENSITIVITY;
}
