/*
 * hal_SPI.h
 *
 * Created: 30/10/2018 14:50:45
 *  Author: Mahmoud
 */ 


#ifndef HAL_SPI_H_
#define HAL_SPI_H_
#include <avr/io.h>
#include "hal_SPI_CFG.h"
#include "common.h"
#include <avr/interrupt.h>
#include <util/delay.h>
/*
#define FALLING_EDGE 
#define RISING_EDGE
#define CLOCK_POLARIT RISING_EDGE
*/

typedef enum
{
	MODE_1=0x00,	//Read On Rising  Edge,Changed On Falling Edge
	MODE_2=0x01,	//Read On Falling Edge,Changed On Rising  Edge
	MODE_3=0x02,	//Read On Falling Edge,Changed On Rising  Edge
	MODE_4=0x03,	//Read On Rising  Edge,Changed On Falling Edge
}spi_transfer_modes_t;

typedef enum
{	//spif will be one when the slave is called or the master finished sending and the slave that data came
	SPI_POLLING,
	SPI_INTERRUPTING,
}spi_notifcs_mode_t;

typedef enum
{
	SLAVE_EN=0x00,
	MASTER_EN=0x01,
}spi_operating_mode_t;


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
}spi_sck_freq_select_t;

#define THE_MSTR_MAY_B_SLAVE
//in init chck if master >>and this definition is defined then make the pin input other-wise,NotDefined>>make the pin as output
//and if slave,directly make the pin as input

typedef enum
{
	NO_SPI_ERROR,
	INVALID_SPI_PARAMS,
	SPI_DRIVER_NOT_INITIATED,
}spi_error_t;

typedef enum
{
	SPI_1_base=0x2D,	//the add of the ctrl reg of this specific module so if there are more than one spi,
	//add here :D
}spi_driver_base_t;

typedef enum
{
	DRIVER_NOT_INITIATED,
	DRIVER_INITIATED,
}spi_cfg_state_t;

typedef enum
{
	MSB_FIRST,
	LSB_FIRST,
}spi_data_order_t;


typedef struct
{
	spi_driver_base_t		driver_base_obj;
	spi_sck_freq_select_t	freq_select_obj;
	spi_operating_mode_t	mode_obj;
	spi_notifcs_mode_t		notfics_mode_obj;
	spi_transfer_modes_t	transfer_mode_obj;
	spi_data_order_t		data_order_obj;
	spi_cfg_state_t			driver_state_obj;
}str_spi_objectInfo_t;

//SPCOL will be set if i write on SPDR during a data transfer op.
spi_error_t hal_spiInit( str_spi_objectInfo_t *strg_obj,spi_driver_base_t driver_base,spi_sck_freq_select_t	freq_select,
						 spi_operating_mode_t mode,spi_notifcs_mode_t notfics_mode,spi_transfer_modes_t transfer_mode,
						 spi_data_order_t data_order
					   );
spi_error_t hal_spiExchangeDATA(str_spi_objectInfo_t * strg_obj,msa_u8 *ByteOUT,msa_u8 *ByteIN);
spi_error_t hal_spiExchangeDataPacket(str_spi_objectInfo_t * strg_obj,msa_u8 *PacketOUT,msa_u8 *PacketIN,msa_u8 PacketSize);
spi_error_t hal_spiDeinit(str_spi_objectInfo_t *strg_obj);
spi_error_t hal_setSpiIsrCallback(str_spi_objectInfo_t * strg_obj,void (*vptr_cb)(void));

#endif /* HAL_SPI_H_ */
