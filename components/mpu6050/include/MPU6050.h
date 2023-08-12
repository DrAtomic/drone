#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

#define MPU6050_SENSOR_ADDR		0x68

#define MPU6050_WHO_AM_I_REG		0x75
#define MPU6050_WHO_AM_I_VAL		0x68

#define MPU6050_ACCEL_XOUT_H_REG	0x3B
#define MPU6050_ACCEL_YOUT_H_REG	0x3D
#define MPU6050_ACCEL_ZOUT_H_REG	0x3F
#define MPU6050_GYRO_XOUT_H_REG	0x43
#define MPU6050_GYRO_YOUT_H_REG	0x45
#define MPU6050_GYRO_ZOUT_H_REG	0x47

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
#define MPU6050_2G_AFS_SEL_VAL		0
#define MPU6050_4G_AFS_SEL_VAL		1
#define MPU6050_8G_AFS_SEL_VAL		2
#define MPU6050_16G_AFS_SEL_VAL	3

#define MPU6050_GYRO_CONFIG_REG	0x1B
#define MPU6050_FS_SEL_BIT		3
#define MPU6050_FS_SEL_FIELD_LEN	2
#define MPU6050_250_FS_SEL_VAL		0
#define MPU6050_500_FS_SEL_VAL		1
#define MPU6050_1000_FS_SEL_VAL	2
#define MPU6050_2000_FS_SEL_VAL	3

#define LSB_SENSITIVITY_ACCEL_2G	16384.0
#define LSB_SENSITIVITY_ACCEL_4G	8192.0
#define LSB_SENSITIVITY_ACCEL_8G	4096.0
#define LSB_SENSITIVITY_ACCEL_16G	2048.0
#define LSB_SENSITIVITY_GYRO_250FS	131.0
#define LSB_SENSITIVITY_GYRO_500FS	65.5
#define LSB_SENSITIVITY_GYRO_1000FS	32.8
#define LSB_SENSITIVITY_GYRO_2000FS	16.4

typedef struct accel_type {
	int16_t x;
	int16_t y;
	int16_t z;
} ACCEL_TYPE;

typedef struct gyro_type {
	int16_t x;
	int16_t y;
	int16_t z;
} GYRO_TYPE;

typedef struct mpu6050_type {
	ACCEL_TYPE accl;
	GYRO_TYPE gyro;
} MPU6050_TYPE;

void mpu6050_setup();
void get_accel(ACCEL_TYPE *ad);
void get_gyro(GYRO_TYPE *gd);
void get_mpu(MPU6050_TYPE *m);

/* for testing */
void peek_reg(uint8_t reg, uint8_t *data);
int add(int a, int b);

#endif /* MPU6050_H */
