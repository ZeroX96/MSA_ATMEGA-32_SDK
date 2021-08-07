/*
 * hal_SPI.c
 *
 * Created: 30/10/2018 14:50:32
 *  Author: Mahmoud
 */ 
#include "SPI.h"


#define CTRL_REG_OFFSET		0
#define STATUS_REG_OFFSET	1
#define DATA_REG_OFFSET		2
#define SPR0				0	//bits 0&1 used to config the freq
#define CPHA				2	//bits 2&3 used to config the transfer mode
#define MSTR				4	//bit  4   used to config the driver mode
#define	DORD				5	//bit  5   used to config the data order
#define SPE					6	//bit  6   used to enable the driver
#define SPIE				7	//bit  7   used to enable the driver interrupts
#define ENABLE				1
#define SPIF				7

//SPI_PINS according to the atmega32,otherwise..edit this
#define SPI_PIN_MOSI		5
#define SPI_PIN_MISO		6
#define SPI_PIN_SCK			7
#define SPI_PIN_SS			4

static void (*SpiHandler)(void);

SpiRetT SpiInit(SpiT *SpiObj,SpiBaseT Base,SpiFreqT	FreqSelect,
						SpiOpModeT mode,SpiNotificsModeT NotificsMode,SpiTransferModeT TransferMode,SpiDataOrderT DataOrder)
{
	SpiRetT RetVal=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if(  (SpiObj != NULL) && (Base == SPI_1_base) && (FreqSelect >= FREQ_BY_4 && FreqSelect <= FREQ_BY_64B) &&
	     (mode == SLAVE_EN || mode == MASTER_EN) && (NotificsMode == SPI_POLLING || NotificsMode == SPI_INTERRUPTING) &&
		 ((TransferMode >= MODE_1) && (TransferMode <= MODE_4) ) && ((DataOrder == LSB_FIRST)||(DataOrder == MSB_FIRST)) 
	  )
	{
#endif
		cli();//to finish the driver initializations first
		//storing data 
		SpiObj->SpiBase	=Base;
		SpiObj->SpiState =DRIVER_INITIATED;
		SpiObj->SpiFreq	=FreqSelect;
		SpiObj->SpiOpMode =mode;
		SpiObj->SpiTransferMode	=TransferMode;
		SpiObj->SpiNotificsMode	=NotificsMode;
		SpiObj->SpiDataOrder	=DataOrder;
		
		//actual initialization
		//&
		//configuring the driver pins
		if (mode == MASTER_EN)
		{
			//again,this is according to the atmega32 mcu
			SET_BIT(DDRB,SPI_PIN_SS);
			SET_BIT(DDRB,SPI_PIN_SCK);
			SET_BIT(DDRB,SPI_PIN_MOSI);
			CLEAR_BIT(DDRB,SPI_PIN_MISO);
			//actual initialization
			*((volatile uint8_t *)Base+CTRL_REG_OFFSET)	|= (FreqSelect & 0x03)|(TransferMode<<CPHA)|(mode << MSTR);
			*((volatile uint8_t *)Base+CTRL_REG_OFFSET)	|= (ENABLE<<SPE)|(NotificsMode<<SPIE)|(DataOrder<<DORD);
			*((volatile uint8_t *)Base+STATUS_REG_OFFSET) |= ( (FreqSelect&0xfcUL)>>2 );	//test the res is -4?? or worked well...>>edited and worked
			
			
		} 
		else //slave_en
		{
			//again,this is according to the atmega32 mcu
			CLEAR_BIT(DDRB,SPI_PIN_SS);
			CLEAR_BIT(DDRB,SPI_PIN_SCK);
			CLEAR_BIT(DDRB,SPI_PIN_MOSI);
			SET_BIT  (DDRB,SPI_PIN_MISO);
			//actual initialization
			*((volatile uint8_t *)Base+CTRL_REG_OFFSET)	|= (ENABLE<<SPE)|(NotificsMode<<SPIE);
			
		}
		if (NotificsMode == SPI_INTERRUPTING) //re-edit not to override the interrupt status,or the condition with the previous interrupt status
		{
			sei();
		}
#if (DEBUGGING == 1)
			
	}
	else
	{
		RetVal=INVALID_SPI_PARAMS;
	}
#endif
	return RetVal;
}

