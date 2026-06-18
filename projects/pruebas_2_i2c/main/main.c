#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

#define SDA_PIN 18
#define SCL_PIN 19

static const char *tag = "Main";

void find_address(i2c_master_bus_handle_t master_dev_handle);

void app_main(void)
{
    // INIT I2C
    i2c_master_bus_config_t i2c_master_bus_config = {
        .i2c_port = 0,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true};
    i2c_master_bus_handle_t i2c_master_bus_handle;
    i2c_new_master_bus(&i2c_master_bus_config, &i2c_master_bus_handle);

    i2c_device_config_t i2c_device_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = 0x60,
        .scl_speed_hz = 100000};
    i2c_master_dev_handle_t i2c_master_dev_handle;
    i2c_master_bus_add_device(i2c_master_bus_handle, &i2c_device_config, &i2c_master_dev_handle);

    /*
    uint16_t dac_value = 4095;
    uint8_t buffer[3];
    buffer[0] = 0x40;
    buffer[1] = (dac_value >> 4) & 0xFF;
    buffer[2] = (dac_value << 4) & 0xF0;

    i2c_master_transmit(i2c_master_dev_handle,  buffer, 3, -1);
    */
    find_address(i2c_master_bus_handle);
    while (1)
    {
        // i2c_master_transmit()
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void find_address(i2c_master_bus_handle_t master_bus_handle)
{
    ESP_LOGI(tag, "Searching.......");

    for (uint8_t address = 0; address < 127; address++)
    {
        esp_err_t addr_found = i2c_master_probe(master_bus_handle, address, -1);
        if (addr_found == ESP_OK)
        {
            ESP_LOGI(tag, "Address found at: 0x%02x", address);
        }
    }
}