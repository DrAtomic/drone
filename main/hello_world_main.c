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
#include "driver/gpio.h"

#define MAX_MOTOR 4

#define MOTOR_GPIO0 GPIO_NUM_5
#define MOTOR_GPIO1 GPIO_NUM_18
#define MOTOR_GPIO2 GPIO_NUM_19
#define MOTOR_GPIO3 GPIO_NUM_21

static TaskHandle_t motor_task;

static const BaseType_t app_cpu = 0;

typedef struct {
	gpio_num_t gpio;
} motor_t;

typedef struct drone {
	motor_t motors[MAX_MOTOR];
} drone_t;

drone_t drone;

void setup()
{
	drone.motors[0].gpio = MOTOR_GPIO0;
	drone.motors[1].gpio = MOTOR_GPIO1;
	drone.motors[2].gpio = MOTOR_GPIO2;
	drone.motors[3].gpio = MOTOR_GPIO3;
}

static void control_motors(void *parameters)
{
	motor_t *motors = (motor_t *)parameters;

	while (1) {
		for (size_t i = 0; i < MAX_MOTOR; i++)
			gpio_set_level(motors[i].gpio, 0);
		vTaskDelay(100);
		for (size_t i = 0; i < MAX_MOTOR; i++)
			gpio_set_level(motors[i].gpio, 1);
		vTaskDelay(100);
	}
}

static void create_tasks()
{
	xTaskCreatePinnedToCore(control_motors,
				"Motor Control",
				1204,
				(void *)&drone.motors,
				1,
				&motor_task,
				app_cpu);
}

static void arm_motors(void)
{
	esp_err_t ret;

	for (size_t i = 0; i < MAX_MOTOR ; i++) {
		ret = gpio_set_direction(drone.motors[i].gpio, GPIO_MODE_OUTPUT);
		if (ret != 0) {
			fprintf(stderr, "Failed to arm engine %zu, Restarting...", i);
			fflush(stdout);
			esp_restart();
		}
	}
}

void app_main(void)
{
	setup();
	arm_motors();
	create_tasks();
}
