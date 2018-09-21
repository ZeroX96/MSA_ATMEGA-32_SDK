/*
 * LCD_P216.c
 *
 * Created: 19/09/2018 08:58:08
 *  Author: Mahmoud
 */ 
#include "LCD_P216.h"
/*
funcX(){
	//use in all the functions
	#if NO_OF_DATA_LINES == 4
	//4bits mode
	PORTA=0xff;
	#else 
	//8bits mode
	PORTA=0x55;
	#endif
}
*/
#define PORTX (*((volatile msa_u8*)lcd_obj->obj_data_port+PORT_OFFSET))


lcd_return_types lcd_init(	g_lcd_t* lcd_obj,	 gpio_bases data_port,	beginning_of_data_pins	data_pins_beginning,
							gpio_bases comm_port,gpio_pin_no RS_pin,    gpio_pin_no RW_pin,gpio_pin_no E_pin
						 )
{
	lcd_return_types ret_val=NO_LCD_ERRORS;
	if ( (lcd_obj != NULL)		&& ( (data_port >= BASE_D) && (data_port <= BASE_A)) && ( (comm_port >= BASE_D) && (comm_port <= BASE_A))&&
		 ( (RS_pin >= PIN_ZERO) && (RS_pin <= PIN_SEVEN) ) && ( (RW_pin >= PIN_ZERO) && (RW_pin <= PIN_SEVEN) ) && ( (E_pin >= PIN_ZERO) && (E_pin <= PIN_SEVEN) )
	   )
	{
		
		#if NO_OF_DATA_LINES == 4
		//4bits mode
		if ( (data_pins_beginning < PORT_PIN_ZERO) || (data_pins_beginning > PORT_PIN_FOUR) )
		{
			ret_val=INVALID_LCD_PARAMS;
			return ret_val;
		}
		#elif NO_OF_DATA_LINES == 8
		//8bits mode
		if (data_pins_beginning != PORT_PIN_ZERO)
		{
			ret_val=INVALID_LCD_PARAMS;
			return ret_val;
		}
		#else
		#error "invalid number of data lines.it can be 4 or 8 otherwise error"
		#endif
		
		//storing the configurations in the structure
		lcd_obj->obj_data_port=data_port;
		lcd_obj->obj_comm_port=comm_port;
		lcd_obj->obj_E_pin=E_pin;
		lcd_obj->obj_RS_pin=RS_pin;
		lcd_obj->obj_RW_pin=RW_pin;
		lcd_obj->obj_data_pins_beginning=data_pins_beginning;
		lcd_obj->obj_device_state=INITIATED;
		
		//actual initialization
		set_pin_output((gpio_bases *)lcd_obj->obj_comm_port,E_pin);
		set_pin_output((gpio_bases *)lcd_obj->obj_comm_port,RS_pin);
		set_pin_output((gpio_bases *)lcd_obj->obj_comm_port,RW_pin);
		
		#if LCD_INIT_IS_FIRST_FUNC == 1
		_delay_ms(20);
		#endif
		
		/*NOTE
		* RS == 0 >>COMMAND				RS == 1  >>DATA
		* RW == 0 >>WRITING TO LCD		RW == 1 >>READING FROM LCD
		* E  ==>>a high to low pulse must be applied so the lcd latches in the data on the data bus
		*/
		set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,RS_pin);
		set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,RW_pin);
		
		#if NO_OF_DATA_LINES == 4
		//4bits mode
		for (msa_u8 i=0;i<4;i++)
		{
			set_pin_output((gpio_bases *)lcd_obj->obj_data_port,data_pins_beginning+i);
		}
		lcd_comm_out(lcd_obj,INIT_4LINES_STEP_1);
		lcd_comm_out(lcd_obj,INIT_4LINES_STEP_2);
		lcd_comm_out(lcd_obj,INIT_4LINES_STEP_3);
		
		#elif NO_OF_DATA_LINES == 8
		//8bits mode
		set_port_output((gpio_bases *)lcd_obj->obj_data_port);
		lcd_comm_out(lcd_obj,INIT_8LINES_STEP_1);
		lcd_comm_out(lcd_obj,INIT_8LINES_STEP_2);
		lcd_comm_out(lcd_obj,INIT_8LINES_STEP_3);
		#else
		#error "invalid number of data lines.it can be 4 or 8 otherwise error"
		#endif
	} 
	else
	{
		ret_val=INVALID_LCD_PARAMS;
	}
	
	return ret_val;
}


