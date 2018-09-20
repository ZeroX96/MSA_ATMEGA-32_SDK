/*
 * TMU.h
 *
 * Created: 02/08/2018 17:51:10
 *  Author: Mahmoud Saad
 */ 


#ifndef TMU_H_
#define TMU_H_
#include "common.h"
#define F_CPU 1000000UL
#include <util/delay.h>
#define MAX_TIMERS_NO 10

typedef enum
{
	PERIODIC=0,				//0
	ONESHOOT,				//1	
	INVALID_TYPE,			//2
}Timer_Type;

typedef enum
{
	NO_ERRORS=0,			//0
	TIMER_NOT_INITIATED,	//1
	INVALID_PARAMS,			//2
	MAX_NO_REACHED,			//3
	
}TMU_ErrorTypes;

typedef struct  
{
	void (*func_CB)(void);
	ecore_s16 remaining_time;
	ecore_s16 period;
	Timer_Type timer_type;
	ecore_u8  index;
}TMU_STR_ELEMENT;

//here i'll work on the func that reserves the timer0 first and then i'll modify it
TMU_ErrorTypes tmu_init(ecore_u16 resolution);
TMU_ErrorTypes tmu_start_timer(TMU_STR_ELEMENT *obj,Timer_Type timer_Type,ecore_u16 timer_Period,void (*CB_func)(void));
TMU_ErrorTypes tmu_stop_timer(TMU_STR_ELEMENT *obj);
TMU_ErrorTypes tmu_dispatch(void);
TMU_ErrorTypes tmu_deinit(void);

#endif /* TMU_H_ */