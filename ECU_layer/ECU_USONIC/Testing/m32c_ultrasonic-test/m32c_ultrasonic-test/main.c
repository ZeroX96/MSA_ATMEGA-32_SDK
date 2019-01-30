/*
 * m32c_ultrasonic-test.c
 * main_test_2.c
 *
 * Created: 23/01/2019 20:32:57
 *  Author: Mahmoud
 */ 
/*
 * m32c_ultrasonic-test.c
 * main_test_1
 * Created: 28/04/2018 11:56:12
 * Author : Mahmoud
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include "usart.h"
static volatile int pulse = 0;  // interger  to access all though the program
static volatile int i = 0;      // interger  to access all though the program

int main(void)
{
	//usart_init();
	DDRC=0xff;
	DDRA=0xff;
	DDRB=0xff;
	PORTB=0x55;
	DDRD &=~(1<<2);      //setting the interrupt pin as an input
	_delay_ms(50);
	GICR|=(1<<INT0);     //enabling interrupt0
	MCUCR|=(1<<ISC00);   //setting interrupt triggering logic change
	sei();               //enabling global interrupts
    uint16_t COUNTA = 0; //storing digital output
	char SHOWA [3];//displaying digital output as it's sent as ascii
    while (1) 
    {
		_delay_ms(500);
		PORTB =~PORTB;
		PORTC|=(1<<PINC0);
		_delay_us(15);		//triggering the sensor for 15usec
		PORTC &=~(1<<PINC0);
		_delay_ms(50);
		COUNTA = pulse/58;	//getting the distance based on formula on introduction
		//itoa(COUNTA,SHOWA,10);
		PORTA=COUNTA;
// 		usart_send_arr("current usonic value= ");
// 		usart_send_arr(SHOWA);
// 		usart_send("\n");
		
		

    }
}


ISR(INT0_vect)//interrupt service routine when there is a change in logic level

{

	if (i==1)//when logic from HIGH to LOW

	{

		TCCR1B=0;//disabling counter

		pulse=TCNT1;//count memory is updated to integer

		TCNT1=0;//resetting the counter memory

		i=0;

	}

	if (i==0)//when logic change from LOW to HIGH

	{

		TCCR1B|=(1<<CS10);//enabling counter

		i=1;

	}

}