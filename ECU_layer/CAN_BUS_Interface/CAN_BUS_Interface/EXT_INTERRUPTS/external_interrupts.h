/*
 * external_interrupts.h
 *
 * Created: 31/07/2018 12:52:53
 *  Author: Mahmoud
 */ 
//NOTE_when using interrupts it's better to use their pins only for interrupts 
//as some modes will use the pull-up resistor 
//and then will work only when it's set as input

#ifndef EXTERNAL_INTERRUPTS_H_
#define EXTERNAL_INTERRUPTS_H_
typedef enum{
	INTERRUPT0 =6,			//6
	INTERRUPT1 =7,			//7
	INTERRUPT2 =5,			//5_NOTE_INT2 has only two modes falling or rising edge  
	}INT_NAMES;

typedef enum{
	NO_ERRORS=0,			//0
	INVALID_PARAMS,			//1
	NOT_INITIALIZED,		//2
	INVALID_MODE_FOR_INT2	//3
}INT_ERRORS;

typedef enum{
	LOW_LEVEL=0,			//0_NOTE_selecting this mode will enable the pull-up resistor for this pin
	FALLING_OR_RISING_EDGE,	//1	
	FALLING_EDGE,			//2	
	RISING_EDGE,			//3
}INT_MODES;

INT_ERRORS init_interrrupts(void);
INT_ERRORS set_interrupt(INT_NAMES int_name,INT_MODES int_mode);
INT_ERRORS clr_interrupt(INT_NAMES int_name);
INT_ERRORS deinit_interrrupts(void);

#endif /* EXTERNAL_INTERRUPTS_H_ */