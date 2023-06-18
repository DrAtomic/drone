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
	uint8_t data[2] = {0};

	while (1) {
		peek_pwr_config(data);
		printf("the data in the power config register is 0x%x\n", *data);
		printf("iteration [%d]\n", i);
		i++;
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}
