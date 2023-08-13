#include <stdint.h>
#include <stdio.h>
#include "MPU6050.h"
#include "driver/i2c.h"
#include "portmacro.h"

#define I2C_MASTER_TIMEOUT_MS 1000
#define I2C_MASTER_NUM 0
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0

// testing for tests
int add(int a, int b)
{
	return a + b;
}

static const GYRO_SENSITIVITY_TYPE gyro_sens_table[] = {
	{MPU6050_250_FS_SEL_VAL, LSB_SENSITIVITY_GYRO_250FS},
	{MPU6050_500_FS_SEL_VAL, LSB_SENSITIVITY_GYRO_500FS},
	{MPU6050_1000_FS_SEL_VAL, LSB_SENSITIVITY_GYRO_1000FS},
	{MPU6050_2000_FS_SEL_VAL, LSB_SENSITIVITY_GYRO_2000FS}
};

static const ACCEL_SENSITIVITY_TYPE accel_sens_table[] = {
	{MPU6050_2G_AFS_SEL_VAL, LSB_SENSITIVITY_ACCEL_2G},
	{MPU6050_4G_AFS_SEL_VAL, LSB_SENSITIVITY_ACCEL_4G},
	{MPU6050_8G_AFS_SEL_VAL, LSB_SENSITIVITY_ACCEL_8G},
	{MPU6050_16G_AFS_SEL_VAL, LSB_SENSITIVITY_ACCEL_16G}
};

static esp_err_t mpu6050_register_write_byte(uint8_t reg_addr, uint8_t data)
{
	uint8_t write_buf[2] = {reg_addr, data};

	return i2c_master_write_to_device(I2C_MASTER_NUM,
					  MPU6050_SENSOR_ADDR,
					  write_buf,
					  sizeof(write_buf),
					  I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
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

static inline uint8_t make_mask(uint8_t start_bit, size_t mask_len)
{
	uint8_t ret = 0;
	for (int i = 0; i < mask_len; i++)
		ret |= 1 << (start_bit + i);
	return ret;
}

static void set_register_field(uint8_t reg_addr, uint8_t start_bit, uint8_t val, size_t mask_len)
{
	uint8_t mask = 0;
	uint8_t data = 0;

	ESP_ERROR_CHECK(mpu6050_register_read(reg_addr, &data, 1));
	mask = make_mask(start_bit, mask_len);
	data &= ~(mask);
	data |= (val << start_bit);
	ESP_ERROR_CHECK(mpu6050_register_write_byte(reg_addr, data));
}

static void set_register(uint8_t reg_addr, uint8_t val)
{
	ESP_ERROR_CHECK(mpu6050_register_write_byte(reg_addr, val));
}

static void mpu6050_check()
{
	uint8_t data = 0;
	size_t len = 1;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_WHO_AM_I_REG, &data, len));
	if (data != MPU6050_WHO_AM_I_VAL) {
		printf("error: not an mpu6050\n");
		exit(1);
	}
}

static void mpu6050_reset()
{
	set_register_field(MPU6050_PWR_MGMT_1_REG, MPU6050_RESET_BIT, MPU6050_RESET_VAL, 1);
}

static void mpu6050_set_clock(uint8_t clk_val)
{
	set_register_field(MPU6050_PWR_MGMT_1_REG,
			   MPU6050_CLK_SEL_BIT,
			   clk_val,
			   MPU6050_CLK_SEL_FIELD_LEN);
}

static void mpu6050_set_gyroscope_range(GYRO_SENSITIVITY_TYPE *g_sens, GYRO_SENSITIVITY_ENUM fs_sel_val)
{
	*g_sens = gyro_sens_table[fs_sel_val];

	set_register_field(MPU6050_GYRO_CONFIG_REG,
			   MPU6050_FS_SEL_BIT,
			   fs_sel_val,
			   MPU6050_FS_SEL_FIELD_LEN);
}

static void mpu6050_set_accel_range(ACCEL_SENSITIVITY_TYPE *a_sens, ACCEL_SENSITIVITY_ENUM afs_sel_val)
{
	*a_sens = accel_sens_table[afs_sel_val];

	set_register_field(MPU6050_ACCEL_CONFIG_REG,
			   MPU6050_AFS_SEL_BIT,
			   afs_sel_val,
			   MPU6050_AFS_SEL_FIELD_LEN);
}

static void mpu6050_wakeup()
{
	set_register_field(MPU6050_PWR_MGMT_1_REG, MPU6050_SLEEP_BIT, 0, 1);
}

static void mpu6050_lowpass_init()
{
	set_register(MPU6050_CONFIG_REG, 1);
}

void get_accel(ACCEL_TYPE *a)
{
	uint8_t data[6] = {0};
	size_t len = 6;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_ACCEL_XOUT_H_REG, data, len));

	a->accel_raw[X] = (int16_t)((data[0] << 8) | data[1]);
	a->accel_raw[Y] = (int16_t)((data[2] << 8) | data[3]);
	a->accel_raw[Z] = (int16_t)((data[4] << 8) | data[5]);

	for (size_t i = 0; i < AXIS_COUNT; i++)
		a->accel[i] = a->accel_raw[i] / a->accl_sens.accel_divider;
}

void get_gyro(GYRO_TYPE *g)
{
	uint8_t data[6] = {0};
	size_t len = 6;

	ESP_ERROR_CHECK(mpu6050_register_read(MPU6050_ACCEL_XOUT_H_REG, data, len));

	g->gyro_raw[X] =(int16_t)((data[0] << 8) | data[1]);
	g->gyro_raw[Y] =(int16_t)((data[2] << 8) | data[3]);
	g->gyro_raw[Z] =(int16_t)((data[4] << 8) | data[5]);

	for (size_t i = 0; i < AXIS_COUNT; i++)
		g->gyro[i] = g->gyro_raw[i] / g->gyro_sens.gyro_divider;
}

void get_mpu(MPU6050_TYPE *m)
{
	get_accel(&m->accl);
	get_gyro(&m->gyro);
}

void peek_reg(uint8_t reg, uint8_t *data)
{
	ESP_ERROR_CHECK(mpu6050_register_read(reg, data, 1));
}

void mpu6050_setup(MPU6050_TYPE *m)
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
	m->alignment = CW90_DEG;

	mpu6050_check();
	mpu6050_reset();
	mpu6050_lowpass_init();
	mpu6050_set_clock(MPU6050_X_PLL_VAL);
	vTaskDelay(15 / portTICK_PERIOD_MS);
	mpu6050_set_gyroscope_range(&m->gyro.gyro_sens, MPU6050_500_FS_SEL_VAL);
	mpu6050_set_accel_range(&m->accl.accl_sens, MPU6050_2G_AFS_SEL_VAL);
	mpu6050_wakeup();
}
