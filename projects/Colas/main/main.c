#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define led 2
#define button 4

void xTaskButton(void *pvParams);
void xTaskLed(void *pvParams);

QueueHandle_t queue1;
static const char * tag = "Main";

void app_main(void)
{
    // Create GPIO
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);

    gpio_reset_pin(button);
    gpio_set_direction(button, GPIO_MODE_INPUT);
    gpio_set_pull_mode(button, GPIO_PULLUP_ONLY);

    // Create Tasks
    xTaskCreate(xTaskButton, "Button task", 2048, NULL, 1, NULL);
    xTaskCreate(xTaskLed, "Led Task", 2048, NULL, 1, NULL);

    // create queues
    queue1 = xQueueCreate(10, sizeof(uint16_t));

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void xTaskButton(void *pvParams)
{   
    uint16_t event = 0;
    while (1)
    {
        if (gpio_get_level(button) == 0)
        {
            xQueueSend(queue1, &event, portMAX_DELAY);
            while (gpio_get_level(button) == 0){
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void xTaskLed(void *pvParams)
{
    uint16_t received;
    uint8_t sum = 0;
    while (1)
    {
        if (xQueueReceive(queue1, &received, portMAX_DELAY)){
            sum += 1;
            ESP_LOGI(tag, "Sum: %u", sum);
            gpio_set_level(led, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            gpio_set_level(led, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}