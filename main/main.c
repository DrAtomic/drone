#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "MPU6050.h"

void setup()
{
	print_test();
}

void app_main(void)
{
	setup();

	int i = 0;
	while (1) {
		printf("[%d] Hello world!\n", i);
		i++;
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}
