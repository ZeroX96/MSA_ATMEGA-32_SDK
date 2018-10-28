/*
 * HAL_PWM.c
 *
 * Created: 12/09/2018 04:19:54
 * Author : Mahmoud
 */ 

#include "hal_PWM.h"
/*
*	will divide the slot/the available time period into slots so the motor will be controlled with +-10 every pwm_up or pwm_dwn
*	will work on the three available pwm modes specially with the one with the large period of time so we have more ctr over the motor speed
*	The OCR0 Register is double buffered when using any of the Pulse Width Modulation (PWM) modes.
*	The actual OC0 value will only be visible on the port pin if the data direction for the port pin is set as output.
*   in fast-mode		Focn_PWM=(F_clk_io/(N*256))
*	in phase-correct	Focn_PWM=(F_clk_io/(N*510))
*	The Timer/Counter Overflow Flag (TOV0) is set each time the counter reaches BOTTOM.
*/  
int main(void)
{
	st_pwm_object pwm_1,pwm_2;
	DDRB=0xff;
	PORTB=0xff;
	DDRD=0xff;
	PORTD=0xff;
	pwm_error_t res= pwm_init(&pwm_1,TIMER_0,NON_INVERTED,FAST_PWM,31250,FIVE_PERCENT);
			    //res= pwm_init(&pwm_2,TIMER_2,NON_INVERTED,FAST_PWM,15686,FIVE_PERCENT|TWENTY_PERCENT|FIFTY_PERCENT);
    while (1) 
    {
    }
}

