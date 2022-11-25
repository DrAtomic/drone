/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BLINK_GPIO0 GPIO_NUM_5
#define BLINK_GPIO1 GPIO_NUM_18
#define BLINK_GPIO2 GPIO_NUM_19
#define BLINK_GPIO3 GPIO_NUM_21

static TaskHandle_t task1, task2, task3, task4;

static const BaseType_t app_cpu = 0;

struct arg_struct {
	gpio_num_t gpio;
	int delay;
};

void toggle_LEDs(void *parameters)
{
	struct arg_struct arg = *(struct arg_struct *)parameters;

	while (1) {
		gpio_set_level(arg.gpio, 0);
		vTaskDelay(arg.delay);
		gpio_set_level(arg.gpio, 1);
		vTaskDelay(arg.delay);
	}
}

static void create_tasks()
{
	static struct arg_struct arg1, arg2, arg3, arg4;

	arg1.gpio = BLINK_GPIO0;
	arg1.delay = 100;

	arg2.gpio = BLINK_GPIO1;
	arg2.delay = 150;

	arg3.gpio = BLINK_GPIO2;
	arg3.delay = 200;

	arg4.gpio = BLINK_GPIO3;
	arg4.delay = 50;

	xTaskCreatePinnedToCore(toggle_LEDs, "task1", 1204, (void *)&arg1, 1, &task1, app_cpu);
	xTaskCreatePinnedToCore(toggle_LEDs, "task2", 1204, (void *)&arg2, 1, &task2, app_cpu);
	xTaskCreatePinnedToCore(toggle_LEDs, "task3", 1204, (void *)&arg3, 1, &task3, app_cpu);
	xTaskCreatePinnedToCore(toggle_LEDs, "task4", 1204, (void *)&arg4, 1, &task4, app_cpu);

}

static void setup(void)
{
	gpio_set_direction(BLINK_GPIO0, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO1, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO2, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO3, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
	setup();
	create_tasks();
}
