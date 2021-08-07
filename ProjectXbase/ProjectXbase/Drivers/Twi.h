/*
 * Twi.h
 *
 *  Created on: Dec 6, 2020
 *      Author: plan2
 */
#ifndef TWI_H_
#define TWI_H_
//TODO: Return Val Handler
#include <avr/io.h>
#include "HwAccess.h"

#define TWI_ERR_BASE 20

//define the mode of transmission> interrupt or polling based
#define TWI_INTERRUPT_DRIVEN FALSE
#define TWI_POLLING_DRIVEN	 TRUE

//define the Enable Ack initial state
#define TWI_ACK_INITIALLY_ENABLED  TRUE
#define TWI_ACK_INITIALLY_DISABLED FALSE

typedef enum
{
	TWI_ERR_NO 				= (TWI_ERR_BASE+1),
	TWI_ERR_INV_PARAMS      = (TWI_ERR_BASE+2),
	TWI_ERR_DEVICE_NOT_INIT = (TWI_ERR_BASE+3),
	TWI_ERR_WRITE_COLLISION = (TWI_ERR_BASE+4),
}TwiRetT;
/*
typedef enum
{
	TWI_INTERRUPT_DRIVEN,
	TWI_POLLING_DRIVEN,
}TwiModeT;*/



typedef enum
{
	TWI_NOT_INITIATED,
	TWI_INITIATED,
}TwiStateT;

//non expandable, solved
#define TWBR_0_OFFSET  (0U)
#define TWSR_0_OFFSET  (1U)
#define TWAR_0_OFFSET  (2U)
#define TWDR_0_OFFSET  (3U)
#define TWCR_0_OFFSET  (54U) //TODO:Really? Solved :D

typedef enum{
	TWI_0_BASE = 0x20, 	//add of TWBR
	//for future use with devices having more than one twi module
}TwiBaseT;
//TODO: Revise the values
typedef enum{ 
	//*************************MASTER TRANSMITTER MODE**************************************************//
		TWI_MT_ST_SENT 		   	    = 0x08,	//A START condition has been transmitted
		TWI_MT_RP_ST_SENT 		   	= 0x10, //A repeated START condition has been transmitted
		TWI_MT_SLAW_SENT_ACK_GOT  	= 0x18,	//SLA+W has been transmitted; ACK has been received
		TWI_MT_SLAW_SENT_NACK_GOT 	= 0x20,	//SLA+W has been transmitted; NOT-ACK has been received
		TWI_MT_DATA_SENT_ACK_GOT  	= 0x28,	//Data byte has been transmitted; ACK has been received
		TWI_MT_DATA_SENT_NACK_GOT 	= 0x30,	//Data byte has been transmitted; NOT-ACK has been received
		TWI_MT_ARBITRATION_LOST   	= 0x38,	//SLA+W has been transmitted; NOT-ACK has been received
	//**************************************************************************************************//
	//*************************MASTER RECEIVER MODE*****************************************************//
		TWI_MR_ST_SENT 		   	    = 0x08,	//A START condition has been transmitted
		TWI_MR_RP_ST_SENT 		   	= 0x10, //A repeated START condition has been transmitted
		TWI_MR_ARB_LOST_OR_NACK  	= 0x38,	//Arbitration lost in SLA+R or NOT ACK bit
		TWI_MR_SLAR_SENT_ACK_GOT 	= 0x40,	//SLA+R has been transmitted; ACK has been received
		TWI_MR_SLAR_SENT_NACK_GOT 	= 0x48,	//SLA+R has been transmitted; NOT ACK has been received
		TWI_MR_DATA_GOT_ACK_SENT  	= 0x50,	//Data byte has been received; ACK has been returned
		TWI_MR_DATA_GOT_NACK_SENT 	= 0x58,	//Data byte has been received; NOT ACK has been returned
	//**************************************************************************************************//
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//**************************SLAVE TRANSMITTER MODE*****************************************************//
		TWI_ST_SLAR_GOT_ACK_SENT    		= 0xA8,	//Own SLA+R has been received; ACK has been returned
		TWI_ST_ARB_LOST_SLAR_GOT_ACK_SENT 	= 0xB0, //Arbitration lost in SLA+R as master; own SLA+R
													//has been received; ACK has been returned
		TWI_ST_DATA_SENT_ACK_GOT  			= 0xB8,	//Data byte has been transmitted; ACK has been received
		TWI_ST_DATA_SENT_NACK_GOT  			= 0xC0,	//Data byte has been transmitted; NOT ACK has been received
		TWI_ST_DATA_SENT_ACK_RECEIVED	  	= 0xC8,	//Last data byte in TWDR has been transmitted
													//(TWEA = “0”); ACK has been received
	//**************************************************************************************************//
	//**************************SLAVE RECEIVER MODE*****************************************************//
		TWI_SR_SLAW_GOT_ACK_SENT    		= 0x60,	//Own SLA+W has been received; ACK has been returned
		TWI_SR_ARB_LOST_SLAW_GOT_ACK_SENT 	= 0x68, //Arbitration lost in SLA+R/W as master; own SLA+W
													//has been received; ACK has been returned
		TWI_SR_G_CALL_GOT_ACK_SENT	  		= 0x70,	//General call address has been received;
													//ACK has been returned
		TWI_SR_ARB_LOST_G_CALL_GOT_ACK_SENT = 0x78, //Arbitration lost in SLA+R/W as master;
													//General call address has 	been received;
													//ACK has been returned
		TWI_SR_DATA_GOT_ACK_SENT  			= 0x80,	//Previously addressed with own	SLA+W; data has
													//been received; ACK has been returned
		TWI_SR_DATA_GOT_NACK_SENT 			= 0x88,	//Previously addressed with own	SLA+W; data has
													//been received; NOT ACK has been returned
		TWI_SR_PR_G_CALL_GOT_ACK_SENT		= 0x90,	//Previously addressed with general call;
													//data has been received; ACK has been returned
		TWI_SR_G_CALL_GOT_NACK_SENT		   	= 0x98,	//Previously addressed with general call;
													//data has been received; NOT ACK has been returned
		TWI_SR_STOP_OR_RP_ST_GOT_STILL_SLAVE= 0xA0,	//A STOP condition or repeated START condition has
													//been received while still addressed as slave
	//*****************************************************************************************************//
}TwiStatusT;

