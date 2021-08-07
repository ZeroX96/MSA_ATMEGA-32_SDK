/*
 * MotorSwingApp.c
 *
 * Created: 4/2/2020 9:53:02 PM
 *  Author: Mahmo
 */ 
#include "MotorSwingApp.h"

int8_t  SwingVal= 0;
// uint16_t ReadVal2;
 
#define SwingCtrl
static uint16_t SwingAdcReadOld = 0;
static uint16_t SwingAdcReadCur = 0;

static uint16_t SwingAdcReadOldTry = 0;
static uint16_t SwingAdcReadCurTry = 0;

static _Bool SwingAdcChangeSt = 0;//if abs cur - old >= 11 >> SwingProcess
static AdcT SwingMotoCtrl;


SwingRetT SwingMotoSetAngle(SwingAngleT MotoAngle);


/*
moto angle 0 1 0 1
				 1		
			   1
			 1
		   1	
*/
SwingRetT SwingMotoInit()
{
	
	SwingMotoCtrl.AutoTrig = AT_DISABLE;
	SwingMotoCtrl.CALLBackFunc = NULL;
	SwingMotoCtrl.Channel = ADC_CH0_SINGLE;
	SwingMotoCtrl.IntMode = INTURREPT_DISABLE;
	SwingMotoCtrl.PrescSel = DIV_BY_64;
	SwingMotoCtrl.RefSel = AVCC;
	SwingMotoCtrl.ResAdj = RIGHT_ADJ;
	SwingMotoCtrl.TrigSrc = FREE_RUNNING_M;
	
	AdcInit(&SwingMotoCtrl);
	GpioInitPin(BASE_A,PIN_0,PIN_LOW);
	//HwSetRegBit(BASE_A+PORTx_OFFSET, PIN_0);
	uint8_t i;
	for (i = S_MOTOR_F_PIN;i < (S_MOTOR_F_PIN + 4); i++){
		GpioInitPin(S_MOTOR_PORT,i,PIN_OUT);
	}
	return SWING_ERR_NO;
}


SwingRetT SwingMotoSetAngle(SwingAngleT MotoAngle){
	uint8_t SwingState,i;
	for(i = 0;i < 4;i++){
		SwingState = MotoAngle & (1<<i);
		if(SwingState != 0){
			GpioWritePin(S_MOTOR_PORT,(S_MOTOR_F_PIN+i),PIN_HIGH);
		}
		else{
			GpioWritePin(S_MOTOR_PORT,(S_MOTOR_F_PIN+i),PIN_LOW);
		}
	}
	return SWING_ERR_NO;
}

SwingRetT CheckSwingChange(){
	
	AdcRead(&SwingMotoCtrl,&SwingAdcReadCur);
	//SwingAdcReadCur = ReadVal2;
	if (abs(SwingAdcReadCur - SwingAdcReadOld) >= SwingMotoValRange ){
		SwingAdcChangeSt = TRUE;
	}
	SwingAdcReadCurTry = SwingAdcReadCur;
	SwingAdcReadOldTry = SwingAdcReadOld;
	
	SwingAdcReadOld = SwingAdcReadCur;
	//ReadVal2;
	return SWING_ERR_NO;
}

SwingRetT ProcessSwingChange(){
	if (SwingAdcChangeSt == TRUE){
		if (SwingAdcReadCurTry > SwingAdcReadOldTry){
			if (SwingVal < 9){
				SwingVal++;
			}
		}
		else{
			if (SwingVal > 0){
				SwingVal--;
			}
		}
	SwingAdcReadOldTry = SwingAdcReadCurTry;
		SwingMotoSetAngle(SwingVal);
		SwingAdcChangeSt = FALSE;
	}
	return SWING_ERR_NO;
}