SpiRetT SpiExchangeDATAEdited(SpiT * SpiObj,uint8_t *ByteOUT,uint8_t *ByteIN)
{
	SpiRetT RetVal=NO_SPI_ERROR;
	#if (DEBUGGING == 1)
	if ( (SpiObj != NULL) && (ByteOUT != NULL) && (ByteIN != NULL) )
	{
		if (SpiObj->SpiState == DRIVER_INITIATED)
		{
			#endif
			_delay_us(1);////for safety but edit if made an error with the CAN Driver
			//put the outgoing byte to be sent
			(*(volatile uint8_t*)(SpiObj->SpiBase+DATA_REG_OFFSET))=*ByteOUT;
			//SET_BIT(DDRA,0);
			//wait the exchange completion
			volatile unsigned int protection=0;
			while(!((*(volatile uint8_t*)(SpiObj->SpiBase+STATUS_REG_OFFSET)) & (1<<SPIF)))//fixed an error,was testing the 7th bit in the data reg wich is wrong
			{	
				//PORTA |= 1<<1;_delay_us(10);PORTA&= ~(1<<1);_delay_us(10);protection++;
				if (protection >= 2000 )
				{
					protection=0;
					break;
				}
			}
			//CLEAR_BIT(DDRA,0);
			//take the incoming byte that was received
			*ByteIN=(*(volatile uint8_t*)(SpiObj->SpiBase+DATA_REG_OFFSET));

			#if (DEBUGGING == 1)
		}
		else
		{
			RetVal=DRIVER_NOT_INITIATED;
		}
	}
	else
	{
		RetVal=INVALID_SPI_PARAMS;
	}
	#endif
	return RetVal;
	
}


SpiRetT SpiExchangeDATA(SpiT * SpiObj,uint8_t *ByteOUT,uint8_t *ByteIN)
{
	SpiRetT RetVal=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ( (SpiObj != NULL) && (ByteOUT != NULL) && (ByteIN != NULL) )
	{
		if (SpiObj->SpiState == DRIVER_INITIATED)
		{
#endif
			_delay_us(2);////for safety but edit if made an error with the CAN Driver
			//put the outgoing byte to be sent
			(*(volatile uint8_t*)(SpiObj->SpiBase+DATA_REG_OFFSET))=*ByteOUT;
			//wait the exchange completion
			while(!((*(volatile uint8_t*)(SpiObj->SpiBase+STATUS_REG_OFFSET)) & (1<<SPIF)))//fixed an error,was testing the 7th bit in the data reg wich is wrong
			;
			//take the incoming byte that was received
			*ByteIN=(*(volatile uint8_t*)(SpiObj->SpiBase+DATA_REG_OFFSET));

#if (DEBUGGING == 1)		
		}
		else
		{
			RetVal=DRIVER_NOT_INITIATED;
		}
	}
	else
	{
		RetVal=INVALID_SPI_PARAMS;
	}
#endif
	return RetVal;
	
}

SpiRetT SpiExchangeDataPacket(SpiT * SpiObj,uint8_t *PacketOUT,uint8_t *PacketIN,uint8_t Packet_Size)
{
	SpiRetT RetVal=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ((SpiObj != NULL) && (PacketOUT != NULL) && (PacketIN != NULL) && (Packet_Size >= 0) )
	{
#endif
		uint8_t data_counter=0;
		while(Packet_Size > 0)//ooooooooooh yeaaaaaaaaaaaaaaah
		{
			SpiExchangeDATA(SpiObj,(PacketOUT+data_counter),(PacketIN+data_counter));
			Packet_Size--;
			data_counter++;
		}
#if (DEBUGGING == 1)
	} 
	else
	{
		RetVal=INVALID_SPI_PARAMS;
	}
#endif
	return RetVal;
}

SpiRetT SpiDeinit(SpiT *SpiObj)
{
	SpiRetT RetVal=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ( (SpiObj != NULL))
	{
#endif
		//CLEARING INFO
		SpiObj->SpiDataOrder=0;
		SpiObj->SpiBase=0;
		SpiObj->SpiState=DRIVER_NOT_INITIATED;
		SpiObj->SpiFreq=0;
		SpiObj->SpiOpMode=0;
		SpiObj->SpiNotificsMode=0;
		SpiObj->SpiTransferMode=0;
	
		//deinitiating the driver registers
		(*(volatile uint8_t*)(SpiObj->SpiBase+CTRL_REG_OFFSET))=0X00;
		(*(volatile uint8_t*)(SpiObj->SpiBase+STATUS_REG_OFFSET))=0X00;
		SpiHandler=NULL;
#if (DEBUGGING == 1)
	}
	else
	{
		RetVal=INVALID_SPI_PARAMS;
	}
#endif
	return RetVal;
}

SpiRetT SpiSetIsrCallback(SpiT * SpiObj,void (*vptr_cb)(void))
{
	SpiRetT RetVal=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ( (SpiObj != NULL) && (vptr_cb != NULL) )
	{
		if (SpiObj->SpiState == DRIVER_INITIATED)
		{
#endif
			SpiHandler=vptr_cb;
#if (DEBUGGING == 1)
		}
		else
		{
			RetVal=SPI_DRIVER_NOT_INITIATED;
		}
	}
	else
	{
		RetVal=INVALID_SPI_PARAMS;
	}
#endif
	return RetVal;
	
	
}

ISR(SPI_STC_vect)
{
	if (SpiHandler)
	{
		(void)(*SpiHandler)();
	}
}