/*
typedef enum
{
	TWI_INTERRUPT_DRIVEN,
	TWI_POLLING_DRIVEN,
}TwiIntModeT;*/

typedef enum
{
	TWI_GEN_CALL_NOT_RECOGNIZED = 0x00,
	TWI_GEN_CALL_RECOGNIZED = 0x01,
}TwiGenCallRecT;

typedef enum
{
	TWI_PRESCALLER_1,
	TWI_PRESCALLER_4,
	TWI_PRESCALLER_16,
	TWI_PRESCALLER_64,
}TwiPrescallerT;

typedef enum
{
	TWI_WRITE,
	TWI_READ,
}TwiOpModeT;

typedef enum
{
	TWI_NACK,
	TWI_ACK,
}TwiAckT;

typedef void(*TwiCallBkFuncT)(void);

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef struct
{
	TwiBaseT		TwiBase;
	uint8_t			TwiTWAR_Offset;
	uint8_t			TwiTWBR_Offset;
	uint8_t			TwiTWCR_Offset;
	uint8_t			TwiTWDR_Offset;
	uint8_t			TwiTWSR_Offset;
	TwiStatusT		TwiStatus;	//TODO:returns the status of the last operation
	//TwiIntModeT	   	TwiInt;
	uint8_t 		TwiBitRate;				//the value will be set in the TWBR register.
	uint8_t			TwiMySlaveAdd;			//the slave add, me, maybe called by.
	uint8_t			TwiTargetedSlaveAdd;	//the slave add, me, as a master, will talk to.
	TwiPrescallerT	TwiPrescaller;
	TwiGenCallRecT  TwiGenCallRecognition;
	TwiCallBkFuncT	TwiCallBkFunc;
	//TwiOpModeT		TwiOpMode;
	TwiStateT		TwiInitialized;
}TwiT;
/*
 * given that the SCL_FREQ = F_CPU / (16 +2(TWBR).4^TWPS)
 * assuming prescaler = 1 >> and given F_ZPU 8 000 000
 * and we wanna work on 100k SCL_FREQ
 * 8 000 000 / 100 000
 * */
TwiRetT TwiInit(TwiT* Twi);
TwiRetT TwiEdit(TwiT* Twi);
TwiRetT TwiSendStart(TwiT* Twi);
TwiRetT TwiSendRepeatedStart(TwiT* Twi);
TwiRetT TwiSendStop(TwiT* Twi);
TwiRetT TwiSetMySlaveAdd(TwiT* Twi, uint8_t SlaveAdd); //MT > Receive/Write MR > Transmit/Read
TwiRetT TwiSendSlaveAdd(TwiT* Twi, uint8_t SlaveAdd, TwiOpModeT TwiOpMode);	//
TwiRetT TwiSendDataByte(TwiT* Twi, uint8_t* Data);
TwiRetT TwiSendDataArr(TwiT* Twi, uint8_t* DataArr, uint8_t DataLen);
TwiRetT TwiGetDataByte(TwiT* Twi, uint8_t* Data, TwiAckT TwiAck);
TwiRetT TwiGetDataArr(TwiT* Twi,uint8_t* DataArr, uint8_t DataLen);
TwiRetT TwiSlaveWait2bCalled(TwiT* Twi);
TwiRetT TwiGetStatus(TwiT* Twi, TwiStatusT* Status);
//TwiRetT TwiStSendDataByte(TwiT Twi);
//TwiRetT TwiStSendDataArr(TwiT Twi);
//TwiRetT TwiSrReceiveDataByte(TwiT Twi);
//TwiRetT TwiSrReceiveDataArr(TwiT Twi);

