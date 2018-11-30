/*
 * hal_PWM.c
 *
 * Created: 24/10/2018 23:13:48
 *  Author: Mahmoud
 */ 

#include "hal_PWM.h"

#define ON_PWM_SRC(PORT,PIN) (PORT |= (1<<PIN) )
#define OFF_PWM_SRC(PORT,PIN) (PORT &=~ (1<<PIN) )

pwm_error_t pwm_init(st_pwm_object*pwm_obj,timer_bases_t pwm_base ,pwm_output_mode_t pwm_mode ,pwm_operating_mode_t pwm_op_mode,msa_u32 pwm_freq,pwm_duty_cycle_t pwm_duty_cycle)
{
	pwm_error_t ret_val=NO_PWM_ERRORS;
	if ( ( (pwm_base==TIMER_0)||(pwm_base==TIMER_1)||(pwm_base==TIMER_2) ) && (pwm_freq != 0) && (pwm_obj != NULL)&&
		 ( (pwm_mode==INVERTED)||(pwm_mode==NON_INVERTED) ) && ( (pwm_op_mode == FAST_PWM) || (pwm_op_mode == PCORRECT_PWM) )&&
		 ( (pwm_duty_cycle >= 0) || (pwm_duty_cycle <= 100) )
       )
	{
		//store data in the reference object
		pwm_obj->pwm_base_obj=pwm_base;
		pwm_obj->pwm_duty_cycle_obj=pwm_duty_cycle;
		pwm_obj->pwm_freq_obj=pwm_freq;
		pwm_obj->pwm_mode_obj=pwm_mode;
		pwm_obj->pwm_op_mode_obj=pwm_op_mode;
		pwm_obj->pwm_module_config_state_obj=CONFIGED;
		//data storing done
		
		//initialization
		msa_u16 OcrRegVal;
		msa_f32  PrescalerNeeded;
		//getting the prescaler and the OCR register value needed
		if (pwm_op_mode == FAST_PWM)
		{
			PrescalerNeeded=(msa_f32)((msa_f32)Fosc/((msa_f32)pwm_freq*(msa_f32)256));
			
			if (pwm_mode == NON_INVERTED)
			{
				OcrRegVal=(((pwm_duty_cycle*256)/100)-1);
			}
			else //inverted
			{
				OcrRegVal=255-((256*pwm_duty_cycle)/100);
			}
		}
		else //phase_correct
		{
			PrescalerNeeded=(msa_f32)((msa_f32)Fosc/((msa_f32)pwm_freq*(msa_f32)510));
			
			if (pwm_mode == NON_INVERTED)
			{
				OcrRegVal=((pwm_duty_cycle*255)/100);
			}
			else //inverted
			{
				OcrRegVal=255-((255*pwm_duty_cycle)/100);
			}
		}
			 if (  PrescalerNeeded < (msa_f32)1.0) pwm_obj->prescaler_obj=NO_PRESCALER;
		else if ( (PrescalerNeeded > (msa_f32)1.0) && (PrescalerNeeded <(msa_f32)8.0) ) pwm_obj->prescaler_obj=PRESCALER_8;
		else if ( (PrescalerNeeded > (msa_f32)8.0) && (PrescalerNeeded <(msa_f32)64.0) ) pwm_obj->prescaler_obj=PRESCALER_64;
		else if ( (PrescalerNeeded > (msa_f32)64.0) && (PrescalerNeeded <(msa_f32)256.0) ) pwm_obj->prescaler_obj=PRESCALER_256;
		else if ( (PrescalerNeeded > (msa_f32)256.0) && (PrescalerNeeded <(msa_f32)1024.0) ) pwm_obj->prescaler_obj=PRESCALER_1024;
		else ret_val=THE_NEEDED_PRESCALER_IMPOSSIBLE; 
		
		
		*(volatile msa_u8*)(pwm_obj->pwm_base_obj)|= pwm_obj->prescaler_obj;
		*(volatile msa_u8*)(pwm_obj->pwm_base_obj)|= ( (( (pwm_obj->pwm_op_mode_obj) & 0x02)<< WGMn1) |((( (pwm_obj->pwm_op_mode_obj) & 0x01)<<WGMn0)) );
		*(volatile msa_u8*)(pwm_obj->pwm_base_obj)|= ( (pwm_obj->pwm_mode_obj)<<COMn); 
		if (pwm_base == TIMER_0)
		{
			OCR0=OcrRegVal;
		}
		else if (pwm_base == TIMER_1)
		{
			//timer 1
		}
		else if (pwm_base == TIMER_2)
		{
			OCR2=OcrRegVal;
		}
		
	} 
	else
	{
		ret_val=INVALID_PWM_PARAMS;
	}
	return ret_val;
}


