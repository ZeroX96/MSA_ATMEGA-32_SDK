/*
 * KeyPad.h
 *
 * Created: 4/25/2020 11:29:33 PM
 *  Author: Mahmoud
 */ 
#if 0

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include "HwAccess.h"
#include "Gpio.h"
/*
	NOTE
	ROWS are used as outputs 
	and
	columns are used as inputs
*/
#define KPM_BUFFER_LENGTH	(10U)

#define KPM_NUMB_COL	(4U)

#define KPM_NUMB_ROW	(4U)

/*ERROR SOLVED HERE
 * dont leave the hash-defines without any val, this will cause an error
 */
//TODO:Revise
#define COLUMNS_STARTING_BIT	(0x04)

//TODO:Revise
#define ROWS_STARTING_BIT		(0x01)

#define KPM_NO_KEY_PRESSED		(0xFF)
#define KPM_NO_TIMEOUT			(0x00)

#define KPM_KEY_NOT_PRESSED		(0U)
#define KPM_KEY_DEBOUNCE		(1U)
#define KPM_KEY_PRESSED			(2U)

#define KPM_AUTO_REPEATE_DELAY	250
#define KPM_TIMER_ISR_PERIOD	10
#ifndef NULL
#define NULL ((void*)0)
#endif

typedef enum
{
	KPM_ERR_NO,
	KPM_ERR_INV_PARAMS,	
	KPM_ERR_NOT_INITIALIZED,
}KeyPadRetT;

typedef enum
{
	KPM_NOT_INITIATED,
	KPM_INITIATED,
}KeyPadStateT;

typedef struct{
	GpioBaseT	 ColumnsBase;
	GpioBaseT	 RowsBase;
	KeyPadStateT State;
}KeyPadT;

KeyPadRetT KeyPadInit(KeyPadT*);
KeyPadRetT KeyPadFlush(KeyPadT*);
KeyPadRetT KeyPadGetKeyBlocking(KeyPadT*,uint8_t* Key,uint16_t TimeOut);
KeyPadRetT KeyPadGetKeyNonBlocking(KeyPadT*,uint8_t* Key);
KeyPadRetT KeyPadGetKeyDwnTime(KeyPadT*,uint8_t* Key);

#endif /* KEYPAD_H_ */

#endif //0