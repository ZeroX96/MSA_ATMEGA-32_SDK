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

typedef enum
{
								//SJW: Synchronization Jump Width Length bits <1:0>
	SJW_1_TQ	=0X00,			//00= Length = 1 x TQ
	SJW_2_TQ	=0X01,			//01= Length = 2 x TQ
	SJW_3_TQ	=0X10,			//10= Length = 3 x TQ
	SJW_4_TQ	=0X11,			//11= Length = 4 x TQ	
}synch_jmp_width_t;

typedef enum
{
						//BTLMODE: PS2 Bit Time Length bit
	BTLMODE_0	=0x00,	//Length of PS2 is the greater of PS1 and IPT (2 TQ)
	BTLMODE_1	=0x01,	//Length of PS2 determined by PHSEG22:PHSEG20 bits of CNF3
}btl_mode_t;

typedef enum
{
							//SAM: Sample Point Configuration bit
	ONE_TIME	=0x00,		//Bus line is sampled once at the sample point
	THREE_TIMES	=0x01,		//Bus line is sampled three times at the sample point
}sampling_no_t;

typedef enum 
{
	LENGTH_1_TQ=0x00,
	LENGTH_2_TQ=0x01,
	LENGTH_3_TQ=0x02,
	LENGTH_4_TQ=0x03,
	LENGTH_5_TQ=0x04,
	LENGTH_6_TQ=0x05,
	LENGTH_7_TQ=0x06,
	LENGTH_8_TQ=0x07,
}segment_length_t;

typedef enum
{
	//WAKFIL: Wake-up Filter bit
	DISABLED	=0x00,	//0= Wake-up filter disabled
	ENABLED		=0x01,	//1= Wake-up filter enabled
}wake_up_filter_t;

typedef enum
{
								//SOF: Start-of-Frame signal bit
	CLOCKOUT_FUNCTION	=0x00,	//0= Wake-up filter disabled
	SOF_SIGNAL			=0x01,	//1= Wake-up filter enabled
}start_of_frame_t;
// 
// typedef struct  
// {
// }gs_cfg_regs_t;

typedef struct
{
	msa_u8				baud_rate_prescaler_obj;
	start_of_frame_t	start_of_frame_obj;
	synch_jmp_width_t	synch_jmp_width_obj;
	btl_mode_t		    btl_mode_obj;
	sampling_no_t		sampling_number_obj;
	segment_length_t	phase1_seg_length_obj;
	segment_length_t	phase2_seg_length_obj;
	segment_length_t	propagation_seg_length_obj;
	wake_up_filter_t	wake_up_filter_obj;
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

#if 0
//////////////////////////////////////

//*****************************************************************************
//
// API Function prototypes
//
//***************************************************************************** from TI as a ref.
extern void CANBitTimingGet(uint32_t ui32Base, tCANBitClkParms *psClkParms);
extern void CANBitTimingSet(uint32_t ui32Base, tCANBitClkParms *psClkParms);
extern uint32_t CANBitRateSet(uint32_t ui32Base, uint32_t ui32SourceClock,
                              uint32_t ui32BitRate);
extern void CANDisable(uint32_t ui32Base);
extern void CANEnable(uint32_t ui32Base);
extern bool CANErrCntrGet(uint32_t ui32Base, uint32_t *pui32RxCount,
                          uint32_t *pui32TxCount);
extern void CANInit(uint32_t ui32Base);
extern void CANIntClear(uint32_t ui32Base, uint32_t ui32IntClr);
extern void CANIntDisable(uint32_t ui32Base, uint32_t ui32IntFlags);
extern void CANIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags);
extern void CANIntRegister(uint32_t ui32Base, void (*pfnHandler)(void));
extern uint32_t CANIntStatus(uint32_t ui32Base, tCANIntStsReg eIntStsReg);
extern void CANIntUnregister(uint32_t ui32Base);
extern void CANMessageClear(uint32_t ui32Base, uint32_t ui32ObjID);
extern void CANMessageGet(uint32_t ui32Base, uint32_t ui32ObjID,
                          tCANMsgObject *psMsgObject, bool bClrPendingInt);
extern void CANMessageSet(uint32_t ui32Base, uint32_t ui32ObjID,
                          tCANMsgObject *psMsgObject, tMsgObjType eMsgType);
extern bool CANRetryGet(uint32_t ui32Base);
extern void CANRetrySet(uint32_t ui32Base, bool bAutoRetry);
extern uint32_t CANStatusGet(uint32_t ui32Base, tCANStsReg eStatusReg);

//////////////////////////////////////
#endif
#endif /* ECU_CAN_H_ */