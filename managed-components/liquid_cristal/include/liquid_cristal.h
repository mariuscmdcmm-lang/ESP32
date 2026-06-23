#ifndef LIQUID_CRISTAL_H
#define LIQUID_CRISTAL_H

#include "esp_err.h"
#include "driver/i2c.h"

#define LCD_ADDR                    0x27 // Try 0x3F if your module requires

// PCF8574T i2c byte list
#define LCD_CLEARDISPLAY            0x01
#define LCD_RETURNHOME              0x02
#define LCD_ENTRYMODESET            0x04
#define LCD_DISPLAYCONTROL          0x08
#define LCD_CURSORSHIFT             0x10
#define LCD_FUNCTIONSET             0x20
#define LCD_SETCGRAMADDR            0x40
#define LCD_SETDDRAMADDR            0x80

// Diaplay control flags
#define LCD_DISPLAYON               0x04
#define LCD_CURSOROFF               0x00
#define LCD_BLINKOFF                0x00

// PCF8574 - light control
#define LCD_BACKLIGHT               0x08
#define LCD_NOBACKLIGHT             0x00
#define En                          0x04  // Enable bit (Bit 2)
#define Rw                          0x02  // Read/Write bit (Bit 1)
#define Rs                          0x01  // Register Select bit (Bit 0)

// Contol funcions

/**
 * @brief Select an I2C port number.
 * @note You can use the same definitions from driver/i2c.h
 * 
 * @param i2c_num I2C port number
 * 
 * @return
 *      - ESP_OK 
 */
esp_err_t lcd_init(i2c_port_t i2c_num);

/**
 * @brief Clears the display
 */
void lcd_clear(void);

/**
 * @brief Set cursor position
 * 
 * @param col Set the column number
 * @param row Set the Row number
 */
void lcd_set_cursor(uint8_t col, uint8_t row);

/**
 * @brief Print a text 
 * 
 * @param str - Text to print
 */
void lcd_print(const char* str);

/**
 * @brief Write a single char
 * 
 * @param c Caracter to print
 */
void lcd_write_char(char c);

#endif