lcd_return_types lcd_comm_out(g_lcd_t* lcd_obj,lcd_commands_t lcd_command)  //optimize the repeation of the delay func
{
	lcd_return_types ret_val=NO_LCD_ERRORS;
	if( (lcd_obj != NULL) && ( (lcd_command >=0x01) && (lcd_command <= 0xc0) ) )
	{
		if (lcd_obj->obj_device_state != INITIATED)
		{
			ret_val=DEVICE_NOT_INITIATED;
			return ret_val;
		}
		set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_RS_pin);
			#if NO_OF_DATA_LINES == 4
			//4bits mode>>>many if statements i know but this is to reduce the repetition of the high to low pulse and the delay and so on>>>then it's for optimization 
			if (lcd_obj->obj_data_pins_beginning == PORT_PIN_FOUR)
			{
				//the high nipple
				PORTX= (PORTX & 0x0f) | (lcd_command & 0xf0);
			} 
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_THREE)
			{
				//the high nipple
				PORTX= (PORTX & 0x87) | ( (lcd_command & 0xf0) >> 1 );
			}
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_TWO)
			{
				//the high nipple
				PORTX= (PORTX & 0xc3) | ( (lcd_command & 0xf0) >> 2 );
			}
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_ONE)
			{
				//the high nipple
				PORTX= (PORTX & 0xe1) | ( (lcd_command & 0xf0) >> 3 );
			}
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_ZERO)
			{
				//the high nipple
				PORTX= (PORTX & 0xf0) | ( lcd_command >> 4);
			}
	
			//a high to low pulse to make tthe lcd latch the data in
			set_pin_high((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
			_delay_us(1);
			set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
	
	
			_delay_us(20);
	
	
			if (lcd_obj->obj_data_pins_beginning == PORT_PIN_FOUR)
			{
				//the low nipple
				PORTX= ( (PORTX & 0x0f) | (lcd_command<<4) );
			}
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_THREE)
			{
				//the low nipple
				PORTX= ( (PORTX & 0x87) | ((lcd_command & 0x0f)<<3) );
			}
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_TWO)
			{
				//the low nipple
				PORTX= ( (PORTX & 0xc3) | ((lcd_command & 0x0f)<<2) );
			}
			else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_ONE)
			{
				//the low nipple
				PORTX= ( (PORTX & 0xe1) | ((lcd_command & 0x0f)<<1) );
			}
			else									    //PORT_PIN_ZERO
			{
				//the low nipple
				PORTX= ( (PORTX & 0xf0) | (lcd_command  & 0x0f) );
			}
	
		
	
		#elif NO_OF_DATA_LINES == 8
			//8bits mode
			write_port((gpio_bases *)lcd_obj->obj_data_port,(msa_u8*)&lcd_command);
		#else
	
			#error "invalid number of data lines.it can be 4 or 8 otherwise error"
			#endif
		
			//a high to low pulse to make tthe lcd latch the data in
			set_pin_high((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
			_delay_us(1);
			set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
		
		
			//a delay after each command
			if ( (lcd_command == RETURN_HOME) || (lcd_command == CLEAR_SCREEN) )
			{
				_delay_ms(2);
			} 
			else
			{
				_delay_us(120);
			}
	}
	else
	{
		ret_val = INVALID_LCD_PARAMS;
	}
	return ret_val;
}


