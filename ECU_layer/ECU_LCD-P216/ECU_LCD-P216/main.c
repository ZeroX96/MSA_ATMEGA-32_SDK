/*
 * ECU_LCD-P216.c
 *
 * Created: 19/09/2018 08:56:10
 * Author : Mahmoud
 *note the name "LCD_P216" stands for lcd-parallel 2*16 so if used another will notice the difference.
 */ 

#include <avr/io.h>
#include "LCD_P216.h"
/*

lcd_return_types lcd_init (g_lcd_t* lcd_obj,gpio_bases data_port,beginning_of_data_pins data_pins_beginning,gpio_bases comm_port,
						   gpio_pin_no RS_pin,gpio_pin_no RW_pin,gpio_pin_no E_pin);

lcd_return_types lcd_data_out(g_lcd_t* lcd_obj,msa_u8 lcd_data);
lcd_return_types lcd_array_data_out(g_lcd_t* lcd_obj,msa_u8* lcd_array_data);
lcd_return_types lcd_comm_out(g_lcd_t* lcd_obj,lcd_commands_t lcd_command);
lcd_return_types lcd_goto_xy(g_lcd_t* lcd_obj,msa_u8 lcd_x,msa_u8 lcd_y);
lcd_return_types lcd_deinit(g_lcd_t* lcd_obj);
*/
int main(void)
{
	g_lcd_t lcd_obj_1;
	lcd_init(&lcd_obj_1,BASE_D,PORT_PIN_FOUR,BASE_C,PIN_TWO,PIN_THREE,PIN_FOUR);
	msa_u8 lcd_array_data[13]="Mahmoud Saad";
    while (1) 
    {
	    lcd_data_out(&lcd_obj_1,'M');
		_delay_ms(500);
	    lcd_data_out(&lcd_obj_1,'S');
		_delay_ms(500);
	    lcd_data_out(&lcd_obj_1,'A');
		_delay_ms(500);
		lcd_comm_out(&lcd_obj_1,CLEAR_SCREEN);
		_delay_ms(500);		
		lcd_array_data_out(&lcd_obj_1,lcd_array_data);
		_delay_ms(500);
		lcd_comm_out(&lcd_obj_1,CLEAR_SCREEN);
		_delay_ms(500);
	}
}

