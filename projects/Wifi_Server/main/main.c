#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

#define LED_PIN 2
#define LED_2 22
#define BUTTON 4

static const char * tag = "Main";

adc_oneshot_unit_handle_t adc1_handle;

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

esp_err_t root_get_handler(httpd_req_t *req);
esp_err_t toggle_post_handler(httpd_req_t *req);
esp_err_t led_2_toggle_handler(httpd_req_t *req);
esp_err_t get_estado_handler(httpd_req_t *req);
esp_err_t get_pot_value(httpd_req_t *req);
esp_err_t set_valor_handler(httpd_req_t *req);

void app_main(void)
{
    // Init ADC
    adc_oneshot_unit_init_cfg_t init_config1 = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    // Configuración del canal (ejemplo en el pin GPIO 34)
    adc_oneshot_chan_cfg_t config_adc = { .bitwidth = ADC_BITWIDTH_12, .atten = ADC_ATTEN_DB_12 };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config_adc);

    // Init NVS Flash
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Init TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // INIT WIFI
    wifi_init_config_t x_wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&x_wifi_config));

    // Create Access Point
    esp_netif_create_default_wifi_ap();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // Wifi config
    wifi_config_t wifi_cfg = {
        .ap = {
            .ssid = "Esp32 Maryo",
            .password = "1882402166",
            .ssid_len = 0,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    esp_wifi_set_config(WIFI_IF_AP, &wifi_cfg);

    // Start WiFi
    ESP_ERROR_CHECK(esp_wifi_start());

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t uri_root = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_get_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &uri_root);

        httpd_uri_t uri_toggle = {
            .uri       = "/toggle",
            .method    = HTTP_POST, // ¡Importante! Aquí usamos POST
            .handler   = toggle_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_toggle);

        httpd_uri_t led_2_toggle = {
            .uri =      "/led_2_toggle",
            .method =   HTTP_POST,
            .handler = led_2_toggle_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &led_2_toggle);

        httpd_uri_t button_state = {
            .uri        = "/estadoBoton",
            .method     = HTTP_GET,
            .handler    = get_estado_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &button_state);

        httpd_uri_t getPot = {
            .uri        = "/pot",
            .method     = HTTP_GET,
            .handler    = get_pot_value,
            .user_ctx   = NULL
        };
        httpd_register_uri_handler(server, &getPot);

        httpd_uri_t getSliderValue = {
            .uri = "/set_valor",
            .method = HTTP_POST,
            .handler = set_valor_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &getSliderValue);
    }

    // Init GPIO
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_reset_pin(LED_2);
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BUTTON);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);

    while (1)
    {
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t root_get_handler(httpd_req_t *req)
{
    size_t html_len = index_html_end - index_html_start;
    httpd_resp_send(req, (const char*)index_html_start, html_len);

    return ESP_OK;
}

esp_err_t toggle_post_handler(httpd_req_t *req)
{
    static bool ledState = false;
    ledState = !ledState;

    gpio_set_level(LED_PIN, ledState);

    const char *resp = "Estado Toggle!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

esp_err_t led_2_toggle_handler(httpd_req_t *req) 
{
    static bool led_2_state = false;
    led_2_state = !led_2_state;
    gpio_set_level(LED_2, led_2_state);

    const char *resp = led_2_state ? "ON" : "Off";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

esp_err_t get_estado_handler(httpd_req_t *req)
{
    int level = gpio_get_level(BUTTON);
    const char *resp = (level == 0) ? "On" : "Off";

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

esp_err_t get_pot_value(httpd_req_t *req)
{
    int raw_data;
    adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &raw_data);

    char resp[10];
    sprintf(resp, "%d", raw_data);

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

esp_err_t set_valor_handler(httpd_req_t *req)
{
    char buf[10];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    
    if (ret <= 0) return ESP_FAIL; 

    buf[ret] = '\0';
    int valor = atoi(buf);

    ESP_LOGI("WEB_INPUT", "Valor recibido desde la web: %d", valor);

    httpd_resp_send(req, "Valor recibido", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}