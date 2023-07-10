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

	int i = 0;
	ACCEL_DATA_TYPE ad = {0};
	GYRO_DATA_TYPE gd = {0};
	size_t fifo_count = 0;

	while (1) {
		get_fifo_count(&fifo_count);

		if (fifo_count > BURST_LEN) {
			get_accel_and_gyro_from_fifo(&gd, &ad);

			printf("the acceleration data is x:%.2f, y:%.2f, z:%.2f\n",
			       ad.x / LSB_SENSITIVITY_ACCEL_2G,
			       ad.y / LSB_SENSITIVITY_ACCEL_2G,
			       ad.z / LSB_SENSITIVITY_ACCEL_2G);
			clear_accel_data(&ad);

			printf("the gyroscope data is x:%.2f, y:%.2f, z:%.2f\n",
			       gd.x / LSB_SENSITIVITY_GYRO_500FS,
			       gd.y / LSB_SENSITIVITY_GYRO_500FS,
			       gd.z / LSB_SENSITIVITY_GYRO_500FS);
			clear_gyro_data(&gd);
		}

		printf("iteration [%d]\n", i);
		i++;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}
