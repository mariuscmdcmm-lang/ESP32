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
esp_err_t lcd_init(i2c_port_t i2c_num);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char* str);
void lcd_write_char(char c);

#endif