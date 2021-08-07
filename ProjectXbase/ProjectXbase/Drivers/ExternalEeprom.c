/*
 * ExternalEeprom.c
 *
 * Created: 6/18/2021 12:27:18 AM
 *  Author: ZeroX
 */ 
#include "ExternalEeprom.h"

//static TwiT Eeprom;

EepromRetT EepromInit(EepromT* Eeprom, TwiBaseT EepromBase, uint8_t EepromBitRate, uint8_t EepromAdd, TwiPrescallerT EepromPrescaller){
	if(NULL != Eeprom){
		
		Eeprom->TwiBase = EepromBase;
		Eeprom->TwiBitRate = EepromBitRate;
		Eeprom->TwiPrescaller = EepromPrescaller;
		Eeprom->TwiTargetedSlaveAdd = EepromAdd;
		Eeprom->TwiGenCallRecognition = TWI_GEN_CALL_RECOGNIZED;
		Eeprom->TwiCallBkFunc = NULL;
		Eeprom->TwiMySlaveAdd = 0x50;
		Eeprom->TwiTWAR_Offset = TWAR_0_OFFSET;
		Eeprom->TwiTWBR_Offset = TWBR_0_OFFSET;
		Eeprom->TwiTWCR_Offset = TWCR_0_OFFSET;
		Eeprom->TwiTWDR_Offset = TWDR_0_OFFSET,
		Eeprom->TwiTWSR_Offset = TWSR_0_OFFSET;
		
		TwiInit(Eeprom);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

//TODO: Returning the status
EepromRetT EepromWriteByte(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* Data ){
	if(NULL != Eeprom){
		EepromAckPolling(Eeprom);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom,Eeprom->TwiTargetedSlaveAdd,TWI_WRITE);
		uint16_t EepromAdd = (*MemAdd) >> 8; //get the high byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		EepromAdd = (*MemAdd) & 0xff; //get the low byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		TwiSendDataByte(Eeprom, Data);
		TwiSendStop(Eeprom);
		//EeprmAckPolling(Eeprom);	//again?
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

//TODO: retest if we dont send stop comm
EepromRetT EepromWriteArrayManual(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataArr, uint8_t ArrayLength){
	if(NULL != Eeprom){
		uint8_t iteration = 0;
		for(iteration = 0; 0 < ArrayLength; iteration++, ArrayLength--){
			EepromWriteByte(Eeprom, MemAdd, DataArr + iteration );
			(*MemAdd)++;
		}
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}


//TODO: retest if we dont send stop comm
EepromRetT EepromWriteArrayAuto(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataArr, uint8_t ArrayLength){
	if(NULL != Eeprom){
		uint8_t iteration = 0;
		EepromAckPolling(Eeprom);		
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom,Eeprom->TwiTargetedSlaveAdd,TWI_WRITE);
		//send the mem add
		uint16_t EepromAdd = (*MemAdd) >> 8; //get the high byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		EepromAdd = (*MemAdd) & 0xff; //get the low byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		//send the array
		for(iteration = 0; 0 < ArrayLength; iteration++, ArrayLength--){
			//EeprmWriteByte(Eeprom, MemAdd, Data + iteration );
			TwiSendDataByte(Eeprom, DataArr + iteration);
			//(*MemAdd)++;
		}
		TwiSendStop(Eeprom);
		//EeprmAckPolling(Eeprom);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}

EepromRetT EepromWritePage(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataPage){
	if(NULL != Eeprom){
		uint8_t iteration = 0;
		EepromAckPolling(Eeprom);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom,Eeprom->TwiTargetedSlaveAdd,TWI_WRITE);
		//send the mem add
		uint16_t EepromAdd = (*MemAdd) >> 8; //get the high byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		EepromAdd = (*MemAdd) & 0xff; //get the low byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		//send the array
		for(iteration = 0; iteration < EEPROM_PAGE_SIZE; iteration++){
			//EeprmWriteByte(Eeprom, MemAdd, Data + iteration );
			TwiSendDataByte(Eeprom, DataPage + iteration);
			//(*MemAdd)++;
		}
		TwiSendStop(Eeprom);
		//EeprmAckPolling(Eeprom);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}


EepromRetT EepromAckPolling(EepromT* Eeprom){
	if(NULL != Eeprom){
		TwiStatusT EepromStatus = 0;
		do{
			TwiSendStart(Eeprom);
			TwiSendSlaveAdd(Eeprom, Eeprom->TwiTargetedSlaveAdd, TWI_WRITE);
			TwiGetStatus(Eeprom, &EepromStatus);
			if (TWI_MT_SLAW_SENT_NACK_GOT != EepromStatus){
				break;
			}
			TwiSendStop(Eeprom);
			//TODO: not to block the system here, add a 1ms delay and a counter up to 20 or 50 else break with status not responding
		}while (TRUE);
		
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}


EepromRetT EepromReadCurrentAdd(EepromT* Eeprom,uint8_t* Data ){
	if(NULL != Eeprom){
		EepromAckPolling(Eeprom);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom, Eeprom->TwiTargetedSlaveAdd, TWI_READ);
		TwiGetDataByte(Eeprom, Data, TWI_NACK);
		TwiSendStop(Eeprom);
		//EeprmAckPolling(Eeprom);
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}


EepromRetT EepromReadRandom(EepromT* Eeprom, uint16_t* MemAdd,uint8_t* Data ){
	if(NULL != Eeprom){
		EepromAckPolling(Eeprom);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom, Eeprom->TwiTargetedSlaveAdd, TWI_WRITE);
		//send the mem add
		uint16_t EepromAdd = (*MemAdd) >> 8; //get the high byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		EepromAdd = (*MemAdd) & 0xff; //get the low byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom, Eeprom->TwiTargetedSlaveAdd, TWI_READ);
		TwiGetDataByte(Eeprom, Data, TWI_NACK);
		TwiSendStop(Eeprom);		
		//EeprmAckPolling(Eeprom)	
		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}


EepromRetT EepromReadSequential(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataArray, uint8_t Length ){
	if(NULL != Eeprom){
		uint8_t iteration = 0;
		EepromAckPolling(Eeprom);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom, Eeprom->TwiTargetedSlaveAdd, TWI_WRITE);
		//send the mem add
		uint16_t EepromAdd = (*MemAdd) >> 8; //get the high byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		EepromAdd = (*MemAdd) & 0xff; //get the low byte of mem add
		TwiSendDataByte(Eeprom, &EepromAdd);
		TwiSendStart(Eeprom);
		TwiSendSlaveAdd(Eeprom, Eeprom->TwiTargetedSlaveAdd, TWI_READ);
		while(Length > 1){
			TwiGetDataByte(Eeprom, DataArray + iteration, TWI_ACK);
			Length--;
			iteration++;
		}
		//the last byte and give a NACK then stop
		TwiGetDataByte(Eeprom, DataArray, TWI_NACK);
		TwiSendStop(Eeprom);
		
		//EeprmAckPolling(Eeprom);

		return TWI_ERR_NO;
	}
	return TWI_ERR_INV_PARAMS;
}