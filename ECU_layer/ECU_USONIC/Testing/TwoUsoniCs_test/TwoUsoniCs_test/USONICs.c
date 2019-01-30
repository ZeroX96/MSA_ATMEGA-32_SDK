/*
 * USONICs.c
 *
 * Created: 30/11/2018 04:38:27
 *  Author: Mahmoud
 */ 
#include "USONICs.h"

//  the ultrasonic0 using counter 1
static volatile int pulse0 = 0;  // integer  to cal the time of the signal transmission and reception
static volatile int state0 = 0;      // integer  to access all though the program
//(cancelled)static volatile int OVF_CNTR_0 = 0;      // integer  to take care of the over-flows of u_sonic_1
static volatile char SHOWA [16];			// displaying digital output as it's sent as ascii

//  the ultrasonic1 using counter 2
static volatile int pulse1 = 0;  // integer  to cal the time of the signal transmission and reception
static volatile int state1 = 0;      // integer  to access all though the program
static volatile int OVF_CNTR_1 = 0;      // integer  to take care of the over-flows of u_sonic_1
static volatile char SHOWB [16];			// displaying digital output as it's sent as ascii

//  the ultrasonic0 using counter 1
uint8_t read_ultra_sonic0(void) //0000 0011
{
	PORTC |=(1<<0);
	_delay_us(15);		//triggering the sensor for 15usec
	PORTC &=~(1<<0);
	//(pulse0/58) +(1*OVF_CNTR_0);	//getting the distance based on formula on introduction
	//test the delay here??//to get more accurate values
	_delay_us(50);
	return ( pulse0 / (58 * (F_CPU/1000000) ));// + (1*OVF_CNTR_0) );//the res is not exact :{ i.e didnt cover ovf.s
}
void send_usonic0(uint16_t COUNTA)
{
	itoa(COUNTA,SHOWA,10);
	usart_send_arr("usonic0 data = ");
	usart_send_arr(SHOWA);
	usart_send_arr("    ");
	
}

//  the ultrasonic1 using counter 2
uint8_t read_ultra_sonic1(void) //0000 0011
{
	PORTC |=(1<<1);
	_delay_us(15);		//triggering the sensor for 15usec
	PORTC &=~(1<<1);
	_delay_us(50);
	//return ( pulse1 / (58 * (F_CPU/1000000) ));// + (1*OVF_CNTR_0) );//the res is not exact :{ i.e didnt cover ovf.s

	return ( (pulse1+ (OVF_VAL*OVF_CNTR_1))/(58 * (F_CPU/1000000)) );	//getting the distance based on formula on introduction
	
}
void send_usonic1(uint16_t COUNTB)
{
	
	itoa(COUNTB,SHOWB,10);
	usart_send_arr("usonic1 data = ");
	usart_send_arr(SHOWB);
	usart_send_arr("    ");
	
}

ISR(TIMER2_OVF_vect)
{
	OVF_CNTR_1++;
	PORTC^=(1<<5);
}


 //will test to work as is and later will test using semaphores and the cond will be  if not available return specific val to tell the func that couldn't read the distancve this ime
 //later on if used this algo, notice if the val couldn't be read, dont move the servo this turn :D <3 


//interrupt service routine when there is a change in logic level for the USONIC_0
ISR(INT0_vect)
{

	if (state0==1)//when logic from HIGH to LOW
	{

		TCCR1B=0;//disabling counter

		pulse0=TCNT1;//count memory is updated to integer

		TCNT1=0;//resetting the counter memory

		state0=0;
		//OVF_CNTR_0=0;

	}
	else//when logic change from LOW to HIGH
	{

		TCCR1B|=(1<<CS10);//enabling counter

		state0=1;

	}

}

//interrupt service routine when there is a change in logic level for the USONIC_0
ISR(INT1_vect)
{

	if (state1==1)//when logic from HIGH to LOW
	{

		TCCR2=0;//disabling counter

		pulse1=TCNT2;//count memory is updated to integer

		TCNT1=0;//resetting the counter memory

		state1=0;
		OVF_CNTR_1=0;

	}
	else//when logic change from LOW to HIGH
	{

		TCCR2|=(1<<CS21)|(1<<CS21);//enabling counter

		state1=1;

	}

}