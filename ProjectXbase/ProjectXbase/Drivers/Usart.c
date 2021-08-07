/*
 * Usart.c
 *
 *  Created on: Dec 24, 2020
 *      Author: Mahmoud Saad
 */

/*
 * originally was hal_USART.c and adopted to the new style and named Usart.c
 *
 * Created: 11/09/2018 20:59:27
 *  Author: Mahmoud
 */
#include "Usart.h"

static void (*TxcCb)(void);
static void (*RxcCb)(void);

//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************
//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************
//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************
//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************

#ifdef UART
///*(volatile uint8_t *)0x40 &= ~(1<<UMSEL);	//activating asynchronous mode
#ifdef NORMAL_ASYNCH
uint32_t UBRR_VAL =( ((uint32_t)F_CPU/((uint32_t)16*BAUD_VAL)) -1) ; //normal speed equ  >>The URSEL must be zero when writing the UBRRH
#else  //DOUBLE_SPEED
uint32_t UBRR_VAL =( ((uint32_t)F_CPU/((uint32_t)8*BAUD_VAL)) -1) ;//double speed equ  >>The URSEL must be zero when writing the UBRRH
(*(volatile uint8_t*)0x2b) |=(1<<U2X);	//enabling the double speed mode
#endif //norm or double speed asynch

#endif  //end of uart

#ifdef USRT
(*(volatile uint8_t*)0x40) |=(1<<URSEL)|(1<<UMSEL);	//activating synchronous mode
#ifdef MASTER_SYNCHRONOUS
uint32_t UBRR_VAL =( ((uint32_t)F_CPU/((uint32_t)2*BAUD_VAL)) -1) ; //synchronous master equ >>The URSEL must be zero when writing the UBRRH
#endif //end of master_synch


#endif //end of usrt



UsartRetT UsartInit(UsartT*      UsartObj,OperationModeT  OpMode,
					StopBitsNoT	 StopBitsNo,ParityModeT	  ParityMode,
					DataBitsNoT	 DataBitsNo,NotificsModeT NotificsMode,
					ClkPolarityT ClkPolarity
				   ){
	UsartRetT RetVal=USART_ERR_NO;
if (		 (UsartObj != NULL)														&&
			((OpMode>=A_SENDER) && (OpMode<=B_SENDER_N_RECEIVER))		&&
			((StopBitsNo==ONE_STP_BIT) || (StopBitsNo==TWO_STP_BITS))				&&
			((ParityMode >= NO_PARITY) && (ParityMode <=ODD_PARITY))					&&
			((DataBitsNo >=FIVE_DATA_BITS) && (DataBitsNo <=NINE_DATA_BITS))							&&
			((NotificsMode== INTERRUPT_DRIVEN) || (NotificsMode == POLLING_DRIVEN))
	)
	{
		if (NotificsMode == INTERRUPT_DRIVEN){
			cli();
			if (OpMode == A_SENDER){
				(*(volatile uint8_t*)0x2a) |=(1<<UDRIE);
			}
			else if (OpMode == A_RECEIVER){
				(*(volatile uint8_t*)0x2a) |=(1<<RXCIE);
			}
			else{
				(*(volatile uint8_t*)0x2a) |=(1<<RXCIE)|(1<<UDRIE);
			}
		}
		//storing config data
		UsartObj->ObjDataBitsNo=DataBitsNo;
		UsartObj->ObjDeviceState=INITIATED;
		UsartObj->ObjNotificsMode=NotificsMode;
		UsartObj->objOpMode=OpMode;
		UsartObj->objParityMode=ParityMode;
		UsartObj->ObjStopBitsNo=StopBitsNo;

		(*(volatile uint8_t*)0x2a) |=(OpMode << TXEN)|( (DataBitsNo & 0x04)<<UCSZ2);
		UCSRC=(1<<URSEL)|(StopBitsNo<<USBS)|((DataBitsNo & 0x03)<<UCSZ0 )|(ParityMode << UPM0)|(ClkPolarity<<UCPOL);
		(*(volatile uint8_t*)0x29)  =(uint8_t) UBRR_VAL;
		(*(volatile uint8_t*)0x40)  =(uint8_t) ((UBRR_VAL>>8)&(0x7f));
	}
	else{
		RetVal =USART_ERR_INV_PARAMS;
	}

	if (NotificsMode == INTERRUPT_DRIVEN){
		sei();	//if interrupt driven the global interrupt must be disabled and then enabled at the end of the initiation
	}

	return RetVal;
}
UsartRetT UsartReceiveByte(UsartT * UsartObj,uint8_t* byte){
	UsartRetT RetVal=USART_ERR_NO;
	if ( (UsartObj != NULL) && (byte != NULL) ){
		if (UsartObj->ObjDeviceState == INITIATED){

			while(!((*(volatile uint8_t*)0x2b) & (1<<RXC)))
			;
			*byte=(*(volatile uint8_t*)0x2c);
			(*(volatile uint8_t*)0x2b)|=(1<<RXC);
		}
		else{
			RetVal=NOT_INITIATED;
		}
	}
	else{
	RetVal=USART_ERR_INV_PARAMS;
	}
	return RetVal;
}

