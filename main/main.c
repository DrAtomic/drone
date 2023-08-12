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
		       m.accl.x / LSB_SENSITIVITY_ACCEL_2G,
		       m.accl.y / LSB_SENSITIVITY_ACCEL_2G,
		       m.accl.z / LSB_SENSITIVITY_ACCEL_2G);

		printf("the gyroscope data is x:%.2f, y:%.2f, z:%.2f\n",
		       m.gyro.x / LSB_SENSITIVITY_GYRO_500FS,
		       m.gyro.y / LSB_SENSITIVITY_GYRO_500FS,
		       m.gyro.z / LSB_SENSITIVITY_GYRO_500FS);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
