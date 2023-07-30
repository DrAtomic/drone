#include <stdint.h>
#include <stdio.h>
#include "MPU6050.h"
#include "driver/i2c.h"
#include <inttypes.h>

#define I2C_MASTER_TIMEOUT_MS 1000
#define I2C_MASTER_NUM 0
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0

#define GPIO_INPUT_IO_0     19
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0)
#define ESP_INTR_FLAG_DEFAULT 0

// testing for tests
int add(int a, int b)
{
	return a + b;
}

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

static void mpu6050_interrupt_config()
{
	uint8_t val = (MPU6050_INT_LEVEL_LOW_VAL << MPU6050_INT_LEVEL_LOW_BIT)
		| (MPU6050_INT_RD_CLEAR_VAL << MPU6050_INT_RD_CLEAR_BIT);
	set_register(MPU6050_INT_PIN_CFG_REG, val);
}

static void mpu6050_interrupt_enable()
{
	uint8_t val = (MPU6050_DATA_RDY_EN_VAL << MPU6050_DATA_RDY_EN_BIT);
	set_register(MPU6050_DATA_RDY_EN_REG, val);
}

static void mpu6050_set_clock(uint8_t clk_val)
{
	set_register_field(MPU6050_PWR_MGMT_1_REG,
			   MPU6050_CLK_SEL_BIT,
			   clk_val,
			   MPU6050_CLK_SEL_FIELD_LEN);
}

static void mpu6050_set_gyroscope_range(uint8_t fs_sel_val)
{
	set_register_field(MPU6050_GYRO_CONFIG_REG,
			   MPU6050_FS_SEL_BIT,
			   fs_sel_val,
			   MPU6050_FS_SEL_FIELD_LEN);
}

static void mpu6050_set_accel_range(uint8_t afs_sel_val)
{
	set_register_field(MPU6050_ACCEL_CONFIG_REG,
			   MPU6050_AFS_SEL_BIT,
			   afs_sel_val,
			   MPU6050_AFS_SEL_FIELD_LEN);
}

static void mpu6050_wakeup()
{
	set_register_field(MPU6050_PWR_MGMT_1_REG, MPU6050_SLEEP_BIT, 0, 1);
}

static void mpu6050_set_fifo()
{
	uint8_t data = 0;

	set_register_field(MPU6050_SMPRT_DIV_REG, 0, 0xff, 1);

	data = (1 << MPU6050_ACCEL_FIFO_EN_BIT)
		| (1 << MPU6050_XG_FIFO_EN_BIT)
		| (1 << MPU6050_YG_FIFO_EN_BIT)
		| (1 << MPU6050_ZG_FIFO_EN_BIT);
	set_register(MPU6050_FIFO_EN_REG, data);

	set_register(MPU6050_USR_CTRL_REG,
		     (1 << MPU6050_FIFO_EN_BIT)
		     | (1 << MPU6050_FIFO_RESET_BIT));
}

TaskHandle_t isr_handler;

static void mpu6050_data_ready_handler(void* arg)
{
	MPU6050_TYPE *inner_m = (MPU6050_TYPE *) arg;
	while (1) {
		inner_m->data_ready = ulTaskNotifyTake(pdTRUE, portMAX_DELAY) ;
		if (inner_m->data_ready)
			printf("notified :) %"PRIu32"\n", inner_m->data_ready);
	}
}

static void IRAM_ATTR mpu6050_data_ready_isr(void *arg)
{
	vTaskNotifyGiveFromISR(isr_handler, NULL);
}

static uint16_t read_16bit_reg(uint8_t reg_addr_H)
{
	uint8_t data[2] = {0};
	size_t len = 2;
	uint16_t ret = 0;

	ESP_ERROR_CHECK(mpu6050_register_read(reg_addr_H, data, len));
	ret |= (data[0] << 8);
	ret |= data[1];

	return ret;
}

static int16_t read_x_accel()
{
	return read_16bit_reg(MPU6050_ACCEL_XOUT_H_REG);
}

static int16_t read_y_accel()
{
	return read_16bit_reg(MPU6050_ACCEL_YOUT_H_REG);
}

static int16_t read_z_accel()
{
	return read_16bit_reg(MPU6050_ACCEL_ZOUT_H_REG);
}

void get_accel_data(ACCEL_TYPE *ad)
{
	ad->x = read_x_accel();
	ad->y = read_y_accel();
	ad->z = read_z_accel();
}

void clear_accel_data(ACCEL_TYPE *ad)
{
	ad->x = 0;
	ad->y = 0;
	ad->z = 0;
}

static int16_t read_x_gyro()
{
	return read_16bit_reg(MPU6050_GYRO_XOUT_H_REG);
}

static int16_t read_y_gyro()
{
	return read_16bit_reg(MPU6050_GYRO_YOUT_H_REG);
}

static int16_t read_z_gyro()
{
	return read_16bit_reg(MPU6050_GYRO_ZOUT_H_REG);
}

void get_gyro_data(GYRO_TYPE *gd)
{
	gd->x = read_x_gyro();
	gd->y = read_y_gyro();
	gd->z = read_z_gyro();
}

void clear_gyro_data(GYRO_TYPE *gd)
{
	gd->x = 0;
	gd->y = 0;
	gd->z = 0;
}

void peek_reg(uint8_t reg, uint8_t *data)
{
	ESP_ERROR_CHECK(mpu6050_register_read(reg, data, 1));
}

void get_fifo_count(size_t *fifo_count)
{
	*fifo_count = (size_t)read_16bit_reg(MPU6050_FIFO_COUNT_H_REG);
}

static void clear_fifo_data()
{
	mpu6050_register_write_byte(MPU6050_USR_CTRL_REG,
				    (1 << MPU6050_FIFO_RESET_BIT)
				    | (1 << MPU6050_FIFO_EN_BIT));
}

void get_accel_and_gyro_from_fifo(MPU6050_TYPE *m)
{
	uint8_t data[BURST_LEN] = {0};

	mpu6050_register_read(MPU6050_FIFO_R_W_REG, data, BURST_LEN);
	clear_fifo_data();

	m->accl.x |= (data[0] << 8)  | data[1];
	m->accl.y |= (data[2] << 8)  | data[3];
	m->accl.z |= (data[4] << 8)  | data[5];

	m->gyro.x |= (data[6] << 8)  | data[7];
	m->gyro.y |= (data[8] << 8)  | data[9];
	m->gyro.z |= (data[10] << 8) | data[11];
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

	gpio_config_t io_conf = {};
	io_conf.intr_type = GPIO_INTR_POSEDGE;
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = 1;
	gpio_config(&io_conf);

	xTaskCreate(mpu6050_data_ready_handler,
		    "mpu6050_data_ready_handler",
		    2048,
		    (void *)m,
		    10,
		    &isr_handler);
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	gpio_isr_handler_add(GPIO_INPUT_IO_0, mpu6050_data_ready_isr, NULL);


	// mpu6050_check();
	// mpu6050_reset();
	// mpu6050_interrupt_config();
	// mpu6050_interrupt_enable();
	// mpu6050_set_clock(MPU6050_X_PLL_VAL);
	// mpu6050_set_gyroscope_range(MPU6050_500_FS_SEL_VAL);
	// mpu6050_set_accel_range(MPU6050_2G_AFS_SEL_VAL);
	// mpu6050_wakeup();
	// mpu6050_set_fifo();
}
