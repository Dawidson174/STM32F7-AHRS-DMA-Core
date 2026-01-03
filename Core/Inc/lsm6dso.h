#ifndef INC_LSM6DSO_H_
#define INC_LSM6DSO_H_

#include "stm32f7xx_hal.h" //HAL library for F7

// Addresses
#define LSM6DSO_I2C_ADDR		(0x6B << 1)

// Registry
#define LSM6DSO_WHO_AM_I_REG 	0x0F
#define LSM6DSO_CTRL1_XL		0x10 // Accelerometer configuration
#define LSM6DSO_CTRL2_G			0x11 // Gyroscope configuration
#define LSM6DSO_STATUS_REG		0x1E
#define LSM6DSO_OUTX_L_A		0x28 // start data from accelerometer

#define LSM6DSO_WHO_AM_I_VAL	0x6C

// Main structure
typedef struct {
	I2C_HandleTypeDef *i2cHandle;

	// raw data
	int16_t accel_raw[3]; 	// X, Y, Z
	int16_t gyro_raw[3]; 	// X, Y, Z

} LSM6DSO_t;

// Functions
uint8_t LSM6DSO_Init(LSM6DSO_t *dev, I2C_HandleTypeDef *i2cHandle);

HAL_StatusTypeDef LSM6DSO_ReadAccel(LSM6DSO_t *dev);

#endif
