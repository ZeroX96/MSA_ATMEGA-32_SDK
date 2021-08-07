/*
 * SpeedButtons.h
 *
 * Created: 4/13/2020 8:42:46 AM
 *  Author: Mahmo
 */ 


#ifndef SPEEDAPP_H_
#define SPEEDAPP_H_
#include "Gpio.h"
#include "Pwm.h"

#define POS_B_BASE	BASE_A
#define POS_B_PIN   PIN_1

#define NEG_B_BASE	BASE_A
#define NEG_B_PIN	PIN_2

#define MOTOR_MAX_SPEED		95
#define MOTOR_MID_SPEED		55
#define MOTOR_MIN_SPEED		20
#define MOTOR_DEFAULT_SPEED MOTOR_MIN_SPEED
#define MOTOR_FREQUENCY		(150U)
//called in the init
void SpeedButtonsInit();
//called in the loop
void ProcessNBChange();
//called in the loop
void ProcessPBChange();
//called in the isr
void CheckPBChange();
//called in the isr
void CheckNBChange();
#endif /* SPEEDAPP_H_ */