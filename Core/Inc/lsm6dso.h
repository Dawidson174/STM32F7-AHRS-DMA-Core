/*
 * lis3mdl.h
 *
 *  Created on: Jan 3, 2026
 *      Author: dawid
 */

#ifndef INC_LSM6DSO_H_
#define INC_LSM6DSO_H_

#include "stm32f7xx_hal.h" //HAL library for F7

// Addresses
#define LSM6DSO_I2C_ADDR		(0x6B << 1)

// Registry
#define LSM6DSO_WHO_AM_I_REG 	0x0F
#define LSM6DSO_CTRL1_XL		0x10 // Accelerometer configuration
#define LSM6DSO_CTRL2_G			0x11 // Gyroscope configuration
#define LSM6DSO_CTRL3_C			0x12 // Control register 3
#define LSM6DSO_STATUS_REG		0x1E
#define LSM6DSO_OUTX_L_G		0x22 // start data from gyroscope
#define LSM6DSO_OUTX_L_A		0x28 // start data from accelerometer

#define LSM6DSO_WHO_AM_I_VAL	0x6C

#define LSM6DSO_ACC_SENSITIVITY 0.122f
#define LSM6DSO_GYRO_SENSITIVITY 0.070f

// Main structure
typedef struct {
	I2C_HandleTypeDef *i2cHandle;

	// raw data
	int16_t accel_raw[3]; 	// X, Y, Z
	int16_t gyro_raw[3]; 	// X, Y, Z

	float accel_g[3];
	float gyro_dps[3];

	uint8_t dma_buffer[12];
	uint8_t data_ready;

} LSM6DSO_t;

// Functions
uint8_t LSM6DSO_Init(LSM6DSO_t *dev, I2C_HandleTypeDef *i2cHandle);

void LSM6DSO_ReadAccel(LSM6DSO_t *dev);
void LSM6DSO_ReadGyro(LSM6DSO_t *dev);
void LSM6DSO_ReadAll(LSM6DSO_t *dev);

void LSM6DSO_Request_DMA(LSM6DSO_t *dev);
void LSM6DSO_Process_DMA_Data(LSM6DSO_t *dev);

#endif /* INC_LSM6DSO_H_ */
