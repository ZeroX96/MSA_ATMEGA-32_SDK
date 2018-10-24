/*
 * HAL_PWM.c
 *
 * Created: 12/09/2018 04:19:54
 * Author : Mahmoud
 */ 

#include <avr/io.h>

/*
*will divide the slot/the available time period into slots so the motor will be controlled with +-10 every pwm_up or pwm_dwn
*will work on the three available pwm modes specially with the one with the large period of time so we have more ctr over the motor speed
*The OCR0 Register is double buffered when using any of the Pulse Width Modulation (PWM) modes.
*/  
int main(void)
{
	
    while (1) 
    {
    }
}

