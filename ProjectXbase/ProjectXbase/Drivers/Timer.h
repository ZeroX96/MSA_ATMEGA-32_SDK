/*
 * Timer.h
 *
 * Created: 3/20/2020 5:39:26 AM
 *  Author: Mahmo
 */ 


#ifndef TIMER_H_
#define TIMER_H_
#include "HwAccess.h"
#include <stdint.h>
#include <avr/interrupt.h>

#ifndef NULL
#define NULL ((void*)0)
#endif
//int reg
#define TIMSK_REG	(0X59U)
#define TIFR_REG	(0X58U)
//timer 0
#define TCNT0_REG	(0X52U)
#define OCR0_REG	(0X5CU)
//timer 2
#define TCNT2_REG	(0X44U)
#define OCR2_REG	(0X43U)
//wgm bits
#define WGM0_BIT	(6U)
#define WGM1_BIT	(3U)
//ocie0 bits
#define TOVIE0_BIT	(0U)
#define OCIE0_BIT	(1U)
//Flag bits
#define TOV0_BIT	(0U)
#define OCF0_BIT	(1U)
//ocie2 bits
#define TOVIE2_BIT	(6U)
#define OCIE2_BIT	(7U)
//ocie2 bits
#define TOV2_BIT    (6U)
#define OCF2_BIT    (7U)
//comm bits
#define COM0_BIT	(4U)
#define COM1_BIT	(5U)
//foc bit
#define FOC_BIT		(7U)
//maximum val in an 8-bit Register
#define REG8BITS_MAX (256U)

typedef enum
{
	TMR_ERR_NO,
	TMR_ERR_INV_PARAMS,
}TimerRetT;

typedef enum
{
	TMR_NOT_INITIATED,
	TMR_INITIATED,
}TimerStateT;

typedef enum{
	OC_DISCONNECTED = 0x0,
	OC_TOGGLE		= 0x1,
	OC_CLEAR		= 0x2,
	OC_SET			= 0x3,
}TimerCoModeT;

typedef enum{
	CLK_OFF				= 0x00,
	CLK_NO_PRESCALER	= 0x01,
	CLK_PRESCALER_8		= 0x02,
	CLK_PRESCALER_64	= 0x03,
	CLK_PRESCALER_256	= 0x04,
	CLK_PRESCALER_1024	= 0x05,
	CLK_EXT_FALLING		= 0x06,
	CLK_EXT_RISING		= 0x07,
}TimerClkT;

typedef enum{
	INT_NO		=0x00,
	INT_OV		=0x01,
	INT_OC		=0x02,
	INT_OC_OV   =0x03,
}TimerIntModeT;

typedef enum{
	NORMAL_OP_MODE = 0x00,
	CTC_OP_MODE	   = 0x02,	
}TimerOpModeT;

typedef enum{
	TIMER_0 = 0x53,
	TIMER_2 = 0x45,
}TimerBaseT;

typedef void(*CallBkFuncT)(void);

typedef struct
{
	TimerBaseT		Base;
	TimerIntModeT	Int;
	TimerClkT		Clk;
	TimerOpModeT	Mode;
	TimerCoModeT	CompMatchMode;
	uint16_t		TcntVal;
	uint16_t		OcrVal;
	TimerStateT		State;
	CallBkFuncT		OvCallBkFunc;
	CallBkFuncT		OcCallBkFunc;
}TimerT;

TimerRetT TimerInit(TimerT* Timer);
TimerRetT TimerFOC(TimerT* Timer);
uint8_t	  TimerRead(TimerT* Timer);
TimerRetT TimerCountUpdate(TimerT* Timer,uint16_t Val);
TimerRetT TimerOcrUpdate(TimerT* Timer,uint16_t Val);
TimerRetT TimerDeInit(TimerT* Timer);

#endif /* TIMER_H_ */