//TwiRetT TwiSetMySlaveAdd(TwiT Twi);
//TwiRetT TwiSetCallBkFunc(TwiT Twi, CallBkFuncT CallBkFunc);
//TODO: next
TwiRetT TwiTxFifoConfigSet(uint32_t ui32Base, uint32_t ui32Config);
TwiRetT TwiTxFifoFlush(uint32_t ui32Base);
TwiRetT TwiRxFifoConfigSet(uint32_t ui32Base, uint32_t ui32Config);
TwiRetT TwiRxFifoFlush(uint32_t ui32Base);
#endif /* TWI_H_ */

/*
 * Features • Simple Yet Powerful and Flexible Communication Interface, Only Two Bus Lines Needed
			• Both Master and Slave Operation Supported
			• Device Can Operate as Transmitter or Receiver
			• 7-bit Address Space allows up to 128 Different Slave Addresses
			• Multi-master Arbitration Support
			• Up to 400kHz Data Transfer Speed
			• Slew-rate Limited Output Drivers
			• Noise Suppression Circuitry Rejects Spikes on Bus Lines
			• Fully Programmable Slave Address with General Call Support
			• Address Recognition causes Wake-upwhen AVR is in Sleep Mode
 *TWI protocol allows the systems designer to interconnect up to 128 different devices using only
	two bi-directional bus lines, one for clock (SCL) and one for data (SDA).
 *The only external hardware needed to implement the bus is
 	a single pull-up resistor for each of the TWI bus lines.
 *The bus drivers of all TWI-compliant devices are open-drain or open-collector.
 	 This implements a wired-AND function which is essential to the operation of the interface.
 *Note that all AVR devices connected to the TWI bus must be powered
 	 in order to allow any bus operation.
 *The number of devices that can be connected to the bus is only limited by the bus capacitance
	 limit of 400pF and the 7-bit slave address space.
 *Two different sets of specifications are presented there,
 	 one relevant for bus speeds below 100kHz,and one valid for bus speeds up to 400kHz.
 *Transferring Bits Each data bit transferred on the TWI bus is accompanied by a pulse on the clock line.
 	 The level of the data line must be stable when the clock line is high. The only exception to
 	 this rule is for generating start and stop conditions.
 *scl = cpu clk freq / (16 + 2(twbr) * 4 ^ twps)
 *
 *//// testing TWI
 ////int main(){
 //////DDRC =0xff;
 ////DDRA = 0xff;
 ////
 ////TwiT msa;
 ////
 ////msa.TwiBase = TWI_0_BASE;
 ////msa.TwiBitRate = 32;
 ////msa.TwiGenCallRecognition = TWI_GEN_CALL_RECOGNIZED;
 //////msa.TwiMySlaveAdd = 0x44;	//Master
 //////msa.TwiMySlaveAdd = 0x23;	//Slave One
 ////msa.TwiMySlaveAdd = 0x32;	//Slave two
 ////msa.TwiOpMode = TWI_WRITE;	//Master
 //////msa.TwiOpMode = TWI_READ;	//Slave one
 ////msa.TwiOpMode = TWI_READ;	//Slave two
 ////msa.TwiPrescaller = TWI_PRESCALLER_4;
 ////msa.TwiTWAR_Offset = TWAR_0_OFFSET;
 ////msa.TwiTWBR_Offset = TWBR_0_OFFSET;
 ////msa.TwiTWCR_Offset = TWCR_0_OFFSET;
 ////msa.TwiTWDR_Offset = TWDR_0_OFFSET,
 ////msa.TwiTWSR_Offset = TWSR_0_OFFSET;
 ////uint8_t SlaveOneAdd = 0x23;//Slave One Add
 ////uint8_t SlaveTwoAdd = 0x32;//Slave One Add
 ////TwiInit(&msa);	//TODO Init Gpio Pins
 //////TwiSendStart(&msa);
 //////TwiSendSlaveAdd(&msa,SlaveOneAdd);
 ////uint8_t mma = 0x00;
 ////while(1){
 //////mma = SlaveOneAdd;
 //////TwiSendDataByte(&msa,&mma);
 //////_delay_ms(100);
 //////mma++;
 //////TwiSendDataByte(&msa,&mma);
 //////_delay_ms(100);
 //////
 //////TwiSendRepeatedStart(&msa);
 //////TwiSendSlaveAdd(&msa,SlaveTwoAdd);
 //////
 //////mma = SlaveTwoAdd;
 //////TwiSendDataByte(&msa,&mma);
 //////_delay_ms(100);
 //////mma++;
 //////TwiSendDataByte(&msa,&mma);
 //////_delay_ms(100);
 //////
 //////TwiSendRepeatedStart(&msa);
 //////TwiSendSlaveAdd(&msa,SlaveOneAdd);
 ////TwiGetDataByte(&msa,&mma, TWI_ACK);
 ////PORTD = mma;
 ////_delay_ms(500);
 ////}
 ////return 0;
 ////}
 //*
 //*
 //*
 //*
//*/
