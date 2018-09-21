/*
 * hal_GPIO.h
 *
 * Created: 18/09/2018 22:26:44
 *  Author: Mahmoud
 */ 


#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_
#include <avr/io.h>
#include "common.h"
#define DDR_OFFSET  1
#define PORT_OFFSET 2

typedef enum
{
	NO_ERRORS,INVALID_PARAMS,	
}gpio_errors;

typedef enum
{
	BASE_D=0x30,BASE_C=0x33,
	BASE_B=0x36,BASE_A=0x39,
}gpio_bases;

typedef enum
{
	PIN_ZERO,PIN_ONE,PIN_TWO,PIN_THREE,
	PIN_FOUR,PIN_FIVE,PIN_SIX,PIN_SEVEN,
	
}gpio_pin_no;

gpio_errors set_port_output	(gpio_bases *base);
gpio_errors set_pin_output	(gpio_bases *base,gpio_pin_no pin_no);
gpio_errors write_port		(gpio_bases *base,msa_u8 *val);
gpio_errors set_pin_high(gpio_bases *base,gpio_pin_no pin_no);
gpio_errors set_pin_low(gpio_bases *base,gpio_pin_no pin_no);

//dont need any port or pin as an input so deleted with
#if 0
//note making the port input will activate the internal pull up resistor according to this driver :b
gpio_errors set_port_input	(gpio_bases *base);
gpio_errors set_pin_input	(gpio_bases *base,gpio_pin_no pin_no);
gpio_errors read_port		(gpio_bases *base,msa_u8 *val);
gpio_errors read_pin		(gpio_bases *base,gpio_pin_no pin_no,bool_t *val);
#endif




#endif /* HAL_GPIO_H_ */