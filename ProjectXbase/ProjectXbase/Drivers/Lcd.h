/*
 * Lcd.h
 *
 * Created: 3/20/2020 5:41:05 AM
 *  Author: Mahmo
 */ 


#ifndef LCD_H_
#define LCD_H_
#ifndef F_CPU
#define F_CPU 8000000UL
#endif //F_CPU
#include <util/delay.h>	//re-organize later
#include "Gpio.h"
#define PIN_RS (0U)
#define PIN_RW (1U)
#define PIN_E  (2U)
#define STARTING_BIT (4U)
#ifndef NULL
#define NULL ((void*)0)
#endif
/*
#define DATA_PORT	PORTA
#define CTRL_PORT	PORTA
*/
typedef enum {
	CLEAR_SCREEN				  = 0x01,	RETURN_HOME						= 0x02,
	SHIFT_CURSOR_LEFT			  = 0x04,	SHIFT_CURSOR_RIGHT				= 0x06,
	SHIFT_DISPLAY_RIGHT			  = 0x05,	SHIFT_DISPLAY_LIFT				= 0X07,
	DISPLAY_OFF_CURSOR_OFF		  = 0x08,	DISPLAY_OFF_CURSOR_ON			= 0x0A,
	DISPLAY_ON_CURSOR_OFF		  = 0x0C,
	DISPLAY_ON_CURSOR_BLINKING	  = 0X0F,	DISPLAY_ON_CURSOR_BLINKING_ALSO	= 0x0E,
	SHIFT_CURSOR_POS_TO_LEFT	  = 0x10,	SHIFT_CURSOR_POS_TO_RIGHT		= 0x14,
	SHIFT_THE_ENTIRE_DISPLAY_LEFT = 0x18,	SHIFT_THE_ENTIRE_DISPLAY_RIGHT	= 0x1C,
	BEGIN_FROM_1ST_LINE			  = 0x80,	BEGIN_FROM_2ND_LINE				= 0xC0,
	//Not a user command, it's for initialization
	INIT_4LINES_STEP_1 = 0x33,
	INIT_4LINES_STEP_2 = 0x32,
	INIT_4LINES_STEP_3 = 0x28,
}LcdCommandT;

typedef enum
{
	LCD_ERR_NO,
	LCD_ERR_INV_PARAMS,
}LcdRetT;
typedef enum  
{
	LCD_NOT_INITIATED,
	LCD_INITIATED,
}LcdStateT;

typedef struct  
{
	LcdStateT State;
	GpioBaseT DataBase;
	GpioBaseT CommBase;
}LcdT;

LcdRetT LcdInit(LcdT* Lcd);
LcdRetT LcdCharDataOut(LcdT* Lcd,uint8_t Data);
LcdRetT LcdArrDataOut(LcdT* Lcd,uint8_t* Data);
LcdRetT LcdCommOut(LcdT* Lcd,uint8_t Comm);
LcdRetT LcdGotoXnY(LcdT* Lcd,uint8_t X,uint8_t Y);
LcdRetT LcdDeInit(LcdT* Lcd);
#endif /* LCD_H_ */