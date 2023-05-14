#include <stdio.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#include "MPU6050.h"
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"

void setup()
{
	esp_err_t err;
	int i2c_master_port = 0;
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 21,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = 22,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 400000
	};
	err = i2c_param_config(i2c_master_port, &conf);
	if (err != ESP_OK){
		printf("failed to config i2c\n");
		exit(1);
	}
	err = i2c_driver_install(i2c_master_port, conf.mode,  0, 0, 0);
	if (err != ESP_OK) {
		printf("i2c driver not installed\n");
		exit(1);
	}

	print_test();
}

void app_main(void)
{
	setup();

	int i = 0;
	while (1) {
		printf("i2c configured!\n");
		printf("i2c driver installed\n");
		printf("[%d] Hello world!\n", i);
		i++;
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}
