/*
 * hal_PWM.h
 *
 * Created: 24/10/2018 23:13:27
 *  Author: Mahmoud
 */ 


#ifndef HAL_PWM_H_
#define HAL_PWM_H_

#include <avr/io.h>
#include "hal_PWM_CFG.h"
#include "common.h"
#include <util/delay.h>
#define WGMn1 2
#define WGMn0 6
#define COMn 4

//the higher the period/the smaller the freq ,the higher the resolution

//rethink about all the coming i.e i need the formula or to get a val from it pwm 

typedef enum
{
	NOT_CONFIGED,
	CONFIGED,
}pwm_module_state_t;

typedef enum
{
	PCORRECT_PWM=1,
	FAST_PWM=3,
}pwm_operating_mode_t;

typedef enum
{
	NON_INVERTED=0x02,
	INVERTED=0x03,
}pwm_output_mode_t;//the inverted non-inverted modes

typedef enum
{
	TIMER_0=0x53,
	TIMER_1=0x4f,
	TIMER_2=0x45,
}timer_bases_t;

typedef enum
{
	OFF=0,
	NO_PRESCALER=1,
	PRESCALER_8=2,
	PRESCALER_64=3,
	PRESCALER_256=4,
	PRESCALER_1024=5,
	EXT_FALLING_EDGE=6,
	EXT_RISING_EDGE=7,
}prescaler_select_t;

typedef enum
{
	NO_PWM_ERRORS=0,
	INVALID_PWM_PARAMS,
	THE_NEEDED_PRESCALER_IMPOSSIBLE,
	MODULE_NOT_CONFIGED,
}pwm_error_t;

typedef enum
{
	ONE_PERCENT=1,TWO_PERCENT=2,
	FIVE_PERCENT=5,TEN_PERCENT=10,
	TWENTY_PERCENT=20,FIFTY_PERCENT=50,
}pwm_duty_cycle_t;//the inverted non-inverted modes

typedef struct
{
	timer_bases_t pwm_base_obj;
	pwm_output_mode_t pwm_mode_obj;
	pwm_operating_mode_t pwm_op_mode_obj;
	msa_u32 pwm_freq_obj;
	pwm_duty_cycle_t pwm_duty_cycle_obj;
	prescaler_select_t prescaler_obj;
	pwm_module_state_t pwm_module_config_state_obj;
}st_pwm_object;

pwm_error_t pwm_init(st_pwm_object*pwm_obj,timer_bases_t pwm_base ,pwm_output_mode_t pwm_mode 
,pwm_operating_mode_t pwm_op_mode,msa_u32 pwm_freq,pwm_duty_cycle_t pwm_duty_cycle);

pwm_error_t pwm_edit(st_pwm_object*pwm_obj,pwm_output_mode_t pwm_mode,pwm_operating_mode_t pwm_op_mode
,msa_u32 pwm_freq,pwm_duty_cycle_t pwm_duty_cycle);

pwm_error_t pwm_stop(st_pwm_object *pwm_obj);
pwm_error_t pwm_run(st_pwm_object *pwm_obj);
pwm_error_t pwm_deinit(st_pwm_object *pwm_obj);
// 
// #define Fout_FAST			(Fosc /(256*prescaler) )
// #define DUTYOUT_FAST		( ( (ocr+1) /256 ) *100 )
// #define Fout_PCORRECT		(Fosc /(512*prescaler) )
// #define DUTYOUT_PCORRECT	( ( (ocr+1) /512 ) *100 ) //the size of the high region
// 
// #define PRESCALER_FAST (Fosc/(256*Fout_FAST)) //i might check if he wants as inverted or as non and as fast or pcorr



/*
ocr=duty/x;//duty will be the res of the oring operation so now it's configurable
prescaler will be dedicated from an equation so will ask for the wanted freq. and apply the equation the decide if it's larger than xx.5 will be xx+1, i.e will sel or idnot remember

pwm_error_t pwm_init()
edit
stop
de-init
*/
#endif /* HAL_PWM_H_ */