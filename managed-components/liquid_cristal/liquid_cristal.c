/*
* POWERED BY MARYO
* AMPFUSION  
* 15.06.2026
* v1.0.0
*/

#include "liquid_cristal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"
#include <string.h>

static i2c_port_t _i2c_num;
static uint8_t _backlight_val = LCD_BACKLIGHT;

// private funcions to send raw bytes to PCF8574T
static esp_err_t pcf8574_write_byte(uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, data | _backlight_val, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(_i2c_num, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Generates the enable pulse
static void lcd_pulse_enable(uint8_t data) {
    pcf8574_write_byte(data | En);
    esp_rom_delay_us(1);
    pcf8574_write_byte(data & ~En);
    esp_rom_delay_us(50);
}

static void lcd_send(uint8_t value, uint8_t mode) {
    uint8_t high_nibble = value & 0xF0;
    uint8_t low_nibble = (value << 4) & 0xF0;
    
    lcd_pulse_enable(high_nibble | mode);
    lcd_pulse_enable(low_nibble | mode);
}

static void lcd_send_cmd(uint8_t cmd) {
    lcd_send(cmd, 0);
}

// send a char
void lcd_write_char(char c) {
    lcd_send(c, Rs);
}

// Enabling display according to HD44780 datasheet sequence
esp_err_t lcd_init(i2c_port_t i2c_num) { // --------------------------- USER METHOD -------------------------- //
    _i2c_num = i2c_num;
    
    vTaskDelay(pdMS_TO_TICKS(50));
    
    lcd_pulse_enable(0x30);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_pulse_enable(0x30);
    esp_rom_delay_us(1);
    lcd_pulse_enable(0x30);
    
    lcd_pulse_enable(0x20); // Set 4 bits mode

    // display config
    lcd_send_cmd(LCD_FUNCTIONSET | 0x08); 
    lcd_send_cmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    lcd_clear();
    lcd_send_cmd(LCD_ENTRYMODESET | 0x02); // cursor - autoincrement
    
    return ESP_OK;
}

/*
* --------------------------------------------------
* USER METHODS BEGIN
* --------------------------------------------------
*/

// CLEAN THE DISPLAY
void lcd_clear(void) {
    lcd_send_cmd(LCD_CLEARDISPLAY);
    vTaskDelay(pdMS_TO_TICKS(2)); // Este comando es lento, requiere tiempo extra
}

// SET CURSOR POSITION A = COL ; B = ROW
void lcd_set_cursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40 };
    if (row > 1) row = 1; 
    lcd_send_cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// SEND A STRINGs
void lcd_print(const char* str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}

/*
* --------------------------------------------------
* USER METHODS END
* --------------------------------------------------
*/