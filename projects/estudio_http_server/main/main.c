#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "driver/gpio.h"

#define PIN_LED GPIO_NUM_2
static bool led_state = false;

// Esta es la web que el ESP32 enviará al navegador
const char *html_page = 
    "<html><body>"
    "<h1>Control ESP32</h1>"
    "<button onclick=\"fetch('/toggle')\">Toggle LED</button>"
    "</body></html>";

// Manejador del botón: hace el toggle y responde
static esp_err_t toggle_handler(httpd_req_t *req) {
    led_state = !led_state;
    gpio_set_level(PIN_LED, led_state);
    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Manejador de la página principal
static esp_err_t root_handler(httpd_req_t *req) {
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Estructuras para definir las rutas
httpd_uri_t uri_root = { .uri = "/", .method = HTTP_GET, .handler = root_handler };
httpd_uri_t uri_toggle = { .uri = "/toggle", .method = HTTP_GET, .handler = toggle_handler };

void start_webserver() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    httpd_start(&server, &config);
    httpd_register_uri_handler(server, &uri_root);
    httpd_register_uri_handler(server, &uri_toggle);
}

void app_main() {
    nvs_flash_init();
    gpio_reset_pin(PIN_LED);
    gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);

    // Configuración del SoftAP (Crea su propia red WiFi)
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    wifi_config_t wifi_config = {
        .ap = { .ssid = "ESP32_SERVER", .password = "12345678", .channel = 1, .max_connection = 4, .authmode = WIFI_AUTH_WPA_WPA2_PSK }
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    start_webserver();
}