lcd_return_types lcd_data_out(g_lcd_t* lcd_obj,msa_u8 lcd_data)
{
	lcd_return_types ret_val=NO_LCD_ERRORS;
	if( lcd_obj != NULL)
	{
		if (lcd_obj->obj_device_state != INITIATED)
		{
			ret_val=DEVICE_NOT_INITIATED;
			return ret_val;
		}
		set_pin_high((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_RS_pin);
		#if NO_OF_DATA_LINES == 4
		//4bits mode>>>many if statements i know but this is to reduce the repetition of the high to low pulse and the delay and so on>>>then it's for optimization
		if (lcd_obj->obj_data_pins_beginning == PORT_PIN_FOUR)
		{
			//the high nipple
			PORTX= (PORTX & 0x0f) | (lcd_data & 0xf0);
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_THREE)
		{
			//the high nipple
			PORTX= (PORTX & 0x87) | ( (lcd_data & 0xf0) >> 1 );
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_TWO)
		{
			//the high nipple
			PORTX= (PORTX & 0xc3) | ( (lcd_data & 0xf0) >> 2 );
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_ONE)
		{
			//the high nipple
			PORTX= (PORTX & 0xe1) | ( (lcd_data & 0xf0) >> 3 );
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_ZERO)
		{
			//the high nipple
			PORTX= (PORTX & 0xf0) | ( lcd_data >> 4);
		}
		
		//a high to low pulse to make tthe lcd latch the data in
		set_pin_high((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
		_delay_us(1);
		set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
		
		
		_delay_us(20);
		
		
		if (lcd_obj->obj_data_pins_beginning == PORT_PIN_FOUR)
		{
			//the low nipple
			PORTX= ( (PORTX & 0x0f) | (lcd_data<<4) );
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_THREE)
		{
			//the low nipple
			PORTX= ( (PORTX & 0x87) | ((lcd_data & 0x0f)<<3) );
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_TWO)
		{
			//the low nipple
			PORTX= ( (PORTX & 0xc3) | ((lcd_data & 0x0f)<<2) );
		}
		else if (lcd_obj->obj_data_pins_beginning == PORT_PIN_ONE)
		{
			//the low nipple
			PORTX= ( (PORTX & 0xe1) | ((lcd_data & 0x0f)<<1) );
		}
		else									    //PORT_PIN_ZERO
		{
			//the low nipple
			PORTX= ( (PORTX & 0xf0) | (lcd_data  & 0x0f) );
		}
		
		
		
		#elif NO_OF_DATA_LINES == 8
		//8bits mode
		write_port((gpio_bases *)lcd_obj->obj_data_port,(msa_u8*)&lcd_data);
		#else
		
		#error "invalid number of data lines.it can be 4 or 8 otherwise error"
		#endif
		
		//a high to low pulse to make tthe lcd latch the data in
		set_pin_high((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
		_delay_us(1);
		set_pin_low((gpio_bases *)lcd_obj->obj_comm_port,lcd_obj->obj_E_pin);
		
		_delay_us(120);
	}
	else
	{
		ret_val=INVALID_LCD_PARAMS;
	}
return ret_val;
}


lcd_return_types lcd_goto_xy(g_lcd_t* lcd_obj,msa_u8 lcd_x,msa_u8 lcd_y)
{
	lcd_return_types ret_val=NO_LCD_ERRORS;
	if ((lcd_obj != NULL) && ( (lcd_x >= 1) && (lcd_x <= 16) ) && ( (lcd_x >= 1) && (lcd_y <= 2) ) )
	{
		msa_u8 first_char_add[]={0x80,0xc0,0x94,0xd4};
			lcd_comm_out(lcd_obj,first_char_add[lcd_y-1]+lcd_x-1);
			_delay_us(100);
	}
	else
	{
		ret_val=INVALID_LCD_PARAMS;
	}
	return ret_val;
}

lcd_return_types lcd_array_data_out(g_lcd_t* lcd_obj,msa_u8* lcd_array_data)
{
	lcd_return_types ret_val=NO_LCD_ERRORS;
	if ( (lcd_obj != NULL) && (lcd_array_data != NULL) )
	{
		msa_u8 char_cntr=0;
		while(lcd_array_data[char_cntr])
		{
			lcd_data_out(lcd_obj,lcd_array_data[char_cntr++]);
		}
	}
	else
	{
		ret_val=INVALID_LCD_PARAMS;
	}
	return ret_val;
}

lcd_return_types lcd_deinit(g_lcd_t* lcd_obj)
{
	lcd_return_types ret_val=NO_LCD_ERRORS;
	if (lcd_obj != NULL)
	{
		lcd_obj->obj_device_state		=NOT_INITIATED;
	}
	else
	{
		ret_val=INVALID_LCD_PARAMS;
	}
	return ret_val;
}
