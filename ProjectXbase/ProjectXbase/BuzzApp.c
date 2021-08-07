/*
 * BuzzApp.c
 *
 * Created: 4/16/2020 2:53:11 PM
 *  Author: Mahmo
 */ 
#include "BuzzApp.h"
int8_t BuzzDelay = 0;
void InitBuzz(){
	GpioInitPin(BUZZ_BASE,BUZZ_PIN,PIN_OUT);
}
void ProcessBuzz(){
	if (BuzzDelay > 0){
		GpioWritePin(BUZZ_BASE,BUZZ_PIN,PIN_HIGH);
	} 
	else{
		GpioWritePin(BUZZ_BASE,BUZZ_PIN,PIN_LOW);
	}
}