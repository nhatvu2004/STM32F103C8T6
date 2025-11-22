#include "ssd1306.h"
#include "font6x8.h"

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static HAL_StatusTypeDef ssd1306_WriteCommand(uint8_t cmd)
{
    uint8_t data[2];
    data[0] = 0x00;
    data[1] = cmd;

    return HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, data, 2, 100);
}

static HAL_StatusTypeDef ssd1306_WriteData(uint8_t *pData, uint16_t size)
{
    uint8_t control = 0x40;
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, &control, 1, 100);
    if (ret != HAL_OK) return ret;

    return HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, pData, size, 100);
}

HAL_StatusTypeDef SSD1306_Init(void)
{
    HAL_StatusTypeDef ret;
    HAL_Delay(100);

ssd1306_WriteCommand(0xAE);
ssd1306_WriteCommand(0x20);
ssd1306_WriteCommand(0x00);
ssd1306_WriteCommand(0xB0);
ssd1306_WriteCommand(0xC8);
ssd1306_WriteCommand(0x00);
ssd1306_WriteCommand(0x10);
ssd1306_WriteCommand(0x40);
ssd1306_WriteCommand(0x81);
ssd1306_WriteCommand(0x7F);
ssd1306_WriteCommand(0xA1);
ssd1306_WriteCommand(0xA6);
ssd1306_WriteCommand(0xA8);
ssd1306_WriteCommand(0x3F);
ssd1306_WriteCommand(0xA4);
ssd1306_WriteCommand(0xD3);
ssd1306_WriteCommand(0x00);
ssd1306_WriteCommand(0xD5);
ssd1306_WriteCommand(0x80);
ssd1306_WriteCommand(0xD9);
ssd1306_WriteCommand(0x22);
ssd1306_WriteCommand(0xDA);
ssd1306_WriteCommand(0x12);
ssd1306_WriteCommand(0xDB);
ssd1306_WriteCommand(0x20);
ssd1306_WriteCommand(0x8D);
ssd1306_WriteCommand(0x14);
ssd1306_WriteCommand(0xAF);


    SSD1306_Fill(0x00);
    return SSD1306_UpdateScreen();
}

void SSD1306_Fill(uint8_t color)
{
    for (uint32_t i = 0; i < sizeof(SSD1306_Buffer); i++)
        SSD1306_Buffer[i] = color ? 0xFF : 0x00;
}
extern uint8_t SSD1306_Buffer[];

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;

    if (color) 
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |=  (1 << (y % 8));
    else        
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}
void SSD1306_DrawChar(uint8_t x, uint8_t y, char c)
{
    const uint8_t *bitmap;

    switch(c) {
        case 'a': bitmap = font_a; break;
        case 'n': bitmap = font_n; break;
        case 'h': bitmap = font_h; break;
        case 'b': bitmap = font_b; break;
        case ' ': bitmap = Font6x8[0]; break;
        default:  bitmap = Font6x8[0]; break;
    }

    for (uint8_t i = 0; i < 6; i++) {
        uint8_t line = bitmap[i];
        for (uint8_t j = 0; j < 8; j++) {
            SSD1306_DrawPixel(x + i, y + j, (line >> j) & 1);
        }
    }
}
void SSD1306_DrawString(uint8_t x, uint8_t y, char *str)
{
    while (*str)
    {
        SSD1306_DrawChar(x, y, *str);
        x += 6;    // m?i ký t? r?ng 6 pixels
        str++;
    }
}

HAL_StatusTypeDef SSD1306_UpdateScreen(void)
{
    HAL_StatusTypeDef ret = HAL_OK;
    for (uint8_t page = 0; page < 8; page++)
    {
        ret |= ssd1306_WriteCommand(0xB0 + page);
        ret |= ssd1306_WriteCommand(0x00);
        ret |= ssd1306_WriteCommand(0x10);

        ret |= ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
    }
    return ret;
}