UsartRetT UsartSendByte(UsartT * UsartObj,uint8_t* byte){
	UsartRetT RetVal=USART_ERR_NO;
	if ( (UsartObj != NULL) && (byte != NULL) ){
		if (UsartObj->ObjDeviceState == INITIATED){

			while ( !( (*(volatile uint8_t*)0x2b) & (1<<UDRE) ) )
			;
				(*(volatile uint8_t*)0x2c)=*byte;


		}
		else{
			RetVal=NOT_INITIATED;
		}
	}
	else{
		RetVal=USART_ERR_INV_PARAMS;
	}
	return RetVal;

}

UsartRetT UsartSendArr(UsartT* UsartObj,uint8_t *ArrAdd){
	UsartRetT RetVal=USART_ERR_NO;
	if ( (UsartObj != NULL) && (ArrAdd != NULL) ){
		if (UsartObj->ObjDeviceState == INITIATED){
			uint8_t i=0;
			for (i=0;*(ArrAdd+i) ;i++){
				while ( !( (*(volatile uint8_t*)0x2b) & (1<<UDRE) ) )
				;
				(*(volatile uint8_t*)0x2c)=*(ArrAdd+i);
			}
		}
		else{
			RetVal=NOT_INITIATED;
		}
	}
	else{
		RetVal=USART_ERR_INV_PARAMS;
	}
	return RetVal;
}

UsartRetT UsartReceiveArr(UsartT * UsartObj,uint8_t* ArrAdd,uint8_t ArrSize){
	UsartRetT RetVal=USART_ERR_NO;
	uint8_t DataCount=0,temp=0;

	if ( (UsartObj != NULL) && (ArrAdd != NULL) ){
		if (UsartObj->ObjDeviceState == INITIATED){
			while( (temp != 13) && (DataCount < (ArrSize) ) ){
				UsartReceiveByte(UsartObj,&temp);
				ArrAdd[DataCount++]=temp;
				if(temp == '\0')
				break;
			}
			ArrAdd[DataCount]=0;
		}
		else{
			RetVal=NOT_INITIATED;
		}
	}
	else{
		RetVal=USART_ERR_INV_PARAMS;
	}
	return RetVal;
}


UsartRetT UsartSetIsrRxcCb(UsartT * UsartObj,void (*IsrCb)(void)){
	UsartRetT RetVal=USART_ERR_NO;
	if ( (UsartObj != NULL) && (IsrCb != NULL) ){
		if (UsartObj->ObjDeviceState == INITIATED){
			RxcCb=IsrCb;
		}
		else{
			RetVal=NOT_INITIATED;
		}
	}
	else{
		RetVal=USART_ERR_INV_PARAMS;
	}
	PORTA=0xff;
	return RetVal;
}


UsartRetT UsartSetIsrTxcCb(UsartT * UsartObj,void (*IsrCb)(void)){
	UsartRetT RetVal=USART_ERR_NO;
	if ( (UsartObj != NULL) && (IsrCb != NULL) ){
		if (UsartObj->ObjDeviceState == INITIATED){
			TxcCb=IsrCb;

		}
		else{
			RetVal=NOT_INITIATED;
		}
	}
	else{
		RetVal=USART_ERR_INV_PARAMS;
	}
	PORTA=0xff;
	return RetVal;
}

ISR(USART_RXC_vect){
	if (RxcCb){
		RxcCb();
	}

}

ISR(USART_UDRE_vect){
	if (TxcCb){
		TxcCb();
	}
}


