
#include <stdio.h>
#include <stdint.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

static const char *TAG = "MPU6050";

#define I2C_MASTER_SCL_IO       CONFIG_I2C_MASTER_SCL
#define I2C_MASTER_SDA_IO       CONFIG_I2C_MASTER_SDA
#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_FREQ_HZ      CONFIG_I2C_MASTER_FREQUENCY
#define I2C_MASTER_TIMEOUT_MS   1000

#define MPU6050_SENSOR_ADDR         0x68
#define MPU6050_WHO_AM_I_REG_ADDR   0x75
#define MPU6050_PWR_MGMT_1_REG_ADDR 0x6B
#define MPU6050_ACCEL_XOUT_H        0x3B

static esp_err_t mpu6050_register_read(
    i2c_master_dev_handle_t dev_handle,
    uint8_t reg_addr,
    uint8_t *data,
    size_t len)
{
    return i2c_master_transmit_receive(
        dev_handle,
        &reg_addr,
        1,
        data,
        len,
        I2C_MASTER_TIMEOUT_MS
    );
}

static esp_err_t mpu6050_register_write_byte(
    i2c_master_dev_handle_t dev_handle,
    uint8_t reg_addr,
    uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};

    return i2c_master_transmit(
        dev_handle,
        write_buf,
        sizeof(write_buf),
        I2C_MASTER_TIMEOUT_MS
    );
}

void app_main(void)
{
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(
        i2c_new_master_bus(&bus_config, &bus_handle)
    );

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MPU6050_SENSOR_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(
            bus_handle,
            &dev_config,
            &dev_handle
        )
    );

    ESP_LOGI(TAG, "I2C baslatildi");

    uint8_t who_am_i = 0;

    ESP_ERROR_CHECK(
        mpu6050_register_read(
            dev_handle,
            MPU6050_WHO_AM_I_REG_ADDR,
            &who_am_i,
            1
        )
    );

    ESP_LOGI(TAG, "WHO_AM_I = 0x%02X", who_am_i);

    // Sensoru uyku modundan cikar
    ESP_ERROR_CHECK(
        mpu6050_register_write_byte(
            dev_handle,
            MPU6050_PWR_MGMT_1_REG_ADDR,
            0x00
        )
    );

    vTaskDelay(pdMS_TO_TICKS(100));

    while (1)
    {
        uint8_t accel_data[6];

        ESP_ERROR_CHECK(
            mpu6050_register_read(
                dev_handle,
                MPU6050_ACCEL_XOUT_H,
                accel_data,
                6
            )
        );

        int16_t accel_x = (int16_t)(
            ((uint16_t)accel_data[0] << 8) |
            accel_data[1]
        );

        int16_t accel_y = (int16_t)(
            ((uint16_t)accel_data[2] << 8) |
            accel_data[3]
        );

        int16_t accel_z = (int16_t)(
            ((uint16_t)accel_data[4] << 8) |
            accel_data[5]
        );

        ESP_LOGI(
            TAG,
            "Ivme: X=%d, Y=%d, Z=%d",
            accel_x,
            accel_y,
            accel_z
        );

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

