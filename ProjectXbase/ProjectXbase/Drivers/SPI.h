/*
 * hal_SPI.h
 *
 * Created: 30/10/2018 14:50:45
 *  Author: Mahmoud
 */ 


#ifndef HAL_SPI_H_
#define HAL_SPI_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/*
#define FALLING_EDGE 
#define RISING_EDGE
#define CLOCK_POLARIT RISING_EDGE
*/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif //F_CPU

#ifndef NULL
#define NULL ((void*)0)
#endif //NULL
//#define DEBUGGING 0


typedef enum
{
	MODE_1=0x00,	//Read On Rising  Edge,Changed On Falling Edge
	MODE_2=0x01,	//Read On Falling Edge,Changed On Rising  Edge
	MODE_3=0x02,	//Read On Falling Edge,Changed On Rising  Edge
	MODE_4=0x03,	//Read On Rising  Edge,Changed On Falling Edge
}SpiTransferModeT;

typedef enum
{	//spif will be one when the slave is called or the master finished sending and the slave that data came
	SPI_POLLING,
	SPI_INTERRUPTING,
}SpiNotificsModeT;

typedef enum
{
	SLAVE_EN=0x00,
	MASTER_EN=0x01,
}SpiOpModeT;


typedef enum
{
	FREQ_BY_4	=0x00,
	FREQ_BY_16	=0x01,
	FREQ_BY_64A	=0x02,
	FREQ_BY_128	=0x03,
	FREQ_BY_2	=0x04,	//it's better not to use as the slave wont work well with u !!!!
	FREQ_BY_8	=0x05,
	FREQ_BY_32	=0x06,
	FREQ_BY_64B	=0x07,	//FREQ_BY_64A & FREQ_BY_64B Are the Same just fuck the designer's mind :{ 
}SpiFreqT;

#define THE_MSTR_MAY_B_SLAVE
//in init chck if master >>and this definition is defined then make the pin input other-wise,NotDefined>>make the pin as output
//and if slave,directly make the pin as input

typedef enum
{
	NO_SPI_ERROR,
	INVALID_SPI_PARAMS,
	SPI_DRIVER_NOT_INITIATED,
}SpiRetT;

typedef enum
{
	SPI_1_base=0x2D,	//the add of the ctrl reg of this specific module so if there are more than one spi,
	//add here :D
}SpiBaseT;

typedef enum
{
	DRIVER_NOT_INITIATED,
	DRIVER_INITIATED,
}SpiStateT;

typedef enum
{
	MSB_FIRST,
	LSB_FIRST,
}SpiDataOrderT;


typedef struct
{
	SpiBaseT		 SpiBase;
	SpiFreqT	     SpiFreq;
	SpiOpModeT	     SpiOpMode;
	SpiNotificsModeT SpiNotificsMode;
	SpiTransferModeT SpiTransferMode;
	SpiDataOrderT	 SpiDataOrder;
	SpiStateT		 SpiState;
}SpiT;

//SPCOL will be set if i write on SPDR during a data transfer op.
SpiRetT SpiInit( SpiT *SpiObj,SpiBaseT Base,SpiFreqT	FreqSelect,
				 SpiOpModeT mode,SpiNotificsModeT NotificsMode,SpiTransferModeT TransferMode,
				 SpiDataOrderT DataOrder
				 );
SpiRetT SpiExchangeDATA(SpiT * SpiObj,uint8_t *ByteOUT,uint8_t *ByteIN);
SpiRetT SpiExchangeDATAEdited(SpiT * SpiObj,uint8_t *ByteOUT,uint8_t *ByteIN);
SpiRetT SpiExchangeDataPacket(SpiT * SpiObj,uint8_t *PacketOUT,uint8_t *PacketIN,uint8_t PacketSize);
SpiRetT SpiDeinit(SpiT *SpiObj);
SpiRetT SpiSetIsrCallback(SpiT * SpiObj,void (*vptr_cb)(void));



//The Master Portion of code
#if 0
int main(void)
{
	DDRD=0xff;
	SpiT obj_1;
	uint8_t data_add=7;
	SpiInit(&obj_1,SPI_1_base,FREQ_BY_16,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);
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

#endif /* HAL_SPI_H_ */
