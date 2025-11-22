#ifndef LCD_H

#define LCD_H // neu LCD.h chua duoc dinh nghia thi define

#endif
#include <stdint.h>
#include <stm32f1xx_hal.h>
/*

RS & RW &En: PORT_R
D0-D7: PORT_DATA
*/
typedef struct{
	GPIO_TypeDef *PORT_DATA;
	GPIO_TypeDef *PORT_R;
	uint16_t D0;
	uint16_t D1;
	uint16_t D2;
	uint16_t D3;
	uint16_t D4;
	uint16_t D5;
	uint16_t D6;
	uint16_t D7;
	uint16_t RS;
	uint16_t RW;
	uint16_t En;

}LCD;

extern LCD myLCD;

void LCD_Init(LCD *mLCD);
void LCD_Print(LCD *mLCD,char *str);
void LCD_Write_Byte(LCD *mLCD ,uint8_t data,uint8_t mode);// write 1 byte
void LCD_Write_data(LCD *mLCD,uint8_t data);
void LCD_Write_cmd(LCD *mLCD, uint8_t data);
