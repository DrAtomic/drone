#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include <freertos/semphr.h>

#define BURST_LEN 12

#define MPU6050_SENSOR_ADDR		0x68

#define MPU6050_FIFO_EN_REG		0x23
#define MPU6050_SMPRT_DIV_REG		0x25
#define MPU6050_WHO_AM_I_REG		0x75
#define MPU6050_USR_CTRL_REG		0x6A
#define MPU6050_PWR_MGMT_1_REG		0x6B
#define MPU6050_ACCEL_CONFIG_REG	0x1C
#define MPU6050_GYRO_CONFIG_REG	0x1B
#define MPU6050_ACCEL_XOUT_H_REG	0x3B
#define MPU6050_ACCEL_XOUT_L_REG	0x3C
#define MPU6050_ACCEL_YOUT_H_REG	0x3D
#define MPU6050_ACCEL_YOUT_L_REG	0x3E
#define MPU6050_ACCEL_ZOUT_H_REG	0x3F
#define MPU6050_ACCEL_ZOUT_L_REG	0x40
#define MPU6050_GYRO_XOUT_H_REG	0x43
#define MPU6050_GYRO_XOUT_L_REG	0x44
#define MPU6050_GYRO_YOUT_H_REG	0x45
#define MPU6050_GYRO_YOUT_L_REG	0x46
#define MPU6050_GYRO_ZOUT_H_REG	0x47
#define MPU6050_GYRO_ZOUT_L_REG	0x48
#define MPU6050_FIFO_R_W_REG		0x74
#define MPU6050_FIFO_COUNT_H_REG	0x72
#define MPU6050_FIFO_COUNT_L_REG	0x73

#define MPU6050_RESET_BIT		7
#define MPU6050_SLEEP_BIT		6
#define MPU6050_FIFO_EN_BIT		6
#define MPU6050_FIFO_RESET_BIT		2
#define MPU6050_FS_SEL_BIT		3
#define MPU6050_AFS_SEL_BIT		3
#define MPU6050_CLK_SEL_BIT		0

#define MPU6050_INT_PIN_CFG_REG	0x37
#define MPU6050_INT_LEVEL_LOW_VAL	1
#define MPU6050_INT_LEVEL_LOW_BIT	7
#define MPU6050_LATCH_INT_EN_VAL	1
#define MPU6050_LATCH_INT_EN_BIT	5
#define MPU6050_INT_RD_CLEAR_VAL	1
#define MPU6050_INT_RD_CLEAR_BIT	4

#define MPU6050_DATA_RDY_EN_REG	0x38
#define MPU6050_DATA_RDY_EN_VAL	1
#define MPU6050_DATA_RDY_EN_BIT	0

#define MPU6050_CLK_SEL_FIELD_LEN	3
#define MPU6050_FS_SEL_FIELD_LEN	2
#define MPU6050_AFS_SEL_FIELD_LEN	2

#define MPU6050_TEMP_FIFO_EN_BIT	7
#define MPU6050_XG_FIFO_EN_BIT		6
#define MPU6050_YG_FIFO_EN_BIT		5
#define MPU6050_ZG_FIFO_EN_BIT		4
#define MPU6050_ACCEL_FIFO_EN_BIT	3
#define MPU6050_SLV2_FIFO_EN_BIT	2
#define MPU6050_SLV1_FIFO_EN_BIT	1
#define MPU6050_SLV0_FIFO_EN_BIT	0

#define MPU6050_WHO_AM_I_VAL		0x68

#define MPU6050_RESET_VAL		1

#define MPU6050_INTERNAL_PLL_VAL	0
#define MPU6050_X_PLL_VAL		1
#define MPU6050_Y_PLL_VAL		2
#define MPU6050_Z_PLL_VAL		3

#define MPU6050_250_FS_SEL_VAL		0
#define MPU6050_500_FS_SEL_VAL		1
#define MPU6050_1000_FS_SEL_VAL	2
#define MPU6050_2000_FS_SEL_VAL	3

#define MPU6050_2G_AFS_SEL_VAL		0
#define MPU6050_4G_AFS_SEL_VAL		1
#define MPU6050_8G_AFS_SEL_VAL		2
#define MPU6050_16G_AFS_SEL_VAL	3

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
	uint32_t data_ready;
} MPU6050_TYPE;

void mpu6050_setup(MPU6050_TYPE *m);
void get_accel_data(ACCEL_TYPE *ad);
void clear_accel_data(ACCEL_TYPE *ad);
void get_gyro_data(GYRO_TYPE *gd);
void clear_gyro_data(GYRO_TYPE *gd);
void get_accel_and_gyro_from_fifo(MPU6050_TYPE *m);

/* for testing */
void peek_reg(uint8_t reg, uint8_t *data);
int add(int a, int b);
void get_fifo_count(size_t *fifo_count);
#endif /* MPU6050_H */
