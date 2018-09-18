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


int main(void)
{
	DDRA=0xff;
	DDRD=0x00;
	//PORTD|=(1<<2);
	init_interrrupts();
	set_interrupt(INTERRUPT0,FALLING_EDGE);
	set_interrupt(INTERRUPT1,FALLING_EDGE);
    while (1) 
    {
		PORTA=0xff;
		_delay_ms(250);
		PORTA=0x00;
		_delay_ms(250);
	}
}
//void INT0_vect(void)
ISR(INT0_vect)
{
	sei();//testing the nested interrupt>>>clear if dont wanna 
	//init_interrrupts();
	*((volatile unsigned char *)0x35)=0x01;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x00;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x01;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x00;
}

//void INIT1_vect(void)
ISR(INT1_vect)
{
	sei();//testing the nested interrupt>>>clear if dont wanna 
	//init_interrrupts();
	*((volatile unsigned char *)0x35)=0x02;
	_delay_ms(500);

	*((volatile unsigned char *)0x35)=0x00;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x02;
	_delay_ms(500);
	*((volatile unsigned char *)0x35)=0x00;
}

