/*
 * hal_GPIO.c
 *
 * Created: 18/09/2018 22:27:26
 *  Author: Mahmoud
 */ 
#include "hal_GPIO.h"

#define NULL ((void *)0)

gpio_errors set_port_output(gpio_bases *base)
{
	gpio_errors b_ret_val=NO_ERRORS;
	if ( (base >=(gpio_bases*) BASE_D) && (base <=(gpio_bases*) BASE_A) )
	{
		*((volatile msa_u8*)base+DDR_OFFSET)=0xff;
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	return b_ret_val;
}

//dont need the set_port_input(gpio_bases *base) so deleted with 
#if 0
gpio_errors set_port_input(gpio_bases *base)
{
	gpio_errors b_ret_val=NO_ERRORS;
	if ( (base >=(gpio_bases*) BASE_D) && (base <=(gpio_bases*) BASE_A) )
	{
		*((volatile msa_u8*)base+DDR_OFFSET)=0x00;
		*((volatile msa_u8*)base+PORT_OFFSET)=0xff;//activates the pull up resistor,comment if not needed !!!>>:<<
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}

	return b_ret_val;
}
#endif

gpio_errors set_pin_output(gpio_bases *base,gpio_pin_no pin_no)
{
	
	gpio_errors b_ret_val=NO_ERRORS;
	if ( (base >=(gpio_bases*) BASE_D) && (base <=(gpio_bases*) BASE_A) && ( (pin_no >= PIN_ZERO) && (pin_no <= PIN_SEVEN) ) )
	{
		*((volatile msa_u8*)base+DDR_OFFSET)|=(1<<pin_no);
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	return b_ret_val;
}

//dont need the set_pin_input(gpio_bases *base,gpio_pin_no pin_no) so deleted with
#if 0
gpio_errors set_pin_input(gpio_bases *base,gpio_pin_no pin_no)
{
	
	gpio_errors b_ret_val=NO_ERRORS;
	if ( (base >= (gpio_bases*) BASE_D) && (base <= (gpio_bases*) BASE_A) && ( (pin_no >= PIN_ZERO) && (pin_no <= PIN_SEVEN) ) )
	{
		*((volatile msa_u8*)base+DDR_OFFSET)&=~(1<<pin_no);
		*((volatile msa_u8*)base+PORT_OFFSET)|=(1<<pin_no);
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	return b_ret_val;
}
#endif

//dont need the read_port(gpio_bases *base,msa_u8 *val) so deleted with
#if 0
gpio_errors read_port(gpio_bases *base,msa_u8 *val)
{
	gpio_errors b_ret_val=NO_ERRORS;
	if((base >= (gpio_bases*) BASE_D) && (base <= (gpio_bases*) BASE_A) && (val != NULL))
	{
		
		*val=*(msa_u8 *)base;
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	
	
	return b_ret_val;
}
#endif

//edited the gpio write_port(gpio_bases *base,msa_u8 *val) to fulfill the lcd driver requirements
gpio_errors write_port(gpio_bases *base,msa_u8 *val)
{
	
	gpio_errors b_ret_val=NO_ERRORS;
	if((base >= (gpio_bases*) BASE_D) && (base <= (gpio_bases*) BASE_A) && (val != NULL))
	{
		
		*((msa_u8 *)base+PORT_OFFSET)=*val;
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	
	
	return b_ret_val;
}

//dont need the read_pin(gpio_bases *base,msa_u8 pin_no,bool_t *val) so deleted with
#if 0
gpio_errors read_pin(gpio_bases *base,msa_u8 pin_no,bool_t *val)
{	gpio_errors b_ret_val=NO_ERRORS;
	if( (base >= (gpio_bases*) BASE_D) && (base <= (gpio_bases*) BASE_A) && (val != NULL) && ( (pin_no >= PIN_ZERO) && (pin_no <= PIN_SEVEN) ) )
	{
		*val=(bool_t)((*(msa_u8 *)base)&(1<<pin_no));
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	
	
	return b_ret_val;
}
#endif

gpio_errors set_pin_low(gpio_bases *base,gpio_pin_no pin_no)
{
	gpio_errors b_ret_val=NO_ERRORS;
	if( (base >= (gpio_bases*) BASE_D) && (base <= (gpio_bases*) BASE_A) && ( (pin_no >= PIN_ZERO) && (pin_no <= PIN_SEVEN) ) )
	{
		(*(volatile msa_u8 *)(base+PORT_OFFSET))&=~(1<<pin_no);
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	return b_ret_val;
}
gpio_errors set_pin_high(gpio_bases *base,gpio_pin_no pin_no)
{
	gpio_errors b_ret_val=NO_ERRORS;
	if( (base >= (gpio_bases*) BASE_D) && (base <= (gpio_bases*) BASE_A) && ( (pin_no >= PIN_ZERO) && (pin_no <= PIN_SEVEN) ) )
	{
		(*(volatile msa_u8 *)(base+PORT_OFFSET))|=(1<<pin_no);
	}
	else
	{
		b_ret_val=INVALID_PARAMS;
	}
	
	return b_ret_val;
}
//end of time :D.....driver completed by-MSA