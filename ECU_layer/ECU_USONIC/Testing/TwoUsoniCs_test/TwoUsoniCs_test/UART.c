/*
 * UART.c
 *
 * Created: 30/11/2018 04:41:01
 *  Author: Mahmoud
 */ 
#include "UART.h"

//the usart
void usart_init (void )
{
	UCSRB=(1<<TXEN)|(1<<RXEN);
	UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	UBRRL=0x33; //9600 @ 8mhz
}
void usart_send(unsigned char ch)
{
	while (!(UCSRA &(1<<UDRE)))
	;
	UDR=ch;
}
void usart_send_arr(uint8_t *val)
{
	unsigned char i=0;
	for (i=0;val[i] ;i++)
	{
		usart_send(val[i]);
	}
	//usart_send('\n');
	
}
unsigned char usart_recve(void){
	static unsigned char temp;
	while(!(UCSRA & (1<<RXC)))
	;
	temp=UDR;
	return temp;
}
