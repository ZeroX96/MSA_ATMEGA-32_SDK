/*
 * TimerButton.h
 *
 * Created: 4/13/2020 6:10:58 PM
 *  Author: Mahmo
 */ 


#ifndef TIMERAPP_H_
#define TIMERAPP_H_

#include <stdint.h>
#include "Gpio.h"
#include "Timer.h"

#define HALF_SEC 50
#define FULL_SEC 100

#define TIMER_B_BASE BASE_D
#define TIMER_B_PIN  PIN_5

#define CLK_TOGG_LED_BASE	BASE_D
#define CLK_TOGG_LED_PIN	PIN_4

//called in the init
void SysTimerInit(void(*TimerCBF)(void));
//called in the loop to toggle the led
void ProcessTimeOut();
//called in the isr
void CheckTimeOut();
//called in the loop to update the value
void CheckTimerBChange();
//called in the isr to get the status
void ProcessTimerBChange();


#endif /* TIMERAPP_H_ */