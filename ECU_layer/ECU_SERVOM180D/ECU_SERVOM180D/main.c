/*
 * ECU_SERVOM180D.c
 *
 * Created: 28/10/2018 22:25:48
 * Author : Mahmoud
 */ 

#include <avr/io.h>
#include "hal_PWM.h"

int main(void)
{
    st_pwm_object pwm_obj1;
	DDRB=0xff;
	//msa_u8 i=0;
	pwm_init(&pwm_obj1,TIMER_0,INVERTED,FAST_PWM,20,5);
    while (1) 
    {
		for (msa_u8 i=0;i<=10;i++)
		{
			pwm_edit(&pwm_obj1,INVERTED,FAST_PWM,20,i);
			_delay_ms(500);
			//i= i>20 ? 0:(++i);
		}
		for (int j=0;(10-j)>=0;j++)
		{
			pwm_edit(&pwm_obj1,INVERTED,FAST_PWM,20,(20-j));
			_delay_ms(500);
			//i= i>20 ? 0:(++i);
		}
		
		
   }
}

