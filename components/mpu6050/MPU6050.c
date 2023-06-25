#include <stdint.h>
#include <stdio.h>
#include "MPU6050.h"
#include "driver/i2c.h"

#define I2C_MASTER_TIMEOUT_MS 1000
#define I2C_MASTER_NUM 0
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0

int add(int a, int b)
{
	return a + b;
}

static esp_err_t mpu6050_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM,
				     MPU6050_SENSOR_ADDR,
				     write_buf,
				     sizeof(write_buf),
				     I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

static esp_err_t mpu6050_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
	return i2c_master_write_read_device(I2C_MASTER_NUM,
					    MPU6050_SENSOR_ADDR,
					    &reg_addr,
					    1,
					    data,
					    len,
					    I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

static void mpu6050_check()
{
	uint8_t data = {0};
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_WHO_AM_I_REG, &data, len));
	if (data != MPU6050_WHO_AM_I_VAL) {
		printf("error: not an mpu6050\n");
		exit(1);
	}
}

static void mpu6050_reset()
{
	uint8_t data = {0};
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_PWR_MGMT_1_REG, &data, len));
	data |= (1 << MPU6050_RESET_BIT);
	ESP_ERROR_CHECK(mpu6050_register_write_byte(MPU6050_PWR_MGMT_1_REG, data));
}

static void mpu6050_set_clock(uint8_t clk_val)
{
	uint8_t data = {0};
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_PWR_MGMT_1_REG, &data, len));
	data |= (clk_val << MPU6050_CLK_SEL_BIT);
	ESP_ERROR_CHECK(mpu6050_register_write_byte(MPU6050_PWR_MGMT_1_REG, data));
}

static void mpu6050_set_gyroscope_range(uint8_t fs_sel_val)
{
	uint8_t data = {0};
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_GYRO_CONFIG_REG, &data, len));
	data |= (fs_sel_val << MPU6050_FS_SEL_BIT);
	ESP_ERROR_CHECK(mpu6050_register_write_byte(MPU6050_GYRO_CONFIG_REG, data));
}

static void mpu6050_set_accel_range(uint8_t afs_sel_val)
{
	uint8_t data = {0};
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_ACCEL_CONFIG_REG, &data, len));
	data |= (afs_sel_val << MPU6050_AFS_SEL_BIT);
	ESP_ERROR_CHECK(mpu6050_register_write_byte(MPU6050_ACCEL_CONFIG_REG, data));
}

static void mpu6050_wakeup()
{
	uint8_t data = {0};
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_PWR_MGMT_1_REG, &data, len));
	data &= ~(1 << MPU6050_SLEEP_BIT);
	ESP_ERROR_CHECK(mpu6050_register_write_byte(MPU6050_PWR_MGMT_1_REG, data));
}

void mpu6050_setup()
{
	int i2c_master_port = I2C_MASTER_NUM;
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = I2C_MASTER_SDA_IO,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = I2C_MASTER_SCL_IO,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ,
	};
	ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port,
					   conf.mode,
					   I2C_MASTER_RX_BUF_DISABLE,
					   I2C_MASTER_TX_BUF_DISABLE,
					   0));

	mpu6050_check();
	mpu6050_reset();
	mpu6050_set_clock(MPU6050_X_PLL_VAL);
	mpu6050_set_gyroscope_range(MPU6050_500_FS_SEL_VAL);
	mpu6050_set_accel_range(MPU6050_2G_AFS_SEL_VAL);
	mpu6050_wakeup();
}

void peek_pwr_config(uint8_t *data)
{
	size_t len = 1;
	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_PWR_MGMT_1_REG, data, len));
}
