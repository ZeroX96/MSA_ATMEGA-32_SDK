/*
 * SpeedApp.c
 *
 * Created: 4/13/2020 8:42:24 AM
 *  Author: Mahmo
 */ 
#include "SpeedApp.h"


extern int16_t DispBDelay;
extern int8_t  BuzzDelay;

//rethink about it
uint8_t MotorSpeed;
static PwmT PwmApp;

#define PositiveButton
static _Bool PosBStateOld;
static _Bool PosBStateCur;
static _Bool PosBProcessSt;

#define NegativeButton
static _Bool NegBStateOld;
static _Bool NegBStateCur;
static _Bool NegBProcessSt;	//process zero(cur,old) if old & cur = TRUE

void SpeedButtonsInit(){
	GpioInitPin(POS_B_BASE,POS_B_PIN,PIN_IN);
	GpioInitPin(NEG_B_BASE,NEG_B_PIN,PIN_IN);
	MotorSpeed = MOTOR_DEFAULT_SPEED;
	PwmInit(&PwmApp,PWM_TIMER_2,INVERTING_OUTPUT,MotorSpeed,FAST_PWM,MOTOR_FREQUENCY);
	//set the pin output
	//GpioInitPin(BASE_D,PIN_7,PIN_OUT);
	//PwmEnableSrc(PwmApp); not needed
}

void ProcessNBChange(){
	if (NegBProcessSt == TRUE){
		BuzzDelay = 20;
		if (MotorSpeed == MOTOR_MAX_SPEED){
			MotorSpeed = MOTOR_MID_SPEED;
		}
		else if(MotorSpeed == MOTOR_MID_SPEED){
			MotorSpeed = MOTOR_MIN_SPEED;
		}
		else{
			//do nothing
		}
		NegBProcessSt = FALSE;
		NegBStateOld  = FALSE;
		NegBStateCur  = FALSE;
	}
	PwmEdit(&PwmApp,INVERTING_OUTPUT,MotorSpeed,FAST_PWM,MOTOR_FREQUENCY);
}

void ProcessPBChange(){
	if (PosBProcessSt == TRUE){
		BuzzDelay = 20;
		if (MotorSpeed == MOTOR_MIN_SPEED){
			MotorSpeed = MOTOR_MID_SPEED;
		}
		else if(MotorSpeed == MOTOR_MID_SPEED){
			MotorSpeed = MOTOR_MAX_SPEED;
		}
		else{
			//do nothing
		}
		PosBProcessSt = FALSE;
		PosBStateOld  = FALSE;
		PosBStateCur  = FALSE;
	}
	PwmEdit(&PwmApp,INVERTING_OUTPUT,MotorSpeed,FAST_PWM,MOTOR_FREQUENCY);
}

void CheckPBChange(){
	PosBStateCur = GpioReadPin(POS_B_BASE,POS_B_PIN);
	
	if ((PosBStateCur == TRUE) && (PosBStateOld == FALSE)){
		PosBStateOld = TRUE;
		PosBStateCur = FALSE;
	}
	else if ((PosBStateCur == FALSE) && (PosBStateOld == TRUE)){
		PosBStateOld = FALSE;
	}
	else if ((PosBStateCur == TRUE) && (PosBStateOld == TRUE)){
		PosBStateCur = FALSE;
		PosBStateOld = FALSE;
		PosBProcessSt = TRUE;
		DispBDelay = 0;
	}
	else{
		//DoNothing
	}
}

void CheckNBChange(){
	NegBStateCur = GpioReadPin(NEG_B_BASE,NEG_B_PIN);
	
	if ((NegBStateCur == TRUE) && (NegBStateOld == FALSE)){
		NegBStateOld = TRUE;
		NegBStateCur = FALSE;
	}
	else if ((NegBStateCur == FALSE) && (NegBStateOld == TRUE)){
		NegBStateOld = FALSE;
	}
	else if ((NegBStateCur == TRUE) && (NegBStateOld == TRUE)){
		NegBStateCur  = FALSE;
		NegBStateOld  = FALSE;
		NegBProcessSt = TRUE;
		DispBDelay = 0;
	}
	else{
		//DoNothing
	}
}