pwm_error_t pwm_edit(st_pwm_object*pwm_obj,pwm_output_mode_t pwm_mode,pwm_operating_mode_t pwm_op_mode,msa_u32 pwm_freq,pwm_duty_cycle_t pwm_duty_cycle)
{
	pwm_error_t ret_val=NO_PWM_ERRORS;
	if ( (pwm_freq != 0) && (pwm_obj != NULL)&&
	( (pwm_mode==INVERTED)||(pwm_mode==NON_INVERTED) ) && ( (pwm_op_mode == FAST_PWM) || (pwm_op_mode == PCORRECT_PWM) )&&
	( (pwm_duty_cycle >= 0) || (pwm_duty_cycle <= 100) )
	)
	{
		if (pwm_obj->pwm_module_config_state_obj == CONFIGED)
		{
			ret_val=pwm_init(pwm_obj,pwm_obj->pwm_base_obj,pwm_mode,pwm_op_mode,pwm_freq,pwm_duty_cycle);
			
		} 
		else //not gonfiged :{ 
		{
			ret_val=MODULE_NOT_CONFIGED;
		}
	}
	else
	{
		ret_val=INVALID_PWM_PARAMS;
	}
	return ret_val;
}

pwm_error_t pwm_stop(st_pwm_object *pwm_obj)
{
	pwm_error_t ret_val=NO_PWM_ERRORS;
	if (pwm_obj != NULL)
	{
		*(volatile msa_u8*)(pwm_obj->pwm_base_obj)=0;
		
		if (pwm_obj->pwm_base_obj == TIMER_0)
		{
			OCR0=0;
			TCNT0=0;
			OFF_PWM_SRC(OCN0_PORT,OCN0_POS);
		}
		else if (pwm_obj->pwm_base_obj == TIMER_1)
		{
			//timer 1
			
			OFF_PWM_SRC(OCN1A_PORT,OCN1A_POS);
			OFF_PWM_SRC(OCN1B_PORT,OCN1B_POS);
		}
		else if (pwm_obj->pwm_base_obj == TIMER_2)
		{
			OCR2=0;
			TCNT2=0;
			OFF_PWM_SRC(OCN2_PORT,OCN2_POS);
		}
	}
	else
	{
		ret_val=INVALID_PWM_PARAMS;
	}
	return ret_val;
}


pwm_error_t pwm_run(st_pwm_object *pwm_obj)
{
	pwm_error_t ret_val=NO_PWM_ERRORS;
	if (pwm_obj != NULL)
	{
		if (pwm_obj->pwm_module_config_state_obj == CONFIGED)
		{
			//was going to store the registers in the stop func and restore them here but it would need more effort and i'm busy so, "???? ?????"
			//so if had time in the future,edit the stop and run functions
			pwm_init(pwm_obj,pwm_obj->pwm_base_obj,pwm_obj->pwm_mode_obj,pwm_obj->pwm_op_mode_obj,pwm_obj->pwm_freq_obj,pwm_obj->pwm_duty_cycle_obj);
		} 
		else
		{
			ret_val=MODULE_NOT_CONFIGED;
		}
	}
	else
	{
		ret_val=INVALID_PWM_PARAMS;
	}
	return ret_val;
}


pwm_error_t pwm_deinit(st_pwm_object *pwm_obj)
{
	pwm_error_t ret_val=NO_PWM_ERRORS;
	if (pwm_obj != NULL)
	{
		if (pwm_obj->pwm_module_config_state_obj == CONFIGED)
		{
			*(volatile msa_u8*)(pwm_obj->pwm_base_obj)=0x00;
			//store data in the reference object
			pwm_obj->pwm_base_obj=0;
			pwm_obj->pwm_duty_cycle_obj=0;
			pwm_obj->pwm_freq_obj=0;
			pwm_obj->pwm_mode_obj=0;
			pwm_obj->pwm_op_mode_obj=0;
			pwm_obj->pwm_module_config_state_obj=NOT_CONFIGED;
			//data storing done
			if (pwm_obj->pwm_base_obj == TIMER_0)
			{
				OCR0=0;
				TCNT0=0;
				OFF_PWM_SRC(OCN0_PORT,OCN0_POS);
			}
			else if (pwm_obj->pwm_base_obj == TIMER_1)
			{
				//timer 1
				
				OFF_PWM_SRC(OCN1A_PORT,OCN1A_POS);
				OFF_PWM_SRC(OCN1B_PORT,OCN1B_POS);
			}
			else if (pwm_obj->pwm_base_obj == TIMER_2)
			{
				OCR2=0;
				TCNT2=0;
				OFF_PWM_SRC(OCN2_PORT,OCN2_POS);
			}
		}
		else
		{
			ret_val=MODULE_NOT_CONFIGED;
		}
	}
	else
	{
		ret_val=INVALID_PWM_PARAMS;
	}
	return ret_val;
}