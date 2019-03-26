/*
 * tmers.c
 *
 * Created: 06/08/2018 23:30:23
 *  Author: Mahmoud
 */ 
#include "timer_1.h"
#define WGMn1 2 
#define WGMn0 6
#define COMn 4
#define  MAX_VAL 

#define TIMER_BASE 0x4f

timer_error_types init_timer(timer_object *given_obj)
{
	timer_error_types ret_val=NO_ERRORS;
	if (given_obj != NULL)
	{
		//test data for safety
		*(volatile ecore_u8*)(given_obj->timer_base)|=( (( (given_obj->t_mode) & 0x02)<<WGMn1) |((( (given_obj->t_mode) & 0x01)<<WGMn0)) );
		*(volatile ecore_u8*)(given_obj->timer_base)|=((given_obj->timer_oc_mode)<<COMn);            
		*(volatile ecore_u8*)(given_obj->timer_base)|= given_obj->timer_freq;
		//given_obj->pf_timer_cb=pf_cb;
		
		
	} 
	else
	{
		ret_val=INVALID_PARAMS;
	}
	return ret_val;
}

timer_error_types is_timer_finished(timer_object *given_obj,ecore_u16 *return_res)
{
	
	timer_error_types ret_val=NO_ERRORS;
	if (given_obj != NULL)
	{
		if ( (given_obj->int_mode >= INTERRUPTABLE_AT_OV) && (given_obj->int_mode <= INTERRUPTABLE_AT_OV_AND_OC) )
		{
			ret_val=CANT_ENSURE_A_RIGHT_VAL;
		} 
		else if (given_obj->int_mode == NON_INTERRUPTABLE)
		{
			if(given_obj->t_mode == NORMAL_MODE)
			{
			   if(given_obj->timer_base == TIMER_0)
			   {
			      *return_res=(TIFR&0x01);
			   }
			   else if(given_obj->timer_base == TIMER_2)
			   {
			     *return_res=(TIFR&(1<<6));
			   }
			   else
			   {
                 //timer 1			 
			   }
			}
			else //ctc mode
			{
				   if(given_obj->timer_base == TIMER_0)
				   {
					   *return_res=(TIFR&(1<<1));
				   }
				   else if(given_obj->timer_base == TIMER_2)
				   {
					   *return_res=(TIFR&(1<<7));
				   }
				   else
				   {
					   //timer 1
				   }
			}			 
		}
		else
		{
			ret_val=INVALID_PARAMS;
		}
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}
	return ret_val;
}
timer_error_types read_timer_val(timer_object *given_obj,ecore_u16 *val)
{
	
	timer_error_types ret_val=NO_ERRORS;
	if ((given_obj != NULL) && (val!=NULL))
	{
		if(given_obj->timer_base == TIMER_0)
		{
			if(given_obj->t_mode == NORMAL_MODE)
			{
				*val=TCNT0 + ((TIFR&(1<<0))*MAX_VAL);
			}
			else if(given_obj->t_mode == CTC_MODE )
			{
				*val=TCNT0 + ((TIFR&(1<<1))*(given_obj->timer_comp_val));
			}
			else
			{
				ret_val=INVALID_PARAMS;
			}
		}
		else if(given_obj->timer_base == TIMER_2)
		{
				if(given_obj->t_mode == NORMAL_MODE)
				{
					*val=TCNT2 + ((TIFR&(1<<TOV2))*MAX_VAL);
				}
				else if(given_obj->t_mode == CTC_MODE )
				{
					*val=TCNT2 + ((TIFR&(1<<OCF2))*(given_obj->timer_comp_val));
				}
				else
				{
					ret_val=INVALID_PARAMS;
				}
			
		}
		else 
		{
			//timer 1
		}
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}
	return ret_val;
}
timer_error_types update_timer(timer_object *given_obj,ecore_u8 *val)
{
		timer_error_types ret_val=NO_ERRORS;
		if (given_obj != NULL)
		{
			given_obj->timer_start_val=*val;
			if (given_obj->timer_base == TIMER_0)
			{
				TCNT0=(ecore_u8)given_obj->timer_start_val;
			}
			else if(given_obj->timer_base == TIMER_2)
			{
				TCNT2=(ecore_u8)given_obj->timer_start_val;
			}
			else
			{
	            //timer 1
			}
			
		}
		else
		{
			ret_val=INVALID_PARAMS;
		}
		return ret_val;
	
}
timer_error_types update_comp_timer(timer_object *given_obj,ecore_u8 *val)
{
	timer_error_types ret_val=NO_ERRORS;
	if (given_obj != NULL)
	{
		given_obj->timer_comp_val=*val;
		if (given_obj->timer_base == TIMER_0)
		{
			OCR0=(ecore_u8)given_obj->timer_comp_val;
		}
		else if(given_obj->timer_base == TIMER_2)
		{
			OCR2=(ecore_u8)given_obj->timer_comp_val;
		}
		else
		{
			//timer 1
		}
		
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}
	return ret_val;
	
}
timer_error_types deinit_timer(timer_object *given_obj)
{
	timer_error_types ret_val=NO_ERRORS;
	if(given_obj != NULL)
	{
		given_obj->int_mode=0;
		given_obj->t_mode=0;
		given_obj->timer_freq=0;
		given_obj->timer_oc_mode=0;
		given_obj->timer_comp_val=0;
		given_obj->timer_start_val=0;
		*(volatile ecore_u8*)(given_obj->timer_base)=0;
		given_obj->timer_base=0;
	}
	else
	{
		ret_val =INVALID_PARAMS;
	}
	
	return ret_val;
}
