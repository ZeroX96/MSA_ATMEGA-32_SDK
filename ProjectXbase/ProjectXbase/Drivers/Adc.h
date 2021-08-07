/*
 * Adc.h
 *
 * Created: 3/20/2020 5:37:47 AM
 *  Author: Mahmo
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include "HwAccess.h"
#include "Gpio.h"

#define ADC_BASE BASE_A
#define ADC_PIN  PIN_0
//Registers In the ADC module for control and status
#define ADC_MUX		(0x27)
#define ADC_CSRA	(0x26)
#define ADC_H		(0x25)
#define ADC_L		(0x24)
#define ADC_SFIO	(0x50)

//ADMUX bits for the adc module
#define REFS_0_BIT	(6U)
#define ADLAR_BIT	(5U)
#define MUX_0_BIT	(0U)

//ADCSRA bits for the adc module
#define ADEN_BIT	(7U) //will set the bit to TRUE
#define ADSC_BIT	(6U) // will be used with adc_read
#define ADATE_BIT	(5U)
#define ADIF_BIT	(4U) //will be used after reading res with adc read
#define ADIE_BIT	(3U)
#define ADPS_0_BIT	(0U)
//SFIOR bits for the adc module
#define ADTS_0_BIT	(5U)

#ifndef NULL
#define NULL ((void*)0)
#endif	//NULL

typedef void (*AdcCallBackFunT)(void);

typedef enum{
	ADC_ERR_NO  = 0x00,
	ADC_INV_PARAMS	= 0x01,
}AdcRetT;
typedef enum{
	AREF		= 0x00,
	AVCC		= 0x01,
	INTERNAL256 = 0x03,
}AdcRefSelectT;

typedef enum{
	RIGHT_ADJ = 0x00,
	LEFT_ADJ  = 0x01,
}AdcResultAdjT;

typedef enum{
	AT_DISABLE  = 0x00,
	AT_ENABLE	= 0x01,
}AdcAutoTrigT;

typedef enum{
	INTURREPT_DISABLE  = 0x00,
	INTURREPT_ENABLE	= 0x01,
}AdcIntModeT;

typedef enum{
	DIV_BY_2	= 0x00,
	DIV_BY_2TOO	= 0x01,
	DIV_BY_4	= 0x02,
	DIV_BY_8	= 0x03,
	DIV_BY_16	= 0x04,
	DIV_BY_32	= 0x05,
	DIV_BY_64	= 0x06,
	DIV_BY_128	= 0x07,
}AdcPrescalerT;

typedef enum{
	//SingleEndedInputChannel
	ADC_CH0_SINGLE		=0x00,
	ADC_CH1_SINGLE		=0x01,
	ADC_CH2_SINGLE		=0x02,
	ADC_CH3_SINGLE		=0x03,
	ADC_CH4_SINGLE		=0x04,
	ADC_CH5_SINGLE		=0x05,
	ADC_CH6_SINGLE		=0x06,
	ADC_CH7_SINGLE		=0x07,
	//Pos_W_NeG_channels with Gain = 10
	ADC_CH0_W_CH0_G10	=0x08,
	ADC_CH1_W_CH0_G10	=0x09,
	ADC_CH2_W_CH2_G10	=0x0C,
	ADC_CH3_W_CH2_G10	=0x0D,
	//Pos_W_NeG_channels with Gain = 200
	ADC_CH0_W_CH0_G200	=0x0A,
	ADC_CH1_W_CH0_G200	=0x0B,
	ADC_CH2_W_CH2_G200	=0x0E,
	ADC_CH3_W_CH2_G200	=0x0F,
	//Pos_W_CH1_channels with Gain = 1
	ADC_CH0_W_CH1_G1,
	ADC_CH1_W_CH1_G1,
	ADC_CH2_W_CH1_G1,
	ADC_CH3_W_CH1_G1,
	ADC_CH4_W_CH1_G1,
	ADC_CH5_W_CH1_G1,
	ADC_CH6_W_CH1_G1,
	ADC_CH7_W_CH1_G1,
	//Pos_W_CH2_channels with Gain = 1
	ADC_CH0_W_CH2_G1,
	ADC_CH1_W_CH2_G1,
	ADC_CH2_W_CH2_G1,
	ADC_CH3_W_CH2_G1,
	ADC_CH4_W_CH2_G1,
	ADC_CH5_W_CH2_G1,
	//SingleEndedInput
	ADC_1v22,
	ADC_0v,
}AdcChannelSelT;

typedef enum{
	FREE_RUNNING_M		= 0x00,
	ANALOG_COMPARATOR	= 0x01,
	EXTERNAL_INT_0		= 0x02,
	TIMER_0_CMP_MATCH	= 0x03,
	TIMER_0_OVERFLOW	= 0x04,
	TIMER_1_CMP_MATCH	= 0x05,
	TIMER_1_OVERFLOW	= 0x06,
	TIMER_1_CAPTURE		= 0x07,
}AdcTriggerSrcT;

typedef struct  
{
	AdcResultAdjT	ResAdj;
	AdcRefSelectT	RefSel;
	AdcIntModeT		IntMode;
	AdcTriggerSrcT	TrigSrc;
	AdcPrescalerT	PrescSel;
	AdcAutoTrigT	AutoTrig;
	AdcChannelSelT  Channel;
	AdcCallBackFunT CALLBackFunc;
}AdcT;

AdcRetT AdcInit	 (AdcT* Adc);
AdcRetT AdcRead	 (AdcT* Adc,uint16_t* ReadValue);
AdcRetT AdcEdit	 (AdcT* Adc);
AdcRetT AdcDeInit(AdcT* Adc);


#endif /* ADC_H_ */