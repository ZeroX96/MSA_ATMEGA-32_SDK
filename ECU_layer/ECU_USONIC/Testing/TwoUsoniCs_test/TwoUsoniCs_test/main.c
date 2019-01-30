/*
 * TwoUsoniCs_test.c
 *
 * Created: 27/11/2018 06:43:17
 * Author : Mahmoud
 */
#include "INCLUDES.h"
void main_init(void);
int main(void)
{
	main_init();
	static uint8_t usonic0;
	static uint8_t usonic1;
    while (1) 
    {
	    usonic0=read_ultra_sonic0();
	    send_usonic0(usonic0);
	    usonic1=read_ultra_sonic1();
	    send_usonic1(usonic1);
		usart_send('\r');
		_delay_ms(150);
		//(void)usart_recve();
	}
	return 0;
}
void main_init(void)
{
	
	DDRC=0xff;
	usart_init();
	//ultrasonic
	DDRD &=~(2<<2);      //setting the interrupt pin as an input
	_delay_ms(50);
	GICR|=(1<<INT0)|(1<<INT1);//	//enabling interrupt0 & interrupt1
	MCUCR|=(1<<ISC00)|(1<<ISC10);   //setting interrupt triggering at any logic change for both interrupts
	TIMSK|=(1<<TOIE2);
	sei();               //enabling global interrupts

}