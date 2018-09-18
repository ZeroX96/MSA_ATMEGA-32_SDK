/*
 * hal_USART.h
 *
 * Created: 11/09/2018 20:59:45
 *  Author: Mahmoud
 */ 


#ifndef HAL_USART_H_
#define HAL_USART_H_

#include "common.h"
#include "MSA_AVR-REGS_SUMMARY.h"
#include "hal_USART_CFG.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define MAX_IN_ARR_SZE 255	//the max array size can be received 


#define USART_ERRORS_BASE 20

typedef enum  //usart_errors
{
	NO_USART_ERRORS=(USART_ERRORS_BASE+1),
	INVALID_PARAMS=(USART_ERRORS_BASE+2),
	DEVICE_NOT_INITIATED=(USART_ERRORS_BASE+3),
	DEVICE_ALREADY_DEINITIATED=(USART_ERRORS_BASE+6),
}usart_ret_types;

typedef enum
{
	FIVE=0x00,
	SIX=0x01,
	SEVEN=0x02,
	EIGHT=0x03,
	NINE=0x07,
}data_bits_no_t;

typedef enum
{
	NO_PARITY=0x00,
	EVEN_PARITY=0x02,
	ODD_PARITY=0x03,
}parity_mode_t;

typedef enum 
{
	ONE_STP_BIT=0x00,
	TWO_STP_BITS=0x01,
}stop_bits_no_t;

typedef enum
{
	A_SENDER=0x01,
	A_RECEIVER=0x02,
	B_SENDER_N_RECEIVER=0x03,
}operations_mode_t;

typedef enum
{
	INTERRUPT_DRIVEN,
	POLLING_DRIVEN,
}notfications_mode_t;

typedef enum
{
	NOT_INITIATED,
	INITIATED,
}device_state_t;

typedef enum
{
	Rising_EDGE,
	FALLING_EDGE,
}clock_polarity_t;
typedef struct  
{
	stop_bits_no_t		obj_stop_bits_no;
	operations_mode_t	obj_operations_mode;
	parity_mode_t		obj_parity_mode;
	data_bits_no_t      obj_data_bits_no;
	notfications_mode_t obj_notifications_mode;
	device_state_t		obj_device_state;
	void				(*obj_vptr_RXC_cb)(void);
	void				(*obj_vptr_TXC_cb)(void);
	clock_polarity_t	clock_polarity;
}usart_t;



usart_ret_types usart_init(	usart_t*			usart_obj,
							operations_mode_t	operations_mode,
							stop_bits_no_t		stop_bits_no,
							parity_mode_t		parity_mode,
							data_bits_no_t		data_bits_no,
							notfications_mode_t notifications_mode,
							clock_polarity_t	clock_polarity
						  );

usart_ret_types usart_send_byte(usart_t * usart_obj,msa_u8* byte);

usart_ret_types usart_send_arr(usart_t* usart_obj,uint8_t *arr_add);

usart_ret_types usart_receive_byte(usart_t * usart_obj,msa_u8* byte);

usart_ret_types usart_receive_arr(usart_t * usart_obj,msa_u8* arr_add,msa_u8 arr_size);

usart_ret_types usart_set_isr_RXC_callback(usart_t * usart_obj,void (*vptr_cb)(void));

usart_ret_types usart_set_isr_TXC_callback(usart_t * usart_obj,void (*vptr_cb)(void));
#endif /* HAL_USART_H_ */