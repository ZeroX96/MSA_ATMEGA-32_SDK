/*
 * Gpio.h
 *
 * Created: 3/20/2020 5:38:18 AM
 *  Author: Mahmo
 */ 


#ifndef GPIO_H_
#define GPIO_H_
#include <stdint.h>
#include "HwAccess.h"
//comment to prevent enabling the of the internal pull-up
//#define INTERNAL_PULL_UP_EN 1
//Gpio Register Addresses
#if 0
#define PORTA	   (0X3B)	//PORTA = PINA + 2 >> PORTA OFFSET = 2
#define DDRA	   (0X3A)	//DDRA = PINA + 1 >> DDRA OFFSET = 1
#define PINA	   (0X39)	//PINA = 39
#define PORTB	   (0X38)
#define DDRB	   (0X37)
#define PINB	   (0X36)
#define PORTC	   (0X35)
#define DDRC	   (0X34)
#define PINC	   (0X33)
#define PORTD	   (0X32)
#define DDRD	   (0X31)
#define PIND	   (0X30) 
#endif //0
#define DDRx_OFFSET  (1U)
#define PORTx_OFFSET (2U)

//with DDRx or PORTx or PINx we would use a uint8_t to accept this val
	//but to be precise and the user will have limited and define options
	//errors will be reduced so we can use an enum
typedef enum 
{
	BASE_A = 0x39U,
	BASE_B = 0x36U,
	BASE_C = 0x33U,
	BASE_D = 0x30U,
}GpioBaseT;
//to determine the pin we may use a #define PIN_0 0
#if 0
#define PIN_0 (0U)
#define PIN_1 (1U)
#define PIN_2 (2U)
#define PIN_3 (3U)
#define PIN_4 (4U)
#define PIN_5 (5U)
#define PIN_6 (6U)
#define PIN_7 (7U)
#endif //0
//again with this way we may need a uint8_t to accept any as parameter
//or we may use an enum
typedef enum
{
	PIN_0 = 0U,
	PIN_1 = 1U,
	PIN_2 = 2U,
	PIN_3 = 3U,
	PIN_4 = 4U,
	PIN_5 = 5U,
	PIN_6 = 6U,
	PIN_7 = 7U,
}GpioPinT;

typedef enum
{
	PORT_IN  = 0x00,
	PORT_OUT = 0xFF,
	PIN_IN   = 0x00,
	PIN_OUT  = 0x01,
	PIN_HIGH = 0x01,
	PIN_LOW  = 0x00,
}GpioStateT;

typedef enum
{
	GPIO_ERR_NO,
	GPIO_ERR_INV_PARAMS,
}GpioRetT;
//Gpio APIs
GpioRetT GpioInitPort(GpioBaseT Base,GpioStateT State);
GpioRetT GpioInitPin(GpioBaseT Base,GpioPinT Pin,GpioStateT State);
GpioRetT GpioWritePort(GpioBaseT Base,uint8_t Val);
GpioRetT GpioWriteGroup(GpioBaseT Base,uint8_t Val, uint8_t Len, uint8_t FirstBit);
GpioRetT GpioWritePin(GpioBaseT Base,GpioPinT Pin,GpioStateT State);
uint8_t  GpioReadPort(GpioBaseT Base);
uint8_t GpioReadGroup(GpioBaseT Base, uint8_t Len, uint8_t FirstBit);
uint8_t  GpioReadPin(GpioBaseT Base,GpioPinT Pin);
#endif /* GPIO_H_ */

#if 0
=The Port Input Pins I/O location is read only, while the Data Register and 
	the Data Direction Register are read/write.

=the DDxn bits are accessed at the DDRx I/O address, the PORTxn bits at the
	PORTx I/O address, and the PINxn bits at the PINx I/O address.

= If DDxn is written logic one, Pxn is configured as an output pin. If DDxn 
	is written logic zero, Pxn is configured as an input pin.

=If PORTxn is written logic one when the pin is configured as an input pin, 
the pull-up resistor is activated.

=To switch the pull-up resistor off, PORTxn has to be written logic zero or
	the pin has to be configured as an output pin.

=If PORTxn is written logic one when the pin is configured as an output pin,
    the port pin is driven high (one). If PORTxn is written logic zero when 
	the pin is configured as an output pin, the port pin is driven low (zero).

=SLEEP is overridden for port pins enabled as External Interrupt pins. 

#endif //0