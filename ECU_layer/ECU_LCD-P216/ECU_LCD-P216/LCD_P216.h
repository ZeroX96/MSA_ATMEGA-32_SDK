/*
 * LCD_P216.h
 *
 * Created: 19/09/2018 08:58:24
 *  Author: Mahmoud
 */ 


#ifndef LCD_P216_H_
#define LCD_P216_H_

#include "LCD_P216_CFG.h"
#include "hal_GPIO.h" //but edited & removed the unneeded functions using #if 0 ... #endif
#define F_CPU 1000000UL
#define F_CPU 1000000UL
#ifndef F_CPU
#define F_CPU 1000000UL
#warning "F_CPU Wasn't Defined so as a default it's now 1 MS"
#endif

#include <util/delay.h>
typedef enum
{
	NO_LCD_ERRORS,INVALID_LCD_PARAMS,
}
lcd_return_types; 

typedef enum
{
	CLEAR_SCREEN=0x01,						RETURN_HOME=0x02,
	SHIFT_CURSOR_LEFT=0x04,					SHIFT_CURSOR_RIGHT=0x06,	
	SHIFT_DISPLAY_RIGHT=0x05,				SHIFT_DISPLAY_LIFT=0X07,
	DISPLAY_OFF_CURSOR_OFF=0x08,			DISPLAY_OFF_CURSOR_ON=0x0A,
	DISPLAY_ON_CURSOR_OFF=0x0C,				
	DISPLAY_ON_CURSOR_BLINKING=0X0F,		DISPLAY_ON_CURSOR_BLINKING_ALSO=0x0E,//TEST THE BLINKING
	SHIFT_CURSOR_POS_TO_LEFT=0x10,			SHIFT_CURSOR_POS_TO_RIGHT=0x14,	
	SHIFT_THE_ENTIRE_DISPLAY_LEFT=0x18,		SHIFT_THE_ENTIRE_DISPLAY_RIGHT=0x1C,
	BEGIN_FROM_1ST_LINE=0x80,				BEGIN_FROM_2ND_LINE=0xC0,
	
	//the next are used with initialization only and the user can't use them or an error will ocour  
	INIT_8LINES_STEP_1=0x38,
	INIT_8LINES_STEP_2=0x0e,
	INIT_8LINES_STEP_3=0x01,
	
	INIT_4LINES_STEP_1=0x33,
	INIT_4LINES_STEP_2=0x32,
	INIT_4LINES_STEP_3=0x28,
}lcd_commands_t; 


typedef enum
{
	NOT_INITIATED,
	INITIATED,
	DEVICE_NOT_INITIATED,
}device_states_t;

typedef enum
{
	PORT_PIN_ZERO,PORT_PIN_ONE,PORT_PIN_TWO,PORT_PIN_THREE,PORT_PIN_FOUR,//more than this will give an error
}beginning_of_data_pins;

typedef struct  
{
	gpio_bases		obj_data_port;
	beginning_of_data_pins		obj_data_pins_beginning; //when 8bits data bus should be zero!!
	gpio_bases		obj_comm_port;
	gpio_pin_no		obj_RS_pin;
	gpio_pin_no		obj_RW_pin;
	gpio_pin_no		obj_E_pin;
	device_states_t	obj_device_state;
}g_lcd_t;

lcd_return_types lcd_init(	g_lcd_t* lcd_obj,	 gpio_bases data_port,	beginning_of_data_pins	data_pins_beginning,
							gpio_bases comm_port,gpio_pin_no RS_pin,    gpio_pin_no RW_pin,gpio_pin_no E_pin
						 );
						 
lcd_return_types lcd_data_out(g_lcd_t* lcd_obj,msa_u8 lcd_data);
lcd_return_types lcd_array_data_out(g_lcd_t* lcd_obj,msa_u8* lcd_array_data);
lcd_return_types lcd_comm_out(g_lcd_t* lcd_obj,lcd_commands_t lcd_command);
lcd_return_types lcd_goto_xy(g_lcd_t* lcd_obj,msa_u8 lcd_x,msa_u8 lcd_y);
lcd_return_types lcd_deinit(g_lcd_t* lcd_obj);

//lcd_return_types lcd_data_in(g_lcd_t* lcd_obj);//if tft

#endif /* LCD_P216_H_ */