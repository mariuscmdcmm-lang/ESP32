#include "esp_log.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "BLE_TEST";

// Callback que se ejecuta cuando el móvil escribe en la característica
static int device_write_callback(uint16_t conn_handle, uint16_t attr_handle, 
                                 struct ble_gatt_access_ctxt *ctxt, void *arg) {
    
    // Aquí recibimos los datos
    uint8_t *data = ctxt->om->om_data;
    uint16_t len = ctxt->om->om_len;

    ESP_LOGI(TAG, "Dato recibido: %.*s (Tamaño: %d)", len, data, len);
    
    return 0;
}

// Definición de la estructura de servicios y características
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(0x180D), // UUID genérico de ejemplo
     .characteristics = (struct ble_gatt_chr_def[]) {
        {.uuid = BLE_UUID16_DECLARE(0x2A37), // UUID de la característica
         .access_cb = device_write_callback, // Nuestra función callback
         .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_READ},
        {0} // Fin de características
    }},
    {0} // Fin de servicios
};

void app_main(void) {
    // Inicializar NimBLE
    nimble_port_init();
    ble_svc_gap_init();
    ble_svc_gatt_init();
    ble_gatts_count_cfg(gatt_svcs);
    ble_gatts_add_svcs(gatt_svcs);
    
    nimble_port_freertos_init(NULL);
    ESP_LOGI(TAG, "BLE iniciado, esperando conexión...");
}