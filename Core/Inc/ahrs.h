/*
 * ahrs.h
 *
 *  Created on: Jan 7, 2026
 *      Author: dawid
 */

#ifndef INC_AHRS_H_
#define INC_AHRS_H_

#include <stdint.h>
#include <math.h>

/* CONFIGURATION */
#define MAG_OFFSET_X  0.14f
#define MAG_OFFSET_Y -0.26f
#define MAG_OFFSET_Z -0.005f

/* DATA STRUCTURE */
typedef struct {
	/* Outputs [degrees] */
	float pitch;
	float roll;
	float yaw;

	/* FIlter Parameters */
	float alpha;
	float dt;
} AHRS_t;

/* API */
void AHRS_Init(AHRS_t *ahrs, float alpha, float dt);

void AHRS_Update(AHRS_t *ahrs, float ax, float ay, float az,
				float gx, float gy, float gz,
				float mx, float my, float mz);

#endif /* INC_AHRS_H_ */
