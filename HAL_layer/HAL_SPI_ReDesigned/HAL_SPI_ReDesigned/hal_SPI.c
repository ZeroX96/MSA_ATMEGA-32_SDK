/*
 * hal_SPI.c
 *
 * Created: 30/10/2018 14:50:32
 *  Author: Mahmoud
 */ 
#include "hal_SPI.h"


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

static void (*spi_interrupt_handler)(void);

spi_error_t hal_spiInit(str_spi_objectInfo_t *strg_obj,spi_driver_base_t driver_base,spi_sck_freq_select_t	freq_select,
						spi_operating_mode_t mode,spi_notifcs_mode_t notfics_mode,spi_transfer_modes_t transfer_mode,spi_data_order_t data_order)
{
	spi_error_t ret_val=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if(  (strg_obj != NULL) && (driver_base == SPI_1_base) && (freq_select >= FREQ_BY_4 && freq_select <= FREQ_BY_64B) &&
	     (mode == SLAVE_EN || mode == MASTER_EN) && (notfics_mode == SPI_POLLING || notfics_mode == SPI_INTERRUPTING) &&
		 ((transfer_mode >= MODE_1) && (transfer_mode <= MODE_4) ) && ((data_order == LSB_FIRST)||(data_order == MSB_FIRST)) 
	  )
	{
#endif
		cli();//to finish the driver initializations first
		//storing data 
		strg_obj->driver_base_obj	=driver_base;
		strg_obj->driver_state_obj	=DRIVER_INITIATED;
		strg_obj->freq_select_obj	=freq_select;
		strg_obj->mode_obj			=mode;
		strg_obj->transfer_mode_obj	=transfer_mode;
		strg_obj->notfics_mode_obj	=notfics_mode;
		strg_obj->data_order_obj	=data_order;
		
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
			*((volatile msa_u8 *)driver_base+CTRL_REG_OFFSET)	|= (freq_select & 0x03)|(transfer_mode<<CPHA)|(mode << MSTR);
			*((volatile msa_u8 *)driver_base+CTRL_REG_OFFSET)	|= (ENABLE<<SPE)|(notfics_mode<<SPIE)|(data_order<<DORD);
			*((volatile msa_u8 *)driver_base+STATUS_REG_OFFSET) |= ( (freq_select&0xfcUL)>>2 );	//test the res is -4?? or worked well...>>edited and worked
			
			
		} 
		else //slave_en
		{
			//again,this is according to the atmega32 mcu
			CLEAR_BIT(DDRB,SPI_PIN_SS);
			CLEAR_BIT(DDRB,SPI_PIN_SCK);
			CLEAR_BIT(DDRB,SPI_PIN_MOSI);
			SET_BIT  (DDRB,SPI_PIN_MISO);
			//actual initialization
			*((volatile msa_u8 *)driver_base+CTRL_REG_OFFSET)	|= (ENABLE<<SPE)|(notfics_mode<<SPIE);
			
		}
		if (notfics_mode == SPI_INTERRUPTING) //re-edit not to override the interrupt status,or the condition with the previous interrupt status
		{
			sei();
		}
#if (DEBUGGING == 1)
			
	}
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
#endif
	return ret_val;
}

spi_error_t hal_spiExchangeDATA(str_spi_objectInfo_t * strg_obj,msa_u8 *ByteOUT,msa_u8 *ByteIN)
{
	spi_error_t ret_val=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ( (strg_obj != NULL) && (ByteOUT != NULL) && (ByteIN != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
#endif
			_delay_us(2);////for safety but edit if made an error with the CAN Driver
			//put the outgoing byte to be sent
			(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET))=*ByteOUT;
			//wait the exchange completion
			while(!((*(volatile msa_u8*)(strg_obj->driver_base_obj+STATUS_REG_OFFSET)) & (1<<SPIF)))//fixed an error,was testing the 7th bit in the data reg wich is wrong
			;
			//take the incoming byte that was received
			*ByteIN=(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET));

#if (DEBUGGING == 1)		
		}
		else
		{
			ret_val=DRIVER_NOT_INITIATED;
		}
	}
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
#endif
	return ret_val;
	
}

spi_error_t hal_spiExchangeDataPacket(str_spi_objectInfo_t * strg_obj,msa_u8 *PacketOUT,msa_u8 *PacketIN,msa_u8 Packet_Size)
{
	spi_error_t ret_val=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ((strg_obj != NULL) && (PacketOUT != NULL) && (PacketIN != NULL) && (Packet_Size >= 0) )
	{
#endif
		msa_u8 data_counter=0;
		while(Packet_Size > 0)//ooooooooooh yeaaaaaaaaaaaaaaah
		{
			hal_spiExchangeDATA(strg_obj,(PacketOUT+data_counter),(PacketIN+data_counter));
			Packet_Size--;
			data_counter++;
		}
#if (DEBUGGING == 1)
	} 
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
#endif
	return ret_val;
}

spi_error_t hal_spiDeinit(str_spi_objectInfo_t *strg_obj)
{
	spi_error_t ret_val=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ( (strg_obj != NULL))
	{
#endif
		//CLEARING INFO
		strg_obj->data_order_obj=0;
		strg_obj->driver_base_obj=0;
		strg_obj->driver_state_obj=DRIVER_NOT_INITIATED;
		strg_obj->freq_select_obj=0;
		strg_obj->mode_obj=0;
		strg_obj->notfics_mode_obj=0;
		strg_obj->transfer_mode_obj=0;
	
		//deinitiating the driver registers
		(*(volatile msa_u8*)(strg_obj->driver_base_obj+CTRL_REG_OFFSET))=0X00;
		(*(volatile msa_u8*)(strg_obj->driver_base_obj+STATUS_REG_OFFSET))=0X00;
		spi_interrupt_handler=NULL;
#if (DEBUGGING == 1)
	}
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
#endif
	return ret_val;
}

spi_error_t hal_setSpiIsrCallback(str_spi_objectInfo_t * strg_obj,void (*vptr_cb)(void))
{
	spi_error_t ret_val=NO_SPI_ERROR;
#if (DEBUGGING == 1)
	if ( (strg_obj != NULL) && (vptr_cb != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
#endif
			spi_interrupt_handler=vptr_cb;
#if (DEBUGGING == 1)
		}
		else
		{
			ret_val=SPI_DRIVER_NOT_INITIATED;
		}
	}
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
#endif
	return ret_val;
	
	
}

ISR(SPI_STC_vect)
{
	if (spi_interrupt_handler)
	{
		(void)(*spi_interrupt_handler)();
	}
}