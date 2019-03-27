/*
 * HAL_SPI_ReDesigned.c
 *
 * Created: 3/27/2019 4:14:58 AM
 * Author : MSA
 */ 

#include "hal_SPI.h"

int main(void)
{
	DDRD=0xff;
	str_spi_objectInfo_t obj_1;
	msa_u8 data_out=20,data_in;
	hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_16,SLAVE_EN,SPI_POLLING,MODE_1,MSB_FIRST);
	_delay_ms(10);
	//hal_spiSendByte(&obj_1,&data_add);
	while (1)
	{
		_delay_ms(1000);
		hal_spiExchangeDATA(&obj_1,&data_out,&data_in);
		PORTD=data_in;
		//
		data_out-=2;
	}
}