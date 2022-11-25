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

static TaskHandle_t task1;

static const BaseType_t app_cpu = 0;

void toggle_LEDs0(void *parameters)
{
	while (1) {
		gpio_set_level(BLINK_GPIO0, 0);
		vTaskDelay(100);
		gpio_set_level(BLINK_GPIO0, 1);
		vTaskDelay(100);
	}
}

static void setup(void)
{
	gpio_set_direction(BLINK_GPIO0, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO1, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO2, GPIO_MODE_OUTPUT);
	gpio_set_direction(BLINK_GPIO3, GPIO_MODE_OUTPUT);
	xTaskCreatePinnedToCore(toggle_LEDs0, "task1", 1204, NULL, 1, &task1, app_cpu);
}

void app_main(void)
{
	setup();
}
