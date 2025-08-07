#pragma once
#include <Arduino.h>
#include <SPI.h>
#define LCD_WIDTH   320 //LCD width
#define LCD_HEIGHT 172 //LCD height

// Define some basic colors
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_BLUE  0x001F

#define SPIFreq                        80000000
#define EXAMPLE_PIN_NUM_MISO           5
#define EXAMPLE_PIN_NUM_MOSI           6
#define EXAMPLE_PIN_NUM_SCLK           7
#define EXAMPLE_PIN_NUM_LCD_CS         14
#define EXAMPLE_PIN_NUM_LCD_DC         15
#define EXAMPLE_PIN_NUM_LCD_RST        21
#define EXAMPLE_PIN_NUM_BK_LIGHT       22
#define Frequency       1000     
#define Resolution      10       

#define VERTICAL   0
#define HORIZONTAL 1

void LCD_SetCursor(uint16_t x1, uint16_t y1, uint16_t x2,uint16_t y2);

void LCD_Init(void);
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t  Yend);
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint16_t* color);

void Backlight_Init(void);
void Set_Backlight(uint8_t Light);

// Status message functions
void LCD_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color);
void LCD_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color);
void LCD_Clear(uint16_t color);
void LCD_ShowStatusMessage(const char* message);
