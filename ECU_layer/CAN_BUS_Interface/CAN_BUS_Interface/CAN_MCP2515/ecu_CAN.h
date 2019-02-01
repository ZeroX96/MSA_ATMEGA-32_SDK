/*
 * ecu_CAN.h
 *
 * Created: 29/01/2019 12:47:23
 *  Author: Mahmoud
 */ 


#ifndef ECU_CAN_H_
#define ECU_CAN_H_


typedef enum
{
	RESET,
	READ,
	Read_RX_Buffer,
	WRITE,
	Load_TX_Buffer,
	Request_To_Send,
	Read_Status,
	RX_Status,
	Bit_Modify,
}can_instruction;



#endif /* ECU_CAN_H_ */