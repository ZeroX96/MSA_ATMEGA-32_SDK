/*
 * Usart.h
 *
 *  Created on: Dec 24, 2020
 *      Author: Mahmoud Saad
 */

#ifndef USART_H_
#define USART_H_
#include "HwAccess.h"
//change these settings as needed.if not changed,these are Z{//default settings-------|
//																					  |
//the operating mode																  |
#define UART  //options for this driver {UART or USRT}								  |
//																					  |
//the operating frequency															  |
#ifndef F_CPU
#define F_CPU 8000000UL														//		  |
#endif //F_CPU
//																					  |
//the required baudRate																  |
#define BAUD_VAL 9600				//												  |
//																					  |
#ifdef UART				//															  |
#define NORMAL_ASYNCH //options for uart {normal_speed or double_speed}				  |
//																					  |
//																					  |
//																					  |
//																					  |
#endif //UART																		  |
//																					  |
//																					  |
#ifdef  USRT																		//|
#define  MASTER_SYNCH //options for usrt {master sychronous or slave synchronous}	  |
//																					  |
//																					  |
#endif //USRT																		  |
//end of the default settings								}//default settings-------|

//USART_REGS
/*
	#define MSA_UBRRH	(*(volatile uint8_t)0x40)
	#define MSA_UCSRC	(*(volatile uint8_t)0x40)
	#define MSA_UDR		(*(volatile uint8_t)0x2c)
	#define MSA_UCSRA	(*(volatile uint8_t)0x2b)
	#define MSA_UCSRB	(*(volatile uint8_t)0x2a)
	#define MSA_UBRRL	(*(volatile uint8_t)0x29)
*/
////non expandable, solved
//#define UBRRL_0_OFFSET (0U)	//the base
//#define UCSRB_0_OFFSET (1U)
//#define UCSRA_0_OFFSET (2U)
//#define UDR_0_OFFSET   (3U)
//#define UCSRC_0_OFFSET (23U) 
//
//typedef enum{
	//USART_0_BASE = 0x29, 	//add of TWBR
	////for future use with devices having more than one twi module
//}TwiBaseT;
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef NULL
#define NULL ((void*)0)
#endif //NULL

#define MAX_IN_ARR_SZE 250	//the max array size can be received


#define USART_ERR_BASE 20

typedef enum  //usart_errors
{
	USART_ERR_NO						 = (USART_ERR_BASE+1),
	USART_ERR_INV_PARAMS				 = (USART_ERR_BASE+2),
	USART_ERR_NOT_INIT					 = (USART_ERR_BASE+3),
	USART_ERR_DEVICE_ALREADY_DEINITIATED = (USART_ERR_BASE+6),
}UsartRetT;

typedef enum
{
	FIVE_DATA_BITS  = 0x00,
	SIX_DATA_BITS   = 0x01,
	SEVEN_DATA_BITS = 0x02,
	EIGHT_DATA_BITS=0x03,
	NINE_DATA_BITS =0x07,
}DataBitsNoT;

typedef enum
{
	NO_PARITY=0x00,
	EVEN_PARITY=0x02,
	ODD_PARITY=0x03,
}ParityModeT;

typedef enum
{
	ONE_STP_BIT=0x00,
	TWO_STP_BITS=0x01,
}StopBitsNoT;

typedef enum
{
	A_SENDER=0x01,
	A_RECEIVER=0x02,
	B_SENDER_N_RECEIVER=0x03,
}OperationModeT;

typedef enum
{
	INTERRUPT_DRIVEN,
	POLLING_DRIVEN,
}NotificsModeT;

typedef enum
{
	NOT_INITIATED,
	INITIATED,
}DeviceStateT;

typedef enum
{
	Rising_EDGE,
	FALLING_EDGE,
}ClkPolarityT;

typedef struct
{
	StopBitsNoT		ObjStopBitsNo;
	OperationModeT	objOpMode;
	ParityModeT		objParityMode;
	DataBitsNoT     ObjDataBitsNo;
	NotificsModeT   ObjNotificsMode;
	DeviceStateT	ObjDeviceState;
	ClkPolarityT	ClkPolarity;
}UsartT;



UsartRetT UsartInit(UsartT*			UsartObj, OperationModeT	OpMode,
					StopBitsNoT		StopBitsNo, ParityModeT		ParityMode,
					DataBitsNoT		DataBitsNo, NotificsModeT   NotificsMode,
					ClkPolarityT	ClkPolarity
					);

UsartRetT UsartSendByte(UsartT * UsartObj,uint8_t* byte);

UsartRetT UsartSendArr(UsartT* UsartObj,uint8_t *ArrAdd);

UsartRetT UsartReceiveByte(UsartT * UsartObj,uint8_t* byte);

UsartRetT UsartReceiveArr(UsartT * UsartObj,uint8_t* ArrAdd,uint8_t ArrSize);

UsartRetT UsartSetIsrRxcCb(UsartT * UsartObj,void (*IsrCb)(void));

UsartRetT UsartSetIsrTxcCb(UsartT * UsartObj,void (*IsrCb)(void));
#endif /* USART_H_ */
