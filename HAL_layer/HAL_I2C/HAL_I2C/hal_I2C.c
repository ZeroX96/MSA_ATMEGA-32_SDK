/*
 * hal_I2C.c
 *
 * Created: 05/11/2018 03:34:51
 *  Author: Mahmoud
 */ 
#include "hal_I2C.h"
twi_error_t twi_init_module(twi_obj_create_t *twi_config_obj,twi_module_base_t module_base,twi_ack_mode_t ack_mode,twi_notifics_mode_t notifics_mode)
{
	twi_error_t ret_val=NO_I2C_ERRORS;
	if ()
	{
		//storing info
		twi_config_obj->ack_mode_obj=ack_mode;
		twi_config_obj->module_base_obj=module_base;
		twi_config_obj->notifics_mode_obj=notifics_mode;
		twi_config_obj->module_state_obj=I2C_INITIATED;
		
	} 
	else
	{
		ret_val=INVALID_I2C_PARAMS;
	}
	return ret_val;
}
twi_error_t twi_set_slave_add(twi_obj_create_t *twi_config_obj,msa_u8 slave_add);
twi_error_t twi_send_a_start(twi_obj_create_t *twi_config_obj);
twi_error_t twi_send_a_stop(twi_obj_create_t *twi_config_obj);
twi_error_t twi_send_byte(twi_obj_create_t *twi_config_obj,msa_u8 *data_byte);
twi_error_t twi_send_packet(twi_obj_create_t *twi_config_obj,msa_u8 *data_add,msa_u8 data_size);
twi_error_t twi_receive_byte(twi_obj_create_t *twi_config_obj,msa_u8 *data_byte);
twi_error_t twi_send_a_get(twi_obj_create_t *twi_config_obj,msa_u8 slave_add);//tell x to send data
twi_error_t twi_get_op_status(twi_obj_create_t *twi_config_obj,msa_u8 *status_byte);