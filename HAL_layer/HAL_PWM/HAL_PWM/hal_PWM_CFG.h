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
#define  Fosc				1000000UL
#endif



#endif /* HAL_PWM_CFG_H_ */