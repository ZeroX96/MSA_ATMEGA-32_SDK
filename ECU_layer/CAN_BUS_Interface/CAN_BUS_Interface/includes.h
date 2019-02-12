/*
 * includes.h
 *
 * Created: 25/01/2019 09:56:42
 *  Author: Mahmoud
 */ 


#ifndef INCLUDES_H_
#define INCLUDES_H_

//Generally
#include <avr/io.h>
#include "common.h"

//used the spi driver but only the needed functions and edited some function names
#include "spi/hal_SPI_CFG.h"
#include "spi/hal_SPI.h"

//for the Interrupts
#include <avr/interrupt.h>
#include "EXT_INTERRUPTS/external_interrupts.h"

//for the delay functions and will revisit when using the RTOS,NOTE>>>>>>>>>>>>>>>>>>>>>>>
#include <util/delay.h>



#endif /* INCLUDES_H_ */