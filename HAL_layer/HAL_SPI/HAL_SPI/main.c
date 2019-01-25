/*
 * HAL_SPI.c
 *
 * Created: 30/10/2018 14:15:43
 * Author : Mahmoud
 */ 
#include <avr/io.h>
#include "hal_SPI.h"
// to remember the driver func.s
#if 0
spi_error_t hal_spiInit( str_spi_objectInfo_t *strg_obj,spi_driver_base_t driver_base,  spi_sck_freq_select_t	freq_select,
						 spi_operating_mode_t mode,     spi_notifcs_mode_t notfics_mode,spi_transfer_modes_t    transfer_mode,
						 spi_data_order_t     data_order
						);
spi_error_t hal_spiSendByte(str_spi_objectInfo_t       *strg_obj,msa_u8 *DataByte);
spi_error_t hal_spiSendArr(str_spi_objectInfo_t        *strg_obj,msa_u8 *DataArray);
spi_error_t hal_spiRecieveByte(str_spi_objectInfo_t *strg_obj,msa_u8 *DataByte,msa_u8 *Data2Bexchanged);//data2Bexchanged is the data to be sent when the master initiates the clock and the default is 0xff
spi_error_t hal_spiRecieveArr(str_spi_objectInfo_t     *strg_obj,msa_u8 *DataArray,msa_u8 arr_size);
spi_error_t hal_spiDeinit(str_spi_objectInfo_t         *strg_obj);
spi_error_t hal_setSpiIsrCallback(str_spi_objectInfo_t *strg_obj,void (*vptr_cb)(void));
#endif
//The Master Portion of code
#if 0
int main(void)
{
	DDRD=0xff;
	str_spi_objectInfo_t obj_1;
	msa_u8 data_add=7;
	hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_16,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);
	_delay_ms(10);
	//hal_spiSendByte(&obj_1,&data_add);
    while (1) 
    {
		hal_spiSendByte(&obj_1,&data_add);
		_delay_ms(1000);
		PORTD++;
		//
		//data_add++;
    }
}
#endif
//the Slave Portion of Code
#if 1
int main(void)
{
	DDRD=0xff;
	DDRC=0xff;
	str_spi_objectInfo_t obj_1;
	//msa_u8 data_add='m';
	msa_u8 data_got=0;
	msa_u8 data2Bexchanged=0x55;
	hal_spiInit(&obj_1,SPI_1_base,0,0,0,0,0);
	_delay_ms(10);
	//hal_spiSendByte(&obj_1,&data_add);
	while (1)
 	{
		//hal_spiSendByte(&obj_1,&data_add);
		hal_spiRecieveByte(&obj_1,&data_got,&data2Bexchanged);
		PORTD++;
		_delay_ms(1000);
		if (data_got == 7)
		{
			PORTC++;
		}
		//data_add++;
	}
}
#endif