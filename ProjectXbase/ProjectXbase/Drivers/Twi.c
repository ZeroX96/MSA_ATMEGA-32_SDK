/*
 * Twi.c
 *
 *  Created on: Dec 6, 2020
 *      Author: plan2
 */
#include "Twi.h"


static TwiRetT TwiWaitCurrentJob(TwiT* Twi);

TwiRetT TwiInit(TwiT* Twi){
	if(NULL != Twi){
		//store the TWBR Register Val
		HwWriteReg(Twi->TwiBase + Twi->TwiTWBR_Offset,Twi->TwiBitRate);
		//store the prescaler val in TWSR
		HwWriteRegBitGroup(Twi->TwiBase + Twi->TwiTWSR_Offset, (Twi->TwiPrescaller & 0x03), 2, 0);
		//TWSR = (Twi->TwiPrescaller & 0x03);
		//store the MySlaveAddress in the TWAR
		//HwWriteRegBitGroup(Twi->TwiBase + Twi->TwiTWAR_Offset, (Twi->TwiMySlaveAdd & ~(0x01)), 7, 1);
		HwWriteReg(Twi->TwiBase + Twi->TwiTWAR_Offset, (Twi->TwiMySlaveAdd & ~(0x01)) );
		//TWAR = Twi->TwiMySlaveAdd & ~(0x01);
		//config the General Recognition Bit
		if(TWI_GEN_CALL_RECOGNIZED == Twi->TwiGenCallRecognition){
			HwSetRegBit(Twi->TwiBase + Twi->TwiTWAR_Offset,TWGCE);
		}
//config the TWCR
		//config the Interrupt Mode
#if(TWI_INTERRUPT_DRIVEN == TRUE)

		HwSetRegBit(Twi->TwiBase + Twi->TwiTWCR_Offset,TWIE);
#endif //TWI_INTERRUPT_DRIVEN
		//Enable the module
		HwSetRegBit(Twi->TwiBase + Twi->TwiTWCR_Offset,TWEN);
#if(TWI_ACK_INITIALLY_ENABLED == TRUE)
		//By Default the Enable Ack is Enabled and configured later as needed
		HwSetRegBit(Twi->TwiBase + Twi->TwiTWCR_Offset,TWEA);
#endif
		Twi->TwiInitialized = TWI_INITIATED; //TODO: INITIALIZED = TRUE OR FALSE
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiEdit(TwiT* Twi){
	if(NULL != Twi){
		TwiInit(Twi);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSendStart(TwiT* Twi){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		//TWCR = 0;//(uint8_t)((1<<TWINT) | (1<<TWEN) | (1<<TWSTA));
		HwWriteReg(Twi->TwiBase + Twi->TwiTWCR_Offset, (uint8_t)((1<<TWINT) | (1<<TWEN) | (1<<TWSTA)) );
		//TODO: Wait the TWINT Flag to be Set
		TwiWaitCurrentJob(Twi);
		//TODO:Read the status and store it in the struct
		Twi->TwiStatus = HwReadRegBitGroup(Twi->TwiBase + Twi->TwiTWSR_Offset, 5, TWS3);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSendRepeatedStart(TwiT* Twi){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		return TwiSendStart(Twi);
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSendStop(TwiT* Twi){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		HwWriteReg(Twi->TwiBase + Twi->TwiTWCR_Offset, (uint8_t)((1<<TWINT) | (1<<TWEN) | (1<<TWSTO)) );
		//TODO: Wait the TWINT Flag to be Set
		TwiWaitCurrentJob(Twi);
		//TODO:Read the status and store it in the struct
		Twi->TwiStatus = HwReadRegBitGroup(Twi->TwiBase + Twi->TwiTWSR_Offset, 5, TWS3);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSetMySlaveAdd(TwiT* Twi, uint8_t SlaveAdd){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		Twi->TwiMySlaveAdd = SlaveAdd;
		HwWriteReg(Twi->TwiBase + Twi->TwiTWAR_Offset, (SlaveAdd & ~(1<<0) ) | Twi->TwiGenCallRecognition);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSendSlaveAdd(TwiT* Twi, uint8_t SlaveAdd, TwiOpModeT TwiOpMode){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		Twi->TwiTargetedSlaveAdd = SlaveAdd;
		//TODO: Put the Needed Val To be Transmitted + the MODE >> READ OR WRITE
		HwWriteReg(Twi->TwiBase + Twi->TwiTWDR_Offset, (SlaveAdd & ~(1<<0) ) | TwiOpMode);
		//TODO: Initiate the transmission
		HwWriteReg(Twi->TwiBase + Twi->TwiTWCR_Offset, (uint8_t)((1<<TWINT) | (1<<TWEN) ) );
		//TODO: Wait the TWINT Flag to be Set 
		TwiWaitCurrentJob(Twi);
		//TODO:Read the status and store it in the struct
		Twi->TwiStatus = HwReadRegBitGroup(Twi->TwiBase + Twi->TwiTWSR_Offset, 5, TWS3);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSendDataByte(TwiT* Twi, uint8_t* Data){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		//TODO: Put the Needed Val To be Transmitted
		HwWriteReg(Twi->TwiBase + Twi->TwiTWDR_Offset, *Data);
		//PORTA++;
		//TWDR = *Data;
		//TODO: Initiate the transmission
		HwWriteReg(Twi->TwiBase + Twi->TwiTWCR_Offset, (uint8_t)((1<<TWINT) | (1<<TWEN) ) );
		//TODO: Wait the TWINT Flag to be Set
		TwiWaitCurrentJob(Twi);
		//TODO:Read the status and store it in the struct
		Twi->TwiStatus = HwReadRegBitGroup(Twi->TwiBase + Twi->TwiTWSR_Offset, 5, TWS3);

		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiSendDataArr(TwiT* Twi, uint8_t* DataArr, uint8_t DataLen){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		uint8_t i = 0;
		while(DataLen > 0){
			TwiSendDataByte(Twi, DataArr+i );
			i++;
			DataLen--;
		}
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}
//if this is the last/ only byte to receive, set ack to nack
TwiRetT TwiGetDataByte(TwiT* Twi, uint8_t* Data, TwiAckT TwiAck){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		//TODO: Initiate the Exchange
		HwWriteReg(Twi->TwiBase + Twi->TwiTWCR_Offset, (uint8_t)((1<<TWINT) | (1<<TWEN) | (TwiAck<<TWEA) ) );
		//TODO: Wait the TWINT Flag to be Set
		TwiWaitCurrentJob(Twi);
		//PORTD = 0xdf;
		//TODO: Get the data from the register
		*Data = HwReadReg(Twi->TwiBase + Twi->TwiTWDR_Offset);
		//PORTA++;
		//*Data = TWDR;
		//TODO:Read the status and store it in the struct
		Twi->TwiStatus = HwReadRegBitGroup(Twi->TwiBase + Twi->TwiTWSR_Offset, 5, TWS3);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

TwiRetT TwiGetDataArr(TwiT* Twi,uint8_t* DataArr, uint8_t DataLen){
	if(NULL != Twi){
		if(Twi->TwiInitialized != TWI_INITIATED){
			return TWI_ERR_DEVICE_NOT_INIT;
		}
		uint8_t i = 0;
		while(DataLen > 0){
			TwiGetDataByte(Twi, DataArr+i, (DataLen > 1) );
			i++;
			DataLen--;
		}
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}


static TwiRetT TwiWaitCurrentJob(TwiT* Twi){
	while( HwIsBitClr(Twi->TwiBase + Twi->TwiTWCR_Offset, TWINT) ){
		//wait for the TWINT flag to become high
	}
	return TWI_ERR_NO;
}

TwiRetT TwiSlaveWait2bCalled(TwiT* Twi){
	while( HwIsBitClr(Twi->TwiBase + Twi->TwiTWCR_Offset, TWINT) ){
		//wait for the TWINT flag to become high
		PORTA++;
		_delay_ms(250);
	}
	return TWI_ERR_NO;
}

TwiRetT TwiGetStatus(TwiT* Twi, TwiStatusT* Status){
	if((NULL != Twi) && (NULL != Status)){
		*Status = Twi->TwiStatus;
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}