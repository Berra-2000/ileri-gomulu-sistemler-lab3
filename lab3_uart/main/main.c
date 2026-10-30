#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"

#define TX_UART UART_NUM_2
#define RX_UART UART_NUM_1
#define TX_PIN 17
#define RX_PIN 18

static const char *TAG = "LAB3_BAUD";

void app_main(void)
{
    uart_config_t tx_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_config_t rx_config = tx_config;
    rx_config.baud_rate = 9600;

    ESP_ERROR_CHECK(uart_driver_install(TX_UART, 1024, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(TX_UART, &tx_config));
    ESP_ERROR_CHECK(uart_set_pin(TX_UART, TX_PIN, UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_driver_install(RX_UART, 1024, 1024, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(RX_UART, &rx_config));
    ESP_ERROR_CHECK(uart_set_pin(RX_UART, UART_PIN_NO_CHANGE, RX_PIN,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    const char *message = "Merhaba Lab 3 UART!";
    uint8_t received[128];

    while (1) {
        uart_flush_input(RX_UART);
        uart_write_bytes(TX_UART, message, strlen(message));
        uart_wait_tx_done(TX_UART, pdMS_TO_TICKS(1000));

        int len = uart_read_bytes(RX_UART, received,
                                 sizeof(received),
                                 pdMS_TO_TICKS(300));

        ESP_LOGI(TAG, "Gonderilen: %s", message);
        ESP_LOGI(TAG, "Alinan bayt sayisi: %d", len);
        ESP_LOG_BUFFER_HEX(TAG, received, len);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
