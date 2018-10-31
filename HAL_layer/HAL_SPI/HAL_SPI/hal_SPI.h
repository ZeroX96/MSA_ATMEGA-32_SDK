/*
 * hal_SPI.h
 *
 * Created: 30/10/2018 14:50:45
 *  Author: Mahmoud
 */ 


#ifndef HAL_SPI_H_
#define HAL_SPI_H_

#define FALLING_EDGE 
#define RISING_EDGE
#define CLOCK_POLARIT RISING_EDGE

typedef enum
{
	MODE_1=0x00,	//Read On Rising  Edge,Changed On Falling Edge
	MODE_2=0x01,	//Read On Falling Edge,Changed On Rising  Edge
	MODE_3=0x02,	//Read On Falling Edge,Changed On Rising  Edge
	MODE_4=0x03,	//Read On Rising  Edge,Changed On Falling Edge
}spi_modes_t;

typedef enum
{	//spif will be one when the slave is called or the master finished sending and the slave that data came
	SPI_POLLING,
	SPI_INTERRUPTING,
}spi_notifa_mode;

typedef enum
{
	NORMAL_SPEED,
	DOUBLE_SPEED,	//chk if the mstr otherwise cant and ret error or wrning telling it's not the mster asking to do that
}spi_speed_select;

typedef enum
{
	SLAVE_EN=0x00,
	MASTER_EN=0x01,
}spi_mode;


typedef enum
{
	FREQ_BY_4=0,
	FREQ_BY_16,
	FREQ_BY_64,
	FREQ_BY_128,
	FREQ_BY_2,
	FREQ_BY_8,
	FREQ_BY_32,
	FREQ_BY_64,
}spi_sck_freq_select;

/*
typedef enum
{
	
}spi_;
*/

//7th-bit = 0 for reading
//7th-bit = 1 for writing
//SPCOL will be set if i write on SPDR during a data transfer op.
 
//SPI_STEPS
/*
*	Make CE = 0 To Begin Writing/Reading						//Cause it's Active LOW 
*	Make The 7th_ADD bit According to your Goal					//Will Read or Write ??
*	The Add byte is sent first 
*	The data byte is sent after the add byte
*	If It's Only A byte,Make CE = 1 To End Writing				//Communication Finished
*	If Not, The next Data Byte Follows the One being Sent Out
*	After The End The Last Byte ,Make CE = 1 To End Writing				//Communication Finished
	*/



#endif /* HAL_SPI_H_ */