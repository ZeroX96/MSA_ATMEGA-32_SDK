/*
 * HAL_GPIO.c
 *
 * Created: 11/09/2018 20:53:03
 * Author : Mahmoud
 */ 
#include "hal_GPIO.h"


int main(void)
{
	gpio_bases *port_a=(gpio_bases *)BASE_A;
	gpio_bases *port_b=(gpio_bases *)BASE_B;
	gpio_bases *port_c=(gpio_bases *)BASE_C;
	msa_u8 val;
	bool_t val_of_pin;
    while (1) 
    {
		set_port_output	(port_a);
		set_port_input	(port_b);
		set_pin_output	(port_c,PIN_ONE);
		set_pin_output	(port_c,PIN_TWO);
		set_pin_input	(port_c,PIN_FOUR);
		set_pin_input	(port_c,PIN_FIVE);
		read_port		(port_b,&val);
		/*if (val >= 2  && val < 4)
		{
			PORTA=0x02;
		} 
		else if (val >= 4 && val < 8)
		{
				PORTA=0x04;
		}else if (val >= 8 && val < 16)
		{
			PORTA=0x08;
		}
		else
		PORTA=0x00;
		*/
		write_port		(port_a,&val);
		read_pin		(port_c,PIN_FOUR,&val_of_pin);
		if (val_of_pin == 0)
		{
			set_pin_low(port_c,PIN_ONE);
		} 
		else
		{
			set_pin_high(port_c,PIN_ONE);
		}
    }
}

