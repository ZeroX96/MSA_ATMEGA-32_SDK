/*
 * BuzzApp.h
 *
 * Created: 4/16/2020 2:53:23 PM
 *  Author: Mahmo
 */ 


#ifndef BUZZAPP_H_
#define BUZZAPP_H_


#include <stdint.h>
#include "Gpio.h"

#define BUZZ_BASE	BASE_D
#define BUZZ_PIN	PIN_6
void InitBuzz();
void ProcessBuzz();

#endif /* BUZZAPP_H_ */