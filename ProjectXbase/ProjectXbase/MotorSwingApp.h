/*
 * MotorSwingApp.h
 *
 * Created: 4/2/2020 9:53:25 PM
 *  Author: Mahmo
 */ 


#ifndef MOTORSWINGAPP_H_
#define MOTORSWINGAPP_H_
#include "Gpio.h"
#include "Adc.h"
#include <stdint.h>

//100/9
#define SwingMotoValRange 11
#define S_MOTOR_F_PIN PIN_4
#define S_MOTOR_PORT BASE_A

typedef enum{
	DEG_0   = 0x00,
	DEG_45  = 0x01,
	DEG_90  = 0x02,
	DEG_135 = 0x03,
	DEG_180 = 0x04,
	DEG_225 = 0x05,
	DEG_270 = 0x06,
	DEG_315 = 0x07,
	DEG_360 = 0x08,
}SwingAngleT;

typedef enum{
	SWING_ERR_NO,
	SWING_ERR_INV_PARAMS,	
}SwingRetT;

//called in the init
SwingRetT SwingMotoInit();
//called in the isr
SwingRetT CheckSwingChange();
//called in the loop
SwingRetT ProcessSwingChange();
#endif /* MOTORSWINGAPP_H_ */