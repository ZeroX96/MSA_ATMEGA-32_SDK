/*
 * LcdApp.h
 *
 * Created: 4/15/2020 7:43:14 AM
 *  Author: Mahmo
 */ 


#ifndef LCDAPP_H_
#define LCDAPP_H_


#include "Lcd.h"
#include <stdint.h>

#define ASCII_BASE 48
#define TWENTY_SEC 2000

#define LCD_B_BASE BASE_A
#define LCD_B_PIN  PIN_3

//called in the init
void LcdAppInit();
//called in the isr
void CheckDispBChange();
//called in the loop
void ProcessDispBChange();


#endif /* LCDAPP_H_ */