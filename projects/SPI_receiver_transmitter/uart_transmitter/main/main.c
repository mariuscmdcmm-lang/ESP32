#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"

#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_CLK  18
#define PIN_CS   15

// Handle global para poder usarlo en otras funciones
spi_device_handle_t spi_handle;

esp_err_t x_init_spi(void);

void app_main(void)
{
    x_init_spi();
    uint8_t data_to_send = 0x0A;
    spi_transaction_t trans = {
        .length = 8,                // Número de bits a enviar
        .tx_buffer = &data_to_send  // Puntero al dato
    };

    while (1)
    {
        spi_device_transmit(spi_handle, &trans);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t x_init_spi(void)
{
    // 1. Configuración del Bus (físico)
    spi_bus_config_t bus_cfg = {
        .miso_io_num = PIN_MISO,
        .mosi_io_num = PIN_MOSI,
        .sclk_io_num = PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092
    };

    // 2. Configuración del Dispositivo (protocolo)
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 1000000,   // 1 MHz
        .mode = 0,                   // SPI Mode 0
        .spics_io_num = PIN_CS,      // Pin de selección
        .queue_size = 1              // Tamaño de cola de transacciones
    };

    // Inicializar el bus SPI
    ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_CH_AUTO));

    // Añadir el dispositivo al bus
    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &dev_cfg, &spi_handle));

    return ESP_OK;
}