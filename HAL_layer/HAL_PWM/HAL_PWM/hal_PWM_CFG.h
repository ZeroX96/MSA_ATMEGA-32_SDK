/*
 * hal_PWM_CFG.h
 *
 * Created: 26/10/2018 03:28:08
 *  Author: Mahmoud
 */ 


#ifndef HAL_PWM_CFG_H_
#define HAL_PWM_CFG_H_
//U need to define the frequency the mcu working on
#ifndef Fosc
#warning "Fosc is not defined in the config file so it's default as 1Mhz"
#define  Fosc 1000000UL
#endif
 
//these are used in stop and run the pwm modul functions
//edit to meet your mcu config.s and pwm sources positions
#define OCN0_PORT PORTB
#define OCN0_POS 3

#define OCN1A_PORT PORTD
#define OCN1A_POS 4

#define OCN1B_PORT PORTD
#define OCN1B_POS 5

#define OCN2_PORT PORTD
#define OCN2_POS 7

#endif /* HAL_PWM_CFG_H_ */