# Liquid-Cristal
This component allows you to easely control an LCD using I2C module.

## Methods:
`lcd_init();`.
`lcd_clear();`.
`lcd_set_cursor(int point, int line);`.
`lcd_print(char* text);`.

## Example of use:
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "liquid_cristal.h"

#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

uint8_t counter = 0;
char lcd_buffer[16];

static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK)
        return err;
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void app_main(void)
{
    // 1. Begin I2X Protocol
    ESP_ERROR_CHECK(i2c_master_init());

    // 2. Begin LCD 
    if (lcd_init(I2C_MASTER_NUM) == ESP_OK)
    {
        printf("LCD Inicializado correctamente!\n");
    }

    // 3. Write a text
    lcd_set_cursor(0, 0);
    lcd_print("ESP-IDF Example");

    lcd_set_cursor(0, 1);
    lcd_print("LCD 16x2 ready");

    vTaskDelay(pdMS_TO_TICKS(2000));
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Powered By");
    lcd_set_cursor(0, 1);
    lcd_print("Maryo");
    vTaskDelay(pdMS_TO_TICKS(2000));

    while (1)
    {
        // ------------------ USER BEGIN WHILE ----------------- //
        counter++;

        snprintf(lcd_buffer, sizeof(lcd_buffer), " Counter: %u", counter);

        lcd_clear();
        lcd_print(lcd_buffer);

        vTaskDelay(pdMS_TO_TICKS(1000));

        // ------------------- USER END WHILE ------------------ //
    }
}
```

*Powered by Maryo*