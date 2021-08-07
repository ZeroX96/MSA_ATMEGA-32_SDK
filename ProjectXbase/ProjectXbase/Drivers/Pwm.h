/*
 * Pwm.h
 *
 * Created: 3/20/2020 5:38:37 AM
 *  Author: Mahmo
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "HwAccess.h"
#include <stdint.h>
#include "Gpio.h"
#ifndef F_CPU
#define F_CPU 8000000UL
#endif //F_CPU

#ifndef NULL
#define NULL ((void*)0)
#endif

#define OCN0_BASE BASE_B
#define OCN0_PIN	(3U)

#define OCN2_BASE BASE_D
#define OCN2_PIN	(7U)

#define PWM_SYS_CLOCK  F_CPU


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

typedef enum{
	FAST_PWM			=0x03,
	PHASE_CORRECT_PWM	=0x01,
}PwmOperatingModeT;

typedef enum{
	INVERTING_OUTPUT		=0x03,	//in FAST_PWM >>the output is set on compare match and cleared at BOTTOM.
									//in PHASE_COR>>the Output Compare (OC0) is Set on the compare match
									//				between TCNT0 and OCR0 while upcounting, and Cleared on 
									//				the compare match while downcounting.
	NON_INVERTING_OUTPUT	= 0x02,	//in FAST_PWM >>the Output Compare (OC0) is cleared on the compare
									//				match between TCNT0 and OCR0, and set at BOTTOM
									//in PHASE_COR>>the Output Compare (OC0) is cleared on the compare match
									//				between TCNT0 and OCR0 while upcounting, and set on the compare
									//				match while downcounting.
}PwmOutputModeT;


typedef enum{
	PWM_OFF				= 0x00,
	PWM_NO_PRESCALER	= 0x01,
	PWM_PRESCALER_8		= 0x02,
	PWM_PRESCALER_64	= 0x03,
	PWM_PRESCALER_256	= 0x04,
	PWM_PRESCALER_1024	= 0x05,
	PWM_EXT_FALLING		= 0x06,
	PWM_EXT_RISING		= 0x07,
}PwmPrescallerT;

typedef enum{
	PWM_TIMER_0 = 0x53,
	PWM_TIMER_2 = 0x45,
}PwmBaseT;

typedef enum{
	PWM_ERR_NO,
	PWM_ERR_INV_PARAMS,
	PWM_ERR_IMP_PRESCALLER,
}PwmRetT;

typedef enum{
	ONE_PERCENT		= 0x01,
	TWO_PERCENT		= 0x02,
	FIVE_PERCENT	= 0x05,
	TEN_PERCENT		= 0x0A,
	TWENTY_PERCENT	= 0x14,
	FIFTY_PERCENT	= 0x32,
}PWmDutyCycleT;

typedef struct{
	PwmBaseT			Base;
	PwmOutputModeT		OutputMode;
	PwmOperatingModeT   OperatingMode;
	PwmPrescallerT		Prescaller;
	PWmDutyCycleT		DutyCycle;
	uint32_t			Freq;
}PwmT;

PwmRetT PwmInit(PwmT* Pwm, PwmBaseT PwmBase,	     \
				PwmOutputModeT		PwmOutputMode,   \
				PWmDutyCycleT		PWmDutyCycle,    \
				PwmOperatingModeT	PwmOperatingMode,\
				uint32_t			PwmFrequency);
PwmRetT PwmEdit(PwmT* Pwm,PwmOutputModeT PwmOutputMode,\
				PWmDutyCycleT PWmDutyCycle,\
				PwmOperatingModeT PwmOperatingMode,\
				uint32_t PwmFrequency);
PwmRetT PwmRun(PwmT* Pwm);
PwmRetT PwmStop(PwmT* Pwm);
PwmRetT PwmDeInit(PwmT* Pwm);

#endif /* PWM_H_ */

#if 0
Fast PWM and Phase Correct MODEs
	inverting and non-inverting modes
OCn0 OCn1 OCn2 pins
the data direction of the pin must be set to output
Focn = Fclk/(N.256)	>	>Fast PWM
	N = Fclk / Focn*256
Focn = Fclk/(N.510)	>	>PhaseCorrect PWM
		The N variable represents the prescale factor
#endif //0