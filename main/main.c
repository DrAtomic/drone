#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "MPU6050.h"

MPU6050_TYPE m;

void setup()
{
	mpu6050_setup(&m);
}

void app_main(void)
{
	setup();

	while (1) {
		get_mpu(&m);

		printf("the acceleration data is x:%.2f, y:%.2f, z:%.2f\n",
		       m.accl.accel[0],
		       m.accl.accel[1],
		       m.accl.accel[2]);

		printf("the gyroscope data is x:%.2f, y:%.2f, z:%.2f\n",
		       m.gyro.gyro[0],
		       m.gyro.gyro[1],
		       m.gyro.gyro[2]);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
