/*
 * hal_SPI_CFG.h
 *
 * Created: 30/10/2018 14:51:14
 *  Author: Mahmoud
 */ 


#ifndef HAL_SPI_CFG_H_
#define HAL_SPI_CFG_H_

#define F_CPU 16000000UL




#endif /* HAL_SPI_CFG_H_ */

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