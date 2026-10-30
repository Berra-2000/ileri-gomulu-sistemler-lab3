#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_err.h"

#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_CLK  18

static const char *TAG = "LAB3_SPI";

static void spi_test(int mode)
{
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 100000,
        .mode = mode,
        .spics_io_num = -1,
        .queue_size = 1
    };

    spi_device_handle_t device;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &device));

    uint8_t tx[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t rx[4] = {0};

    spi_transaction_t transaction = {
        .length = 32,
        .tx_buffer = tx,
        .rx_buffer = rx
    };

    ESP_ERROR_CHECK(spi_device_polling_transmit(device, &transaction));

    ESP_LOGI(TAG, "SPI MODE %d", mode);
    ESP_LOG_BUFFER_HEX(TAG, tx, 4);
    ESP_LOG_BUFFER_HEX(TAG, rx, 4);

    if (memcmp(tx, rx, 4) == 0) {
        ESP_LOGI(TAG, "SONUC: BASARILI");
    } else {
        ESP_LOGW(TAG, "SONUC: VERI FARKLI");
    }

    ESP_ERROR_CHECK(spi_bus_remove_device(device));
}

void app_main(void)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI,
        .miso_io_num = SPI_MISO,
        .sclk_io_num = SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 64
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg,
                                       SPI_DMA_DISABLED));

    spi_test(0);
    spi_test(3);

    ESP_ERROR_CHECK(spi_bus_free(SPI2_HOST));
}
