#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "stm32f1xx_hal.h"

#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  64

#define SSD1306_I2C_ADDR   (0x3C << 1)

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef SSD1306_Init(void);
void SSD1306_Fill(uint8_t color);
HAL_StatusTypeDef SSD1306_UpdateScreen(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void SSD1306_WriteChar(char ch);
void SSD1306_WriteString(char *str);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void SSD1306_DrawChar(uint8_t x, uint8_t y, char c);
void SSD1306_DrawString(uint8_t x, uint8_t y, char *str);

#endif
