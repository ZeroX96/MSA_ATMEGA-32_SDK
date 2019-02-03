/*
 * ecu_CAN.h
 *
 * Created: 29/01/2019 12:47:23
 *  Author: Mahmoud
 */ 


#ifndef ECU_CAN_H_
#define ECU_CAN_H_
#include "includes.h"
#include "MCP2515_RAL.h"
#define CAN_ERRORS_BASE 0x20
#define SS_PIN 5
typedef enum
{
	NO_CAN_ERRORS		=CAN_ERRORS_BASE+0,
	INVALID_CAN_PARAMS	=CAN_ERRORS_BASE+1,
	
}can_errors_t;

typedef enum
{
	DEVICE_NOT_INITIATED	=0x00,
	DEVICE_INITIATED		=0x01,
}device_init_state_t;

typedef enum
{
	LOWEST_PRIORITY				=0x00,
	LOW_INTERMEDIATE_PRIORITY	=0x01,
	HIGH_INTERMEDIATE_PRIORITY	=0x02,
	HIGHEST_PRIORITY			=0x03,
}transmit_buffer_priority_t;

typedef enum
{
	DATA_FRAME		=0x00,
	REMOTE_FRAME	=0x01,
}frame_type_t;

typedef enum
{
	ZERO_BYTES		=0X00,
	ONE_BYTES		=0X01,
	TWO_BYTES		=0X02,
	THREE_BYTES		=0X03,
	FOUR_BYTES		=0X04,
	FIVE_BYTES		=0X05,
	SIX_BYTES		=0X06,
	SEVEN_BYTES		=0X07,
	EIGHT_BYTES		=0X08,
}data_length_code_t;
typedef enum
{
	RESET			=0b11000000,
	READ			=0b00000011,
	Read_RX_Buffer	=0b10010000,//will "or" this val with the val from the config_object or take it in the func. cal???
	WRITE			=0b00000010,
	Load_TX_Buffer	=0b01000000,//will "or" this val with the val from the config_object
	Request_To_Send	=0b10000000,//will "or" this val with the val from the config_object
	Read_Status		=0b10100000,
	RX_Status		=0b10110000,
	Bit_Modify		=0b00000101,//see if it's possible to use the reg will be called with or not from the reg map
}can_instructions_t;

typedef enum
{
	RXB0SIDH	=0x00,	//Receive Buffer 0,Start at RXB0SIDH	add=0x61
	RXB0D0		=0x01,	//Receive Buffer 0,Start at RXB0D0		add=0x66
	RXB1SIDH	=0x02,	//Receive Buffer 1,Start at RXB1SIDH	add=0x71
	RXB1D0		=0x03,	//Receive Buffer 1,Start at RXB1D0		add=0x76
}rx_buff_add_t;

typedef enum
{
	TXB0SIDH	=0x00,	//TX buffer 0, Start at	TXB0SIDH		add=0x31
	TXB0D0		=0x01,	//TX buffer 0, Start at TXB0D0			add=0x36
	TXB1SIDH	=0x02,	//TX buffer 1, Start at	TXB1SIDH		add=0x41
	TXB1D0		=0x03,	//TX buffer 1, Start at TXB1D0			add=0x46
	TXB2SIDH	=0x04,	//TX buffer 2, Start at	TXB2SIDH		add=0x51
	TXB2D0		=0x05,	//TX buffer 2, Start at TXB2D0			add=0x56
}tx_buff_add_t;

typedef enum
{
	RTS_TXB0			=0x01,
	RTS_TXB1			=0x02,
	RTS_TXB0_TXB1		=0x03,
	RTS_TXB2			=0x04,
	RTS_TXB0_TXB2		=0x05,
	RTS_TXB1_TXB2		=0x06,
	RTS_TXB0_TXB1_TXB2	=0x07,
}rts_buffer_select_t;

typedef struct
{
	device_init_state_t initialization_state;
	
}can_configs_t;

can_errors_t ecu_can_init(can_configs_t *cfg_obj); //init the spi driver,set the driver status and sets the cs pin high
can_errors_t ecu_can_reset(can_configs_t *cfg_obj);//performs the reset instruction
can_errors_t ecu_can_read(can_configs_t *cfg_obj,msa_u8 wanted_add,msa_u8 *receiving_buffer,msa_u8 data_size);//performs the read instruction
can_errors_t ecu_can_readRXbuffer(can_configs_t *cfg_obj,rx_buff_add_t rx_buff,msa_u8 *receiving_buffer,msa_u8 data_size);
can_errors_t ecu_can_write(can_configs_t *cfg_obj,msa_u8 targeted_add,msa_u8 *transmitted_buffer,msa_u8 data_size);
can_errors_t ecu_can_RequestToSend(can_configs_t *cfg_obj,rts_buffer_select_t buffer_select);
can_errors_t ecu_can_readStatus(can_configs_t *cfg_obj,msa_u8 *received_status_byte);
can_errors_t ecu_can_RXStatus(can_configs_t *cfg_obj,msa_u8 *received_status_byte);
can_errors_t ecu_can_BitModify(can_configs_t *cfg_obj,msa_u8 targeted_add,msa_u8 modification_mask,msa_u8 *transmitted_buffer,msa_u8 data_size);
// #ByMSA
#endif /* ECU_CAN_H_ */