/*
 * TimerApp.c
 *
 * Created: 4/13/2020 6:10:44 PM
 *  Author: Mahmo
 */ 
#include "TimerApp.h"

extern int8_t BuzzDelay;

static TimerT TimerApp;

#define TimerButton
static _Bool TimerBStateOld;
static _Bool TimerBStateCur;
static _Bool TimerBProcessSt;

_Bool SysAutoOffSt = FALSE;

uint8_t SysClkTick = 0;	//incremented every 10ms

int8_t SysHours = 0;
int8_t SysMinuts = 0;
int8_t SysSeconds = 0;
static _Bool SecPassedSt = FALSE;

void SysTimerInit(void(*TimerCBF)(void)){
	
	GpioInitPin(CLK_TOGG_LED_BASE,CLK_TOGG_LED_PIN,PIN_OUT);
	GpioInitPin(TIMER_B_BASE,TIMER_B_PIN,PIN_IN);
	TimerApp.Base = TIMER_0;
	TimerApp.Clk = CLK_PRESCALER_1024;
	TimerApp.CompMatchMode = OC_DISCONNECTED;
	TimerApp.Int = INT_OC;	//handle the isr
	TimerApp.Mode = CTC_OP_MODE;
	TimerApp.OcrVal = 77;
	TimerApp.TcntVal = 0;
	TimerApp.OcCallBkFunc = TimerCBF;
	TimerInit(&TimerApp);
}

void ProcessTimeOut(){
	static _Bool LedToggle =FALSE;
	if (SysClkTick % HALF_SEC == 0 ){
		if (LedToggle == FALSE){
			LedToggle = TRUE;
			GpioWritePin(CLK_TOGG_LED_BASE,CLK_TOGG_LED_PIN,PIN_HIGH);
		}
		else{
			LedToggle = FALSE;
			GpioWritePin(CLK_TOGG_LED_BASE,CLK_TOGG_LED_PIN,PIN_LOW);
		}
	}
}

void CheckTimeOut(){
	
	if (SysClkTick < FULL_SEC){
		SysClkTick++;
	}
	else{
		SysClkTick = 0;
		SecPassedSt = TRUE;
	}
	//every complete second
	if (SecPassedSt == TRUE){
		SecPassedSt = FALSE;
		
		if (SysSeconds > 0){
			SysSeconds--;
		}
		else{
			if (SysMinuts > 0){
				SysMinuts--;
				SysSeconds = 59;
			} 
			else{
				if (SysHours > 0){
					SysHours--;
					SysMinuts = 59;
					SysSeconds = 59;
				} 
				else{
					SysAutoOffSt = TRUE;//system off => SysTimeOut
				}
			}
		}
	}
}

void ProcessTimerBChange(){
	if (TimerBProcessSt == TRUE){
		BuzzDelay = 20;
		if (SysHours < 4){
			SysHours++;
		}
		else{
			SysHours = 0;
		}
		SysMinuts = 0;
		SysSeconds = 0;
		TimerBProcessSt = FALSE;
		TimerBStateOld  = FALSE;
		TimerBStateCur  = FALSE;
	}
}

void CheckTimerBChange()
{
	TimerBStateCur = GpioReadPin(TIMER_B_BASE,TIMER_B_PIN);
	
	if ((TimerBStateCur == TRUE) && (TimerBStateOld == FALSE)){
		TimerBStateOld = TRUE;
		TimerBStateCur = FALSE;
	}
	else if ((TimerBStateCur == FALSE) && (TimerBStateOld == TRUE)){
		TimerBStateOld = FALSE;
	}
	else if ((TimerBStateCur == TRUE) && (TimerBStateOld == TRUE)){
		TimerBStateCur = FALSE;
		TimerBStateOld = FALSE;
		TimerBProcessSt = TRUE;
		if (SysAutoOffSt == TRUE){
			SysAutoOffSt = FALSE;
		}
	}
	else{
		//DoNothing
	}
}