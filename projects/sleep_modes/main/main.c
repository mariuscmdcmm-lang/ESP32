#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define SLEEP_SECONDS(s) ((s) * 1000000)
#define button 4

RTC_DATA_ATTR int counter = 0;
static const char * tag = "Main";

void app_main(void)
{
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    // 1. Verificar causa
    if (cause == ESP_SLEEP_WAKEUP_EXT0) {
        printf("Despertado por BOTON (GPIO %d)\n", button);
    } else if (cause == ESP_SLEEP_WAKEUP_TIMER) {
        printf("Despertado por TIMER\n");
    } else {
        printf("Inicio por RESET (Power-on)\n");
    }

    ESP_LOGI(tag, "Counter: %i", counter);
    counter += 1;
    
    // 2. Configurar el pin correctamente
    // Si usas 1 como nivel de despertar, necesitas un pull-down externo o interno
    gpio_reset_pin(button);
    gpio_set_direction(button, GPIO_MODE_INPUT);
    gpio_pulldown_en(button); // Asegura que esté en 0 mientras no lo tocas
    gpio_pullup_dis(button);

    // 3. Configurar despertares
    esp_sleep_enable_timer_wakeup(SLEEP_SECONDS(10));
    esp_sleep_enable_ext0_wakeup(button, 1); // 1 = Nivel ALTO
    
    printf("Entrando en Deep Sleep...\n");
    esp_deep_sleep_start();
}