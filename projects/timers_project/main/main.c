#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define led 2

static const char * tag = "Main";

void app_main(void)
{
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);

    bool ledState = 0;

    while (1)
    {
        ledState = !ledState;
        gpio_set_level(led, ledState);
        printf("Mensaje debug");
        ESP_LOGI(tag, "Mensaje del main");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}