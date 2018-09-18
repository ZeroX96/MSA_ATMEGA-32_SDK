/*
 * timers.h
 *
 * Created: 06/08/2018 23:30:40
 *  Author: Mahmoud
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_
#include <avr/io.h>
#include "common.h"
#define FORCE_OUPUT_CMP() (*(volatile ecore_u8 *)53)

typedef enum 
{
	NORMAL_MODE=0,
	CTC_MODE=2,
}timer_mode;

typedef enum
{
	TIMER_0=0x53,
	TIMER_1A=0x4f,
	TIMER_1B=0x4e,
	TIMER_2=0x45,
}timer_bases;


typedef enum
{
	OFF=0,
	NO_PRESCALER=1,
	PRESCALER_8=2,
	PRESCALER_64=3,
	PRESCALER_256=4,
	PRESCALER_1024=5,
	EXT_FALLING_EDGE=6,
	EXT_RISING_EDGE=7,
}freq_select;

typedef enum
{
	OC0_DISCONNECTED=0,
	OC0_TOGGLE=1,
	OC0_CLEAR=2,
	OC0_SET=3,
}compare_output_mode;

typedef enum
{
	NO_ERRORS=0,
	INVALID_PARAMS,
	CANT_ENSURE_A_RIGHT_VAL,
		
}timer_error_types;

typedef enum
{
	NON_INTERRUPTABLE=0x20,
	INTERRUPTABLE_AT_OV,
	INTERRUPTABLE_AT_OC,		
	INTERRUPTABLE_AT_OV_AND_OC=0x23,	
}sys_interrupt_mode;
typedef struct  
{
	timer_bases timer_base;
	timer_mode t_mode;
	freq_select timer_freq;
	compare_output_mode timer_oc_mode;
	ecore_u16 timer_start_val;
	ecore_u16 timer_comp_val; 
	sys_interrupt_mode int_mode;
	//void (*pf_timer_cb)(void);
}timer_object;

timer_error_types init_timer(timer_object *given_obj);  //test,void (*pf_cb)(void)
timer_error_types is_timer_finished(timer_object *given_obj,ecore_u16 *return_res);
timer_error_types read_timer_val(timer_object *given_obj,ecore_u16 *return_res);
timer_error_types update_timer(timer_object *given_obj,ecore_u8 *val);
timer_error_types update_comp_timer(timer_object *given_obj,ecore_u8 *val);
timer_error_types deinit_timer(timer_object *given_obj);
#endif /* TIMERS_H_ */