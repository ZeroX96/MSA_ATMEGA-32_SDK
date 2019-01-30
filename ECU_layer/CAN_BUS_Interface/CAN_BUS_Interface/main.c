/*
 * CAN_BUS_Interface.c
 *
 * Created: 25/01/2019 09:54:27
 * Author : Mahmoud
 */ 
#include "includes.h"

int main(void)
{
	str_spi_objectInfo_t obj_1;
	hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_16,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);
	_delay_us(10);
	while (1)
	{
		PORTA=0xff;
		_delay_ms(1);
		PORTA=0x00;
		_delay_ms(1);
		
	}
	return 0;
}

//test_1_spi_modified
#if 0
int main(void)
{
	DDRD=0xff;
	DDRC=0xff;
	str_spi_objectInfo_t obj_1;
	msa_u8 data_out[10]={1,7,4,5,7,9,54,7,9,10};
	msa_u8 data_in[10]={0};
	hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_16,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);
	_delay_ms(10);
	while (1)
	{
		hal_spiExchangeDataPacket(&obj_1,data_out,data_in,10);
		
		
		for (msa_u8 i=0;i<10;i++)
		{
			if (data_in[i] == 7)
			{
				PORTC++;
			}
			PORTD++;
			_delay_ms(50);
		}
		
		//
		//data_add++;
	}
}
#endif

//test_0_spi_modified
#if 0
 int main(void)
 {
	 DDRD=0x00;
	 DDRC=0x00;
	 str_spi_objectInfo_t obj_1;
	 msa_u8 data_out=0,data_in=0;
	 hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_16,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);
	 _delay_ms(10);
	 while (1)
	 {
		 hal_spiExchangeDATA(&obj_1,&data_out,&data_in);
		 _delay_ms(50);
		 PORTD++;
		 data_out++;
		 if (data_in == 7)
		 {
			 PORTC++;
		 }
		 //
		 //data_add++;
	 }
 }
 #endif