#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

#define MPU6050_SENSOR_ADDR		0x68

#define MPU6050_WHO_AM_I_REG		0x75
#define MPU6050_WHO_AM_I_VAL		0x68

#define MPU6050_ACCEL_XOUT_H_REG	0x3B
#define MPU6050_GYRO_XOUT_H_REG	0x43

#define MPU6050_CONFIG_REG		0x1A

#define MPU6050_PWR_MGMT_1_REG		0x6B
#define MPU6050_CLK_SEL_BIT		0
#define MPU6050_RESET_VAL		1
#define MPU6050_SLEEP_BIT		6
#define MPU6050_RESET_BIT		7
#define MPU6050_CLK_SEL_FIELD_LEN	3
#define MPU6050_X_PLL_VAL		1

#define MPU6050_ACCEL_CONFIG_REG	0x1C
#define MPU6050_AFS_SEL_BIT		3
#define MPU6050_AFS_SEL_FIELD_LEN	2
#define LSB_SENSITIVITY_ACCEL_2G	16384.0
#define LSB_SENSITIVITY_ACCEL_4G	8192.0
#define LSB_SENSITIVITY_ACCEL_8G	4096.0
#define LSB_SENSITIVITY_ACCEL_16G	2048.0

#define MPU6050_GYRO_CONFIG_REG	0x1B
#define MPU6050_FS_SEL_BIT		3
#define MPU6050_FS_SEL_FIELD_LEN	2
#define LSB_SENSITIVITY_GYRO_250FS	131.0
#define LSB_SENSITIVITY_GYRO_500FS	65.5
#define LSB_SENSITIVITY_GYRO_1000FS	32.8
#define LSB_SENSITIVITY_GYRO_2000FS	16.4

#define AXIS_COUNT 3

typedef enum {
    X = 0,
    Y,
    Z
} AXIS_ENUM;

typedef enum {
	MPU6050_2G_AFS_SEL_VAL = 0,
	MPU6050_4G_AFS_SEL_VAL,
	MPU6050_8G_AFS_SEL_VAL,
	MPU6050_16G_AFS_SEL_VAL
} ACCEL_SENSITIVITY_ENUM;

typedef struct accel_sensitivity_type {
	ACCEL_SENSITIVITY_ENUM accel_sensitivity;
	float accel_divider;
} ACCEL_SENSITIVITY_TYPE;

typedef struct accel_type {
	int16_t accel_raw[AXIS_COUNT];
	float accel[AXIS_COUNT];
	ACCEL_SENSITIVITY_TYPE accl_sens;
} ACCEL_TYPE;

typedef enum {
	MPU6050_250_FS_SEL_VAL = 0,
	MPU6050_500_FS_SEL_VAL,
	MPU6050_1000_FS_SEL_VAL,
	MPU6050_2000_FS_SEL_VAL
} GYRO_SENSITIVITY_ENUM;

typedef struct gyro_sensitivity_type {
	GYRO_SENSITIVITY_ENUM gyro_sensitivity;
	float gyro_divider;
} GYRO_SENSITIVITY_TYPE;

typedef struct gyro_type {
	int16_t gyro_raw[AXIS_COUNT];
	float gyro[AXIS_COUNT];
	GYRO_SENSITIVITY_TYPE gyro_sens;
} GYRO_TYPE;

typedef enum {
	CW0_DEG = 1,
	CW90_DEG = 2,
	PERP0_DEG = 3
} ALIGNMENT_ENUM;

typedef struct mpu6050_type {
	ACCEL_TYPE accl;
	GYRO_TYPE gyro;
	ALIGNMENT_ENUM alignment;
} MPU6050_TYPE;

void mpu6050_setup(MPU6050_TYPE *m);
void get_accel(ACCEL_TYPE *a);
void get_gyro(GYRO_TYPE *g);
void get_mpu(MPU6050_TYPE *m);

/* for testing */
void peek_reg(uint8_t reg, uint8_t *data);
int add(int a, int b);

#endif /* MPU6050_H */
