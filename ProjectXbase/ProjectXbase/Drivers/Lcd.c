/*
 * Lcd.c
 *
 * Created: 3/20/2020 5:40:54 AM
 *  Author: Mahmo
 */ 
#include "Lcd.h"

LcdRetT LcdInit(LcdT* Lcd)
{
	if (NULL != Lcd)
	{
		//init the four-pins data-bus
		for (uint8_t i=0;i < 4; i++){
			GpioInitPin(Lcd->DataBase,STARTING_BIT+i,PIN_OUT);
		}
		GpioInitPin(Lcd->CommBase,PIN_RS,PIN_OUT);
		GpioInitPin(Lcd->CommBase,PIN_E,PIN_OUT);
		GpioInitPin(Lcd->CommBase,PIN_RW,PIN_OUT);
		LcdCommOut(Lcd,INIT_4LINES_STEP_1);
		LcdCommOut(Lcd,INIT_4LINES_STEP_2);
		LcdCommOut(Lcd,INIT_4LINES_STEP_3);
		GpioWritePin(Lcd->CommBase,PIN_RW,PIN_LOW);
		//the actual initialization is finished, the next are extra
		LcdCommOut(Lcd,DISPLAY_ON_CURSOR_BLINKING_ALSO);
		LcdCommOut(Lcd,CLEAR_SCREEN);
		//delay 2 ms
		LcdCommOut(Lcd,SHIFT_CURSOR_RIGHT);
		Lcd->State = LCD_INITIATED;
		//TODO: return values.
		return LCD_ERR_NO;
	}
	return LCD_ERR_INV_PARAMS;
}

LcdRetT LcdCharDataOut(LcdT* Lcd,uint8_t Data)
{
	if (NULL != Lcd){
		//high-nipple
		GpioWritePort(Lcd->DataBase,(GpioReadPort(Lcd->DataBase) & 0x0F) | (Data & 0xF0));
		GpioWritePin(Lcd->CommBase,PIN_RS,PIN_HIGH);
		//GpioWritePin(Lcd->CommBase,PIN_RW,PIN_LOW);
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_HIGH);
		_delay_us(1);
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_LOW);
		//low-nipple
		GpioWritePort(Lcd->DataBase,(GpioReadPort(Lcd->DataBase) & 0x0F) | (Data<< 4));
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_HIGH);
		_delay_us(1);
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_LOW);
		_delay_us(50);
	}
	return LCD_ERR_INV_PARAMS;
}

LcdRetT LcdArrDataOut(LcdT* Lcd,uint8_t* Data){
	if ((NULL != Lcd) && (NULL != Data)){
		uint8_t i = 0;
		while(Data[i] != 0){
			LcdCharDataOut(Lcd,Data[i]);
			i++;
		}
	}
	return LCD_ERR_INV_PARAMS;
}

LcdRetT LcdCommOut(LcdT* Lcd,uint8_t Comm)
{
	if (NULL != Lcd){
		//high-nipple
		GpioWritePort(Lcd->DataBase,(GpioReadPort(Lcd->DataBase) & 0x0F) | (Comm & 0xF0));
		GpioWritePin(Lcd->CommBase,PIN_RS,PIN_LOW);
		//GpioWritePin(Lcd->CommBase,PIN_RW,PIN_LOW);	
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_HIGH);
		_delay_us(1);
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_LOW);
		_delay_us(50);
		//low-nipple
		GpioWritePort(Lcd->DataBase,(GpioReadPort(Lcd->DataBase) & 0x0F) | (Comm<< 4));
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_HIGH);
		_delay_us(1);
		GpioWritePin(Lcd->CommBase,PIN_E,PIN_LOW);
		_delay_us(50);
		return LCD_ERR_NO;
	}
	return LCD_ERR_INV_PARAMS;
}

LcdRetT LcdGotoXnY(LcdT* Lcd,uint8_t X,uint8_t Y){
	if (NULL != Lcd){
		uint8_t FirstCharAdd[]= {0X80,0XC0,0X94,0XD4};
		LcdCommOut(Lcd,FirstCharAdd[Y-1] + X-1);
		_delay_us(100);
		return LCD_ERR_NO;
	}
	return LCD_ERR_INV_PARAMS;
}

LcdRetT LcdDeInit(LcdT* Lcd){
	if (NULL != Lcd)
	{
		LcdCommOut(Lcd,CLEAR_SCREEN);
		LcdCommOut(Lcd,DISPLAY_OFF_CURSOR_OFF);
		//De-init the four-pins data-bus
		for (uint8_t i=0;i < 4; i++){
			GpioInitPin(Lcd->DataBase,STARTING_BIT+i,PIN_IN);
		}
		GpioInitPin(Lcd->CommBase,PIN_RS,PIN_IN);
		GpioInitPin(Lcd->CommBase,PIN_E,PIN_IN);
		GpioInitPin(Lcd->CommBase,PIN_RW,PIN_IN);
		
		Lcd->State = LCD_NOT_INITIATED;
		//TODO: return values.
		return LCD_ERR_NO;
	}
	return LCD_ERR_INV_PARAMS;
}
