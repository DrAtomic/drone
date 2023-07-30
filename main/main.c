#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "MPU6050.h"

MPU6050_TYPE m = {0};

void setup()
{
	mpu6050_setup(&m);
}

void app_main(void)
{
	setup();

	int i = 0;
	while (1) {
		printf("iteration [%d]\n", i);
		i++;
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

}
