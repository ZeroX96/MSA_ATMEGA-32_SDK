/*
 * TMU.c
 *
 * Created: 02/08/2018 17:50:48
 *  Author: Mahmoud Saad
 */ 
#include "common.h"
#include "TMU.h"
#include <avr/interrupt.h>
static TMU_STR_ELEMENT * tmu_internal_bufer[MAX_TIMERS_NO];
static ecore_u8 tmu_internal_bufer_counter=0;
//static ecore_u32 MAIN_COUNTER=0;
static bool_t tmu_initiated=E_FALSE;
static ecore_u16 tmu_Resolution=0 ;

ISR(TIMER0_COMP_vect)
{
	for (ecore_u8 i=0; i<MAX_TIMERS_NO; i++)
	{
		if (tmu_internal_bufer[i])	//if there is a valid address process else skip
		{
			tmu_internal_bufer[i]->remaining_time--;			
		}
		
	}
	TCNT0=0;
	OCR0=78;
	//sei();											//NoDifference
}

TMU_ErrorTypes tmu_init(ecore_u16 resolution)
{
	for(int i = 0; i < MAX_TIMERS_NO; i++) 
	{
		tmu_internal_bufer[i] = NULL;
	}
	TMU_ErrorTypes s_ret_val=NO_ERRORS;
	tmu_Resolution=resolution;
	tmu_initiated=E_TRUE;
	//null the internal buffer...think about it
	TCCR0=0x03;
	TCNT0=0;
	OCR0=78;
	TIMSK=1<<1;
	TIFR |=1;
	sei();
	return s_ret_val;
}

TMU_ErrorTypes tmu_start_timer(TMU_STR_ELEMENT *obj,Timer_Type timer_Type,ecore_u16 timer_Period,void (*CB_func)(void))
{
	cli();
	TMU_ErrorTypes s_ret_val=NO_ERRORS;
		if( (tmu_initiated == E_TRUE) )
		{
			if (tmu_internal_bufer_counter < MAX_TIMERS_NO )
			{
				if ( (obj != NULL) && ( (timer_Type == PERIODIC) || (timer_Type == ONESHOOT) ) && (timer_Period != 0) && (CB_func != NULL) )
				{
					obj->func_CB=CB_func;
					obj->period=timer_Period/tmu_Resolution;	//so the user may set it every one msec. or five msec.s //test it work and rethink later
					obj->remaining_time=obj->period;
					obj->timer_type=timer_Type;
					obj->index=tmu_internal_bufer_counter;
					tmu_internal_bufer[tmu_internal_bufer_counter]=obj;
					tmu_internal_bufer_counter++;
				} 
				else
				{
					PORTD=0x3;
					s_ret_val=INVALID_PARAMS;
				}
				
			} 
			else
			{
					PORTD=0x7;
				s_ret_val=MAX_NO_REACHED;
			}

		}
		else
		{
					PORTD=0x7;
			s_ret_val=TIMER_NOT_INITIATED;
		}
	sei();
	return s_ret_val;
}
TMU_ErrorTypes tmu_stop_timer(TMU_STR_ELEMENT *obj)
{
	cli();
		TMU_ErrorTypes s_ret_val=NO_ERRORS;
		if (obj != NULL)
		{
			obj->func_CB=NULL;
			obj->period=0;
			obj->remaining_time=0;
			obj->timer_type=INVALID_TYPE;
			tmu_internal_bufer[obj->index]=NULL;
			obj->index=0;		
			tmu_internal_bufer_counter--;
		} 
		else
		{
			s_ret_val=INVALID_PARAMS;
		}
	sei();
	return s_ret_val;

}
TMU_ErrorTypes tmu_dispatch(void)
{
	cli();
		TMU_ErrorTypes s_ret_val=NO_ERRORS;
		if (tmu_initiated)
		{
			for (ecore_u8 i=0; i<MAX_TIMERS_NO; i++)
			{
				if (tmu_internal_bufer[i])	//if there is a valid address process else skip
				{
					if( (tmu_internal_bufer[i]->remaining_time) <= 0)
					{
						//PORTD=tmu_internal_bufer[0]->remaining_time;
						tmu_internal_bufer[i]->func_CB();
						tmu_internal_bufer[i]->remaining_time+=tmu_internal_bufer[i]->period;
						if( tmu_internal_bufer[i]->timer_type == ONESHOOT)
						tmu_stop_timer(tmu_internal_bufer[i]);
					}
				}
			}
		
		}
		else
		{
			s_ret_val=TIMER_NOT_INITIATED;
		}
	sei();
	return s_ret_val;
}
TMU_ErrorTypes tmu_deinit(void)
{
	cli();
	TMU_ErrorTypes s_ret_val=NO_ERRORS;
	TCCR0=0x00;
	for(int i = 0; i < MAX_TIMERS_NO; i++) {
		tmu_internal_bufer[i] = NULL;
	}
	sei();
	return s_ret_val;
}
