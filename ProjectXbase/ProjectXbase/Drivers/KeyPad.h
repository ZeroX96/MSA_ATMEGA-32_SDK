/*
 * Keypad.h
 *
 * Created: 7/13/2021 6:09:34 AM
 *  Author: ZeroX
 */ 

#ifndef KEYPAD_H_
#define KEYPAD_H_
#define F_CPU (8000000UL)
#include "HwAccess.h"
#include "Gpio.h"
#include "Timer.h"

#define KEYPAD_NUMB_COLS  (0x04U) //4
#define KEYPAD_NUMB_ROWS  (0x04U) //4
#define MAX_BUFFER_LENGTH (0x20)  //32

#define KEYPAD_SAMPLES_NUMB     (0x03U) //3
#define KEYPAD_SAMPLING_PERIOD  (0x14U) //20   
typedef enum{
    MODULE_NOT_INITIALIZED,
    MODULE_INITIALIZED,
}ModuleStateT;

typedef enum{
	KEY_RELEASED,
	KEY_PRE_PRESSED,
	KEY_PRESSED,
	KEY_PRE_RELEASED,
}KeypadStateT;

typedef enum{
	PIN_ZERO,
	PIN_ONE,
	PIN_TWO,
	PIN_THREE,
	PIN_FOUR,
	PIN_FIVE,
	PIN_SIX,
	PIN_SEVEN
}PinPositionT;

typedef struct{
    GpioBaseT ColsPort;
    PinPositionT ColsPosition; 
    GpioBaseT RowsPort;
    PinPositionT RowsPosition;
    ModuleStateT InitState;
}KeypadT;

//initialization
void KeypadInit(KeypadT* Keypad);
//task or isr -- periodic task
void KeypadUpdate(void);
//function
uint8_t KeypadGetKey(KeypadT* KeypadCfg);


#endif /* KEYPAD_H_ */