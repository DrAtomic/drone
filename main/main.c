#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "MPU6050.h"

void setup()
{
	mpu6050_setup();
}

void app_main(void)
{
	setup();
	MPU6050_TYPE m;

	while (1) {
		get_mpu(&m);

		printf("the acceleration data is x:%.2f, y:%.2f, z:%.2f\n",
		       m.accl.accel_raw[0] / LSB_SENSITIVITY_ACCEL_2G,
		       m.accl.accel_raw[1] / LSB_SENSITIVITY_ACCEL_2G,
		       m.accl.accel_raw[2] / LSB_SENSITIVITY_ACCEL_2G);

		printf("the gyroscope data is x:%.2f, y:%.2f, z:%.2f\n",
		       m.gyro.gyro_raw[0] / LSB_SENSITIVITY_GYRO_500FS,
		       m.gyro.gyro_raw[1] / LSB_SENSITIVITY_GYRO_500FS,
		       m.gyro.gyro_raw[2] / LSB_SENSITIVITY_GYRO_500FS);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
