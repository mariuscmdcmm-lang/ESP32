#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"

#define CS 15
#define MISO 19
#define MOSI 23
#define CLK 18
#define LED 33

esp_err_t x_init_SPI(void);
void spi_slave_task(void *pvParams);

bool ledState = 0;

/**       MAIN         */

void app_main(void)
{
    x_init_SPI();
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    xTaskCreate(spi_slave_task, "spi_task", 2048, NULL, 5, NULL);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t x_init_SPI(void) {
    spi_bus_config_t spi_bus = {
        .mosi_io_num = MOSI,
        .miso_io_num = MISO,
        .sclk_io_num = CLK,
        .quadwp_io_num = -1,    // Obligatorio definir estos como -1
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092
    };

    spi_slave_interface_config_t slave_interface = {
        .spics_io_num = CS,
        .mode = 0,
        .queue_size = 3 // Debes definir el tamaño de la cola
    };

    // Cambiado: 0 en lugar de SPI_DMA_DISABLED
    return spi_slave_initialize(SPI3_HOST, &spi_bus, &slave_interface, 0);
}

void spi_slave_task(void *pvParameters) {
    uint8_t rx_buffer[32] = {0};
    uint8_t tx_buffer[32] = {0};

    while (1) {
        spi_slave_transaction_t t = {
            .length = 32 * 8,
            .tx_buffer = tx_buffer,
            .rx_buffer = rx_buffer
        };

        // 1. Ponemos el buffer a la espera
        spi_slave_queue_trans(SPI3_HOST, &t, portMAX_DELAY);

        // 2. Esperamos hasta que la transacción se complete (bloqueante)
        spi_slave_transaction_t *out_t;
        spi_slave_get_trans_result(SPI3_HOST, &out_t, portMAX_DELAY);

        if (rx_buffer[0] == 0x0A)
        {
            ledState = !ledState;
            gpio_set_level(LED, ledState);
        }

        // 3. ¡Ahora ya puedes procesar los datos recibidos!
        printf("Datos recibidos: %02x\n", rx_buffer[0]);
    }
}