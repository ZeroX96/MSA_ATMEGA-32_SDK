/*
 * HAL_TIMER.c
 *
 * Created: 02/08/2018 18:40:45
 * Author : Mahmoud
 *https://github.com/farshield
 */ 
#define F_CPU 1000000
//#include <avr/io.h>
#include "timers.h"
#include <avr/interrupt.h>
#include <util/delay.h>
//add_freqs(timer_object *obj);
	static timer_object timer_test={TIMER_0,NORMAL_MODE,NO_PRESCALER,OC0_DISCONNECTED,0x00,75,NON_INTERRUPTABLE};
	
int main(void)
{
	sei();
	/*
typedef struct
{
	timer_bases timer_base;
	timer_mode t_mode;
	freq_select timer_freq;
	compare_output_mode timer_oc_mode;
	ecore_u16 timer_start_val;
	ecore_u16 timer_comp_val;
	sys_interrupt_mode int_mode;
	//void (*pf_timer_cb)(void);
}timer_object;
	*/
	timer_error_types res= init_timer(&timer_test);
	DDRA=0xff;
	DDRD=0xff;
	DDRC=0xff;
    while (1) 
    {
		PORTA^=0xff;
		_delay_ms(250);
		
		//PORTC=res;
    }
}

/*
timer_error_types init_timer(timer_object *given_obj);  //test,void (*pf_cb)(void)
timer_error_types is_timer_finished(timer_object *given_obj);
timer_error_types read_timer_val(timer_object *given_obj,ecore_u16 *val);
timer_error_types update_timer(timer_object *given_obj,ecore_u8 *val);
timer_error_types update_comp_timer(timer_object *given_obj,ecore_u8 *val);
timer_error_types deinit_timer(timer_object *given_obj);*/
ISR(TIMER0_OVF_vect)
{static ecore_u16 temp=0;
	temp++;
	if(temp %5)
	update_timer(&timer_test,&temp);
	_delay_ms(250);
	PORTC^=0xff;
}

ISR(TIMER0_COMP_vect)
{
	_delay_ms(250);
	PORTD^=0xff;
}
