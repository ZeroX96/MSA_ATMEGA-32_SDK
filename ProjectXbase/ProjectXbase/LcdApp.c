/*
 * LcdApp.c
 *
 * Created: 4/14/2020 9:43:52 PM
 *  Author: Mahmo
 */ 
#include "LcdApp.h"

extern uint8_t MotorSpeed;
extern int8_t  SwingVal;
extern int8_t  BuzzDelay;

int16_t DispBDelay = 0;

static uint8_t DispButtonCounter;
static LcdT	   LcdApp;

#define PositiveButton
static _Bool DispBStateOld;
static _Bool DispBStateCur;
static _Bool DispBProcessSt;

static uint8_t* SpeedDataFLine = "  Motor Speed   ";
static uint8_t* SpeedDataSLine1 = "Speed= ";
static uint8_t* SpeedDataSLine2 = " Degree";

static uint8_t* SwingDataFLine = "  Swing  Value  ";

static uint8_t* SwingAngleShowArr[9] = {"                ",
										"       **       ",
										"      ****      ",
										"     ******     ",
										"    ********    ",
										"   **********   ",
										"  ************  ",
										" ************** ",
										"****************"};

void LcdAppInit(){
	GpioInitPin(LCD_B_BASE,LCD_B_PIN,PIN_IN);
	LcdApp.DataBase = BASE_C;
	LcdApp.CommBase = BASE_C;
	LcdInit(&LcdApp);
}

void CheckDispBChange(){
	DispBStateCur = GpioReadPin(LCD_B_BASE,LCD_B_PIN);
	
	if ((DispBStateCur == TRUE) && (DispBStateOld == FALSE)){
		DispBStateOld = TRUE;
		DispBStateCur = FALSE;
	}
	else if ((DispBStateCur == FALSE) && (DispBStateOld == TRUE)){
		DispBStateOld = FALSE;
	}
	else if ((DispBStateCur == TRUE) && (DispBStateOld == TRUE)){
		DispButtonCounter++;
	}
	else{
		//DoNothing
	}
	if (DispButtonCounter >= 20){
		DispButtonCounter = 0;
		DispBStateOld = FALSE;
		DispBStateCur = FALSE;
		DispBProcessSt = TRUE;
		DispBDelay = TWENTY_SEC;
		BuzzDelay = 20;
	}
}

void ProcessDispBChange(){
	
	if (DispBProcessSt == FALSE){
		LcdCommOut(&LcdApp,BEGIN_FROM_1ST_LINE);
		LcdArrDataOut(&LcdApp,SpeedDataFLine);
		LcdCommOut(&LcdApp,BEGIN_FROM_2ND_LINE);
		LcdArrDataOut(&LcdApp,SpeedDataSLine1);
		LcdCharDataOut(&LcdApp,(MotorSpeed/10) + ASCII_BASE);
		LcdCharDataOut(&LcdApp,(MotorSpeed%10) + ASCII_BASE);
		LcdArrDataOut(&LcdApp,SpeedDataSLine2);
	}
	else{
		LcdCommOut(&LcdApp,BEGIN_FROM_1ST_LINE);
		LcdArrDataOut(&LcdApp,SwingDataFLine);
		LcdCommOut(&LcdApp,BEGIN_FROM_2ND_LINE);
		LcdArrDataOut(&LcdApp,SwingAngleShowArr[SwingVal]);
		if (DispBDelay <= 0){
			DispBProcessSt = FALSE;	
		}
	}
	//return LCD_ERR_NO;
}
