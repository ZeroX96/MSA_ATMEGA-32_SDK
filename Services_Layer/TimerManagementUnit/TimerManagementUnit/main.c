/*
 * TimerManagementUnit.c
 *
 * Created: 02/08/2018 17:50:19
 * Author : Mahmoud
 */ 
#include <avr/io.h>
#include "TMU.h"

void led_toggle_A(void)
{
	//write code here
	PORTA^=0xff;
	//_delay_ms(1000);
	//PORTA=0x00;
	//_delay_ms(1000);
	
}

void led_toggle_B(void)
{
	//write code here
	PORTB^=0xff;
	//_delay_ms(1000);
	//PORTA=0x00;
	//_delay_ms(1000);
	
}

void led_toggle_C(void)
{
	//write code here
	PORTC^=0xff;
	//_delay_ms(1000);
	//PORTA=0x00;
	//_delay_ms(1000);
	
}

void led_toggle_D(void)
{
	//write code here
	PORTD^=0xff;
	//_delay_ms(1000);
	//PORTA=0x00;
	//_delay_ms(1000);
	
}

int main(void)
{
	TMU_STR_ELEMENT obj1,obj2,obj3,obj4;
	DDRA=0xff;
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0xff;
	TMU_ErrorTypes exe_res=tmu_init(10);
	/*if (exe_res == NO_ERRORS)
	{*/
		exe_res=tmu_start_timer( &obj1,PERIODIC,50,led_toggle_A);
		exe_res=tmu_start_timer( &obj2,PERIODIC,100,led_toggle_B);
		exe_res=tmu_start_timer( &obj3,PERIODIC,500,led_toggle_C);
		exe_res=tmu_start_timer( &obj4,PERIODIC,1000,led_toggle_D);
		/*if (exe_res == NO_ERRORS)
		{
		PORTB=0xff;
		}
		else
		{
		PORTB=0x0f;
		}
	} 
	else
	{
		PORTB=exe_res;
	}
	*/
    while (1) 
    {
		//PORTC=0x55;
		//_delay_ms(250);
		//PORTC=0xaa;
		//_delay_ms(250);
		tmu_dispatch();
    }
}

