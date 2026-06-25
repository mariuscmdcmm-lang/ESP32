#include <stdio.h>
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *tag = "MAC_ADDRESS";

esp_err_t x_init_wifi(void);
esp_err_t leer_mac(void);
void callback_recepcion(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len);

void app_main(void)
{
    // inicializar nvs_flash para trabajar con wifi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // inicializar wifi basico y leer la mac
    ESP_ERROR_CHECK(x_init_wifi());
    leer_mac();

    // Registrar callback
    ESP_ERROR_CHECK(esp_now_register_recv_cb(callback_recepcion));

    while (1)
    {
        /** -------- WHILE BEGIN --------- */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Inicializar wifi y esp_now
esp_err_t x_init_wifi(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));

    esp_err_t status = esp_now_init();
    if (status != ESP_OK) {
        return status;
    }

    return ESP_OK;
}

// Leer la mac
esp_err_t leer_mac(void)
{
    uint8_t mac[6];
    esp_err_t ret = esp_read_mac(mac, ESP_MAC_WIFI_STA);
    if (ret == ESP_OK)
    {
        ESP_LOGI(tag, "Mac Address: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        ESP_LOGE(tag, "Error al leer la MAC");
    }

    return ESP_OK;
}

/**
 * @brief On Receive callback
 * @param recv_info - Received information
 * @param data - Byte 8 bits
 * @param data_len - Data length 
 * 
 * @warning esp_now_register_recv_cb() must be called before.
 * @note writed by maryo
 */
void callback_recepcion(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len) {
    ESP_LOGI("RECEPTOR", "Mensaje recibido de: " MACSTR, MAC2STR(recv_info->src_addr));
    ESP_LOGI("RECEPTOR", "Contenido: %.*s", data_len, data);
}