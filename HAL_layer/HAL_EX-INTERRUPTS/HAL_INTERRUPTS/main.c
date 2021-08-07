/*
 * HAL_INTERRUPTS.c
 * Created: 30/07/2018 11:52:41
 * Author : Mahmoud
 *Note>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>i'm testing the nested interrrupt in the avr so u have to edit the ISR
 *>>v0.1
 *till now I'm handling only the external interrupts
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "external_interrupts.h"
#include <avr/interrupt.h>

int cntr=0;
int main(void)
{
	DDRA=0xff;
	DDRD=0x00;
	DDRB=0xff;
	//PORTD=0xff;
	DDRC=0xff;
	//PORTD|=(1<<2);
	init_interrrupts();
	set_interrupt(INTERRUPT0,RISING_EDGE);
	//set_interrupt(INTERRUPT1,RISING_EDGE);
    while (1) 
    {
		PORTA=0xff;
		_delay_ms(250);
		PORTA=0x00;
		_delay_ms(250);
		if (cntr >= 2)
		{
			
			deinit_interrrupts();
	}
	PORTB=cntr;
	}
}
//void INT0_vect(void)
ISR(INT0_vect)
{
	//sei();//testing the nested interrupt>>>clear if dont wanna 
	//init_interrrupts();
	*((volatile unsigned char *)0x35)=0xff;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x00;
	_delay_ms(500);
	//PORTB=cntr;
	if (cntr >= 2)
	{
		
		deinit_interrrupts();
		
	}
	cntr++;
}
/*
//void INIT1_vect(void)
ISR(INT1_vect)
{
	//sei();//testing the nested interrupt>>>clear if dont wanna 
	//init_interrrupts();
	*((volatile unsigned char *)0x35)=0x02;
	_delay_ms(500);

	*((volatile unsigned char *)0x35)=0x00;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x02;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x00;
}
*/
