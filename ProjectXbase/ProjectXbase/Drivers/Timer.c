/*
 * Timer.c
 *
 * Created: 3/20/2020 5:39:16 AM
 *  Author: Mahmo
 */ 
#include "Timer.h"

static CallBkFuncT T0_OV_CB;
static CallBkFuncT T0_OC_CB;

static CallBkFuncT T2_OV_CB;
static CallBkFuncT T2_OC_CB;

TimerRetT TimerInit(TimerT* Timer){
	if (NULL != Timer)
	{
		if (TIMER_0 == Timer->Base){
			HwWriteReg(TCNT0_REG,Timer->TcntVal);
			HwWriteReg(OCR0_REG,Timer->OcrVal);
			if (CTC_OP_MODE == Timer->Mode){
				HwSetRegBit(TIMER_0,WGM1_BIT);
			}
			
			if (Timer->Int == INT_OV){
				HwSetRegBit(TIMSK_REG,TOVIE0_BIT);
				T0_OV_CB = Timer->OvCallBkFunc;
			}else if(Timer->Int == INT_OC){
				HwSetRegBit(TIMSK_REG,OCIE0_BIT);
				T0_OC_CB = Timer->OcCallBkFunc;
			}else if (Timer->Int == INT_OC_OV){
				HwSetRegBit(TIMSK_REG,TOVIE0_BIT);
				HwSetRegBit(TIMSK_REG,OCIE0_BIT);
				T0_OV_CB = Timer->OvCallBkFunc;
				T0_OC_CB = Timer->OcCallBkFunc;
			}else{
				//DoNothing...Not Interruptible
			}
			
			if (Timer->Int != INT_NO)
			{
				//enable the gerneral interrupts flag
				sei();
			}
			if (Timer->CompMatchMode == OC_TOGGLE){
				HwSetRegBit(Timer->Base,COM0_BIT);
			}else if(Timer->CompMatchMode == OC_CLEAR){
				HwSetRegBit(Timer->Base,COM1_BIT);
			}else if(Timer->CompMatchMode == OC_SET){
				HwSetRegBit(Timer->Base,COM0_BIT);
				HwSetRegBit(Timer->Base,COM1_BIT);
			}else{
				//DoNothing
			}
			
		} 
		else if(TIMER_2 == Timer->Base){
			HwWriteReg(TCNT2_REG,Timer->TcntVal);
			HwWriteReg(OCR2_REG,Timer->OcrVal);
			if (CTC_OP_MODE == Timer->Mode){
				HwSetRegBit(TIMER_2,WGM1_BIT);
			}
			
			if (Timer->Int == INT_OV){
				HwSetRegBit(TIMSK_REG,TOVIE2_BIT);
				T2_OV_CB = Timer->OvCallBkFunc;
				}else if(Timer->Int == INT_OC){
				HwSetRegBit(TIMSK_REG,OCIE2_BIT);
				T2_OC_CB = Timer->OcCallBkFunc;
				}else if (Timer->Int == INT_OC_OV){
				HwSetRegBit(TIMSK_REG,TOVIE2_BIT);
				HwSetRegBit(TIMSK_REG,OCIE2_BIT);
				T2_OV_CB = Timer->OvCallBkFunc;
				T2_OC_CB = Timer->OcCallBkFunc;
				}else{
				//DoNothing...Not Interruptible
			}
			
			if (Timer->CompMatchMode == OC_TOGGLE){
				HwSetRegBit(Timer->Base,COM0_BIT);
			}else if(Timer->CompMatchMode == OC_CLEAR){
				HwSetRegBit(Timer->Base,COM1_BIT);
			}else if(Timer->CompMatchMode == OC_SET){
				HwSetRegBit(Timer->Base,COM0_BIT);
				HwSetRegBit(Timer->Base,COM1_BIT);
			}else{
				//DoNothing
			}
			
		}
		else{
			return TMR_ERR_INV_PARAMS;
		}
		
		HwWriteReg(Timer->Base,(HwReadReg(Timer->Base) & ~(7<<0)) | Timer->Clk );
		return TMR_ERR_NO;
	}
	return TMR_ERR_INV_PARAMS;
}

TimerRetT TimerFOC(TimerT* Timer){
	HwSetRegBit(Timer->Base,FOC_BIT);
}

uint8_t TimerRead(TimerT* Timer){
	if(NULL != Timer){
		if (Timer->Base == TIMER_0){
			if (Timer->Mode == NORMAL_OP_MODE){
				return (HwReadReg(TCNT0_REG) + (HwIsBitSet(TIFR_REG,TOV0_BIT) * REG8BITS_MAX)  );
			}else if(Timer->Mode == CTC_OP_MODE){
				return (HwReadReg(TCNT0_REG) + (HwIsBitSet(TIFR_REG,OCF0_BIT) * Timer->OcrVal)  );
			}
		} 
		else if(Timer->Base == TIMER_2){
			if (Timer->Mode == NORMAL_OP_MODE){
				return (HwReadReg(TCNT2_REG) + (HwIsBitSet(TIFR_REG,TOV2_BIT) * REG8BITS_MAX)  );
			}else if(Timer->Mode == CTC_OP_MODE){
				return (HwReadReg(TCNT2_REG) + (HwIsBitSet(TIFR_REG,OCF2_BIT) * Timer->OcrVal)  );
			}
		}else{
			return TMR_ERR_INV_PARAMS;
		}
	}
	return TMR_ERR_INV_PARAMS;
}

TimerRetT TimerCountUpdate(TimerT* Timer,uint16_t Val){
	if (Timer != NULL){
		if (Timer->Base == TIMER_0){
			HwWriteReg(TCNT0_REG,Val);
		}else if (Timer->Base == TIMER_2){
			HwWriteReg(TCNT2_REG,Val);
		}else{
			return TMR_ERR_INV_PARAMS;
		}
		return TMR_ERR_NO;
	}
	return TMR_ERR_INV_PARAMS;
}

TimerRetT TimerOcrUpdate(TimerT* Timer,uint16_t Val){
	if (Timer != NULL){
		if (Timer->Base == TIMER_0){
				HwWriteReg(OCR0_REG,Val);
			}else if (Timer->Base == TIMER_2){
				HwWriteReg(OCR2_REG,Val);
			}else{
				return TMR_ERR_INV_PARAMS;
			}
		return TMR_ERR_NO;
	}
	return TMR_ERR_INV_PARAMS;
}

TimerRetT TimerDeInit(TimerT* Timer){
	if (Timer != NULL){
		HwWriteReg(Timer->Base,0x00);
		if (Timer->Int != INT_NO)
		{
			cli();
		}
	}
}

ISR(TIMER0_OVF_vect){
	if (NULL != T0_OV_CB)
	{
		(*T0_OV_CB)();
	}
}
ISR(TIMER0_COMP_vect){
	if (NULL != T0_OC_CB)
	{
		(*T0_OC_CB)();
	}
	
}
ISR(TIMER2_OVF_vect){
	if (NULL != T2_OV_CB)
	{
		(*T2_OV_CB)();
	}
	
}
ISR(TIMER2_COMP_vect){
	if (NULL != T2_OC_CB)
	{
		(*T2_OC_CB)();
	}
	
}