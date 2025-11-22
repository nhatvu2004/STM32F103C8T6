#include "LCD.h"
void LCD_Write_Byte(LCD *mLCD ,uint8_t data,uint8_t mode){// write 1 byte{// write 1 byte

	if(mode == 0x01){
		HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->RS,1);
	//data
	}else if(mode == 0x00){
		HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->RS,0);
	}
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D0, (data & 0x01) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D1, (data & 0x02) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D2, (data & 0x04) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D3, (data & 0x08) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D4, (data & 0x10) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D5, (data & 0x20) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D6, (data & 0x40) ? 1 : 0);
	HAL_GPIO_WritePin(mLCD->PORT_DATA, mLCD->D7, (data & 0x80) ? 1 : 0);
	//cmd 
	
	HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->En, 0);
	HAL_Delay(1);
	HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->En, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->En, 0);
	HAL_Delay(1);
}

void LCD_Write_data(LCD *mLCD,uint8_t data){
	LCD_Write_Byte(mLCD, data, 1);
}
void LCD_Write_cmd(LCD *mLCD,uint8_t data){
	LCD_Write_Byte(mLCD, data, 0);
}

void LCD_Init(LCD *mLCD){
	HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->RW, 0);
	HAL_Delay(50);
	LCD_Write_cmd(mLCD, 0x38); //Function set 8bit interface, 2line 5*7 pixels
	HAL_Delay(5);
	LCD_Write_cmd(mLCD, 0x0C);
	HAL_Delay(5);
	LCD_Write_cmd(mLCD, 0x06); //kieu hien chu tu trai sang phai
	HAL_Delay(5);
}

void LCD_Print(LCD *mLCD, char *str){
	while(*str){
		LCD_Write_data( mLCD ,*str++);
	}

}