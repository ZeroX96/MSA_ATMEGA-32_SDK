/*
 * hal_SPI.c
 *
 * Created: 30/10/2018 14:50:32
 *  Author: Mahmoud
 */ 
#include "hal_SPI.h"
#include "hal_SPI_CFG.h"


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
static void (*spi_interrupt_handler)(void);

spi_error_t hal_spiInit(str_spi_objectInfo_t *strg_obj,spi_driver_base_t driver_base,spi_sck_freq_select_t	freq_select,
						spi_operating_mode_t mode,spi_notifcs_mode_t notfics_mode,spi_transfer_modes_t transfer_mode,spi_data_order_t data_order)
{
	spi_error_t ret_val=NO_SPI_ERROR;
	if(  (strg_obj != NULL) && (driver_base == SPI_1_base) && (freq_select >= FREQ_BY_4 && freq_select <= FREQ_BY_64B) &&
	     (mode == SLAVE_EN || mode == MASTER_EN) && (notfics_mode == SPI_POLLING || notfics_mode == SPI_INTERRUPTING) &&
		 ((transfer_mode >= MODE_1) && (transfer_mode <= MODE_4) ) && ((data_order == LSB_FIRST)||(data_order == MSB_FIRST)) 
	  )
	{
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
		*((volatile msa_u8 *)driver_base+CTRL_REG_OFFSET)	|= (freq_select & 0x03)|(transfer_mode<<CPHA)|(mode << MSTR);
		*((volatile msa_u8 *)driver_base+CTRL_REG_OFFSET)	|= (ENABLE<<SPE)|(notfics_mode<<SPIE)|(data_order<<DORD);
		*((volatile msa_u8 *)driver_base+STATUS_REG_OFFSET) = ( (freq_select&0xfc)>>2 );	//test the res is -4?? or worked well...>>edited and worked
		if (notfics_mode == SPI_INTERRUPTING)
		{
			sei();
		}
			
	}
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
	return ret_val;
}

spi_error_t hal_spiRecieveByte(str_spi_objectInfo_t * strg_obj,msa_u8* DataByte)
{
	spi_error_t ret_val=NO_SPI_ERROR;
	if ( (strg_obj != NULL) && (DataByte != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
			
			while(!((*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET)) & (1<<SPIF)))
			;
			*DataByte=(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET));
			(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET))|=(1<<SPIF);
			//where is the fucken add????????????????????????????????????????????????????????????
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
	return ret_val;
}


spi_error_t hal_spiSendByte(str_spi_objectInfo_t * strg_obj,msa_u8* DataByte)
{
	spi_error_t ret_val=NO_SPI_ERROR;
	if ( (strg_obj != NULL) && (DataByte != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
			
			(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET))=*DataByte;
			while(!((*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET)) & (1<<SPIF)))
			;
			//(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET))|=(1<<SPIF);
			//where is the fucken add????????????????????????????????????????????????????????????
			
			
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
	return ret_val;
	
}

spi_error_t hal_spiSendArr(str_spi_objectInfo_t * strg_obj,msa_u8* DataArray)
{
	spi_error_t ret_val=NO_SPI_ERROR;
	if ( (strg_obj != NULL) && (DataArray != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
			msa_u8 i=0;
			for (i=0;*(DataArray+i) ;i++)
			{
				while(!((*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET)) & (1<<SPIF)))
				;
				(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET))=*(DataArray+i);
				(*(volatile msa_u8*)(strg_obj->driver_base_obj+DATA_REG_OFFSET))|=(1<<SPIF);
				//where is the fucken add????????????????????????????????????????????????????????????				
			}
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


	return ret_val;
}

spi_error_t hal_spiRecieveArr(str_spi_objectInfo_t *strg_obj,msa_u8 *DataArray,msa_u8 arr_size)
{
	spi_error_t ret_val=NO_SPI_ERROR;
	msa_u8 data_in_cntr=0,temp=0;
	
	if ( (strg_obj != NULL) && (DataArray != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
			while( (temp != 13) && (data_in_cntr < (arr_size) ) )
			{
				hal_spiRecieveByte(strg_obj,&temp);
				DataArray[data_in_cntr++]=temp;
				if(temp == '\0')
				break;
			}
			DataArray[data_in_cntr]=0;
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
	return ret_val;
}


spi_error_t hal_spiDeinit(str_spi_objectInfo_t *strg_obj)
{
	spi_error_t ret_val=NO_SPI_ERROR;
	if ( (strg_obj != NULL))
	{
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
	}
	else
	{
		ret_val=INVALID_SPI_PARAMS;
	}
	return ret_val;
}


spi_error_t hal_setSpiIsrCallback(str_spi_objectInfo_t * strg_obj,void (*vptr_cb)(void))
{
	spi_error_t ret_val=NO_SPI_ERROR;
	if ( (strg_obj != NULL) && (vptr_cb != NULL) )
	{
		if (strg_obj->driver_state_obj == DRIVER_INITIATED)
		{
			spi_interrupt_handler=vptr_cb;

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
	
	return ret_val;
	
	
}

ISR(SPI_STC_vect)
{
	if (spi_interrupt_handler)
	{
		(*spi_interrupt_handler)();
	}
}
