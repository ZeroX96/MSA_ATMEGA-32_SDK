/*
 * HAL_SPI.c
 *
 * Created: 30/10/2018 14:15:43
 * Author : Mahmoud
 */ 

#include <avr/io.h>
#include "hal_SPI.h"

/*
spi_error_t hal_spiInit( str_spi_objectInfo_t *strg_obj,spi_driver_base_t driver_base,spi_sck_freq_select_t	freq_select,
spi_operating_mode_t mode,spi_notifcs_mode_t notfics_mode,spi_transfer_modes_t transfer_mode,
spi_data_order_t data_order
);
spi_error_t hal_spiSendByte(str_spi_objectInfo_t *strg_obj,msa_u8 *DataByte);
spi_error_t hal_spiSendArr(str_spi_objectInfo_t * strg_obj,msa_u8* DataArray);
spi_error_t hal_spiRecieveByte(str_spi_objectInfo_t *strg_obj,msa_u8 *DataByte);
spi_error_t hal_spiRecieveArr(str_spi_objectInfo_t *strg_obj,msa_u8 *DataArray,msa_u8 arr_size);
spi_error_t hal_spiDeinit(str_spi_objectInfo_t *strg_obj);
spi_error_t hal_setSpiIsrCallback(str_spi_objectInfo_t * strg_obj,void (*vptr_cb)(void));*/

int main(void)
{
	DDRD=0xff;
	str_spi_objectInfo_t obj_1;
	msa_u8 data_add='m';
	hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_32,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);
	hal_spiSendByte(&obj_1,&data_add);
    while (1) 
    {
		PORTD^=0x45;
    }
}

