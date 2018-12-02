/*
 * hal_I2C.h
 *
 * Created: 05/11/2018 03:35:06
 *  Author: Mahmoud
 */ 


#ifndef HAL_I2C_H_
#define HAL_I2C_H_
#include <avr/io.h>
#include "common.h"


#define TWBR0_OFFSET 0
#define TWSR0_OFFSET 1
#define TWAR0_OFFSET 2
#define TWDR0_OFFSET 3
#define TWCR0_OFFSET 54

#define TWBR0	(*(volatile msa_u8 *)MODULE_0+TWBR0_OFFSET)
#define TWSR0	(*(volatile msa_u8 *)MODULE_0+TWSR0_OFFSET)
#define TWAR0	(*(volatile msa_u8 *)MODULE_0+TWAR0_OFFSET)
#define TWDR0	(*(volatile msa_u8 *)MODULE_0+TWDR0_OFFSET)
#define TWCR0	(*(volatile msa_u8 *)MODULE_0+TWCR0_OFFSET)

typedef enum
{
	MODULE_0=0x20,
	//if the MCU have more i2c drivers add their add here   
}twi_module_base_t;
typedef enum
{
	POLLING_BASED,
	INTERRUPT_BASED,
}twi_notifics_mode_t; 

typedef enum 
{
	NO_I2C_ERRORS,
	INVALID_I2C_PARAMS,
}twi_error_t;
typedef enum
{
	ACK_NOT_EN,
	ACK_IS_EN,
}twi_ack_mode_t;
typedef enum
{
	I2C_NOT_INITIATED,
	I2C_INITIATED,
}twi_module_state_t;

typedef struct  
{
	twi_ack_mode_t			ack_mode_obj;
	twi_module_base_t		module_base_obj;
	twi_notifics_mode_t		notifics_mode_obj;
	twi_module_state_t		module_state_obj;
}twi_obj_create_t;
twi_error_t twi_init_module(twi_obj_create_t *twi_config_obj,twi_module_base_t module_base,twi_ack_mode_t ack_mode,twi_notifics_mode_t notifics_mode);
twi_error_t twi_set_slave_add(twi_obj_create_t *twi_config_obj,msa_u8 slave_add);
twi_error_t twi_send_a_start(twi_obj_create_t *twi_config_obj);
twi_error_t twi_send_a_stop(twi_obj_create_t *twi_config_obj);
twi_error_t twi_send_byte(twi_obj_create_t *twi_config_obj,msa_u8 *data_byte);
twi_error_t twi_send_packet(twi_obj_create_t *twi_config_obj,msa_u8 *data_add,msa_u8 data_size);
twi_error_t twi_receive_byte(twi_obj_create_t *twi_config_obj,msa_u8 *data_byte);
twi_error_t twi_send_a_get(twi_obj_create_t *twi_config_obj,msa_u8 slave_add);//tell x to send data
twi_error_t twi_get_op_status(twi_obj_create_t *twi_config_obj,msa_u8 *status_byte);
/*
*	send start
*	send slave add
*	send ack
*	send stop
*	send databyte
*	change receiver and send takes add of new and the byte
*	also have send arrays
*	send data and arrange all the frame funcs in it
*	broadcasting in i2c
*	SCLfreq=CPUfreq/(16+(2*TWBR)+(4^TWPS))
*/

/*	CAUTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *	The TWINT Flag must be cleared by software by writing a logic one to it. Note that this flag is not!!!!!!
 *	automatically cleared by hardware when executing the interrupt routine. Also note that clearing!!!!!!!!!!
 *	this flag starts the operation of the TWI, so all accesses to the TWI Address Register (TWAR),!!!!!!!!!!!
 *	TWI Status Register (TWSR), and TWI Data Register (TWDR) must be complete before clearing!!!!!!!!!!!!!!!!
 *	this flag.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/*
-*	When the TWINT Flag is set, the user must update all TWI Registers with the value relevant
-*	for the next TWI bus cycle. As an example, TWDR must be loaded with the value to be
-*	transmitted in the next bus cycle.
*/


#endif /* HAL_I2C_H_ */


#if 0
	    ****************************		***********************
		****************************		***************
		****		****		****		*********************
		****		****		****		***************************
		****		****		****		*************
		****************************		***********
		****  ****************  ****		**********************
		****		   			****		************
		****************************		**********************
		****************************		***********************
#endif