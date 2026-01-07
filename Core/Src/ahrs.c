/*
 * ahrs.c
 *
 *  Created on: Jan 7, 2026
 *      Author: dawid
 */

#include "ahrs.h"

/* Constants for unit conversion */
#define RAD_TO_DEG 57.2957795f
#define DEG_TO_RAD 0.01745329f

void AHRS_Init(AHRS_t *ahrs, float alpha, float dt) {
    if (ahrs == NULL) return;

    ahrs->pitch = 0.0f;
    ahrs->roll  = 0.0f;
    ahrs->yaw   = 0.0f;
    ahrs->alpha = alpha;
    ahrs->dt    = dt;
}

void AHRS_Update(AHRS_t *ahrs, float ax, float ay, float az,
				float gx, float gy, float gz,
				float mx, float my, float mz) {

	if (ahrs == NULL) return;

	/* 1. Pitch & Roll (Accelerometer + Gyroscope) */
	float pitch_acc = atan2f(-ax, sqrtf(ay*ay + az*az)) * RAD_TO_DEG;
	float roll_acc  = atan2f(ay, az) * RAD_TO_DEG;

	/* Complementary Filter */
	ahrs->pitch = ahrs->alpha * (ahrs->pitch + gy * ahrs->dt) + (1.0f - ahrs->alpha) * pitch_acc;
	ahrs->roll = ahrs->alpha * (ahrs->roll + gx * ahrs->dt) + (1.0f - ahrs->alpha) * roll_acc;

	/* 2. Yaw (Magnetometer + Tilt Compensation) */

	/* Apply Hard Iron Calibration */
	float mx_c = mx - MAG_OFFSET_X;
	float my_c = my - MAG_OFFSET_Y;
	float mz_c = mz - MAG_OFFSET_Z;

	/* Pre-calculate trig values for tilt compensation */
	float pitch_rad = ahrs->pitch * DEG_TO_RAD;
	float roll_rad = ahrs->roll * DEG_TO_RAD;

	float sin_p = sinf(pitch_rad);
	float cos_p = cosf(pitch_rad);
	float sin_r = sinf(roll_rad);
	float cos_r = cosf(roll_rad);

	/* Tilt compensation: Rotate magnetic vector to horizontal plane */
	float X_h = mx_c * cos_p + my_c * sin_r * sin_p + mz_c * cos_r * sin_p;
	float Y_h = my_c * cos_r - mz_c * sin_r;

	/* Calculate Yaw */
	float yaw = atan2f(Y_h, X_h) * RAD_TO_DEG;

	/*Normalize to 0-360 range */
	if (yaw < 0.0f) {
		yaw += 360.0f;
	}

	ahrs->yaw = yaw;
}
