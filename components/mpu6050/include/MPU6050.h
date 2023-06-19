#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

#define MPU6050_SENSOR_ADDR		0x68
#define MPU6050_WHO_AM_I_REG		0x75
#define MPU6050_WHO_AM_I_VAL		0x68
#define MPU6050_PWR_MGMT_1_REG		0x6B
#define MPU6050_ACCEL_CONFIG_REG	0x1C
#define MPU6050_GYRO_CONFIG_REG	0x1B

#define MPU6050_RESET_BIT		7
#define MPU6050_SLEEP_BIT		6
#define MPU6050_FS_SEL_BIT		3
#define MPU6050_AFS_SEL_BIT		3
#define MPU6050_CLK_SEL_BIT		0

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

void mpu6050_setup();
void peek_pwr_config(uint8_t *data);

int add(int a, int b);

#endif /* MPU6050_H */
