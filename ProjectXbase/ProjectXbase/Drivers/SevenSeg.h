/*
 * SevenSeg.h
 *
 * Created: 3/20/2020 5:40:37 AM
 *  Author: Mahmo
 */ 


#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif //F_CPU

#include "Gpio.h"
#include <util/delay.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

//display value pattern

#ifndef SSD_STARTING_CTRL_BIT
#define SSD_STARTING_CTRL_BIT 0
#endif //SSD_STARTING_CTRL_BIT

#define MAX_POSSIBLE_VAL 10
#define MUX_NUMBER_OF_DISPS 4

typedef enum{
	SEVENSEG_ERR_NO,
	SEVENSEG_ERR_INV_PARAMS,
}SevenSegRetT;

typedef enum{
	DISP_COMMON_ANODE,
	DISP_COMMON_CATHODE,
}SevenSegTypeT;

typedef struct{
	GpioBaseT		DataBase;
	GpioBaseT		CtrlBase;
	SevenSegTypeT	Type;
}SevenSegT;


//normal method one disp per port
SevenSegRetT DisplayInit(SevenSegT* Display,GpioBaseT DispPort,SevenSegTypeT DispType);
SevenSegRetT DisplayWrite(SevenSegT* Display,uint8_t OutVal);
SevenSegRetT DisplayReset(SevenSegT* Display);
SevenSegRetT DisplayDeInit(SevenSegT* Display);
//multiplexed displays
SevenSegRetT DisplayMuxInit(SevenSegT* Display,GpioBaseT DataPort,GpioBaseT CtrlPort,SevenSegTypeT DispType);
SevenSegRetT DisplayMuxWrite(SevenSegT* Display,uint8_t OutDataVal[]);
SevenSegRetT DisplayMuxReset(SevenSegT* Display);
SevenSegRetT DisplayMuxDeInit(SevenSegT* Display);

#if 0
"Why Multiplexing?"
Often we need to use two, three or more SSDs and that too using only a single MCU, but one problem that we face is the lack of I/O pins in the MCU, as one SSD would take 8 pins, and so three SSDs would take 24 pins. In ATmega8, we have only 23 I/O pins. So what is the solution?
One possibility is that we use a bigger MCU with more I/O pins, like ATmega32, which has 26 I/O pins. But then we are still restricted to only a maximum of 3 SSDs that can be used.
Another much better and recommended solution to this problem is to multiplex the Seven Segment Displays.
"What is Multiplexing?"
Wikipedia says ‘ In telecommunications and computer networks, multiplexing (also known as muxing) is a method by which multiple analog message signals or digital data streams are combined into one signal over a shared medium. The aim is to share an expensive resource.’
What we mean by multiplexing of seven-segment display is that we will be using only 7 output ports to give the display on all of the SSDs.
How to achieve this?
Here, we will use ‘Persistence of Vision‘. Now you must have across this term already before. Yes, this is the same technique which is used in cinematography (display images so fast that our brain cannot distinguish any lag between two consecutive images). Similarly, when we mux more than one SSD, we display only one SSD at a time, and we switch between them so fast that our brain cannot distinguish between them.
Lets say each display is active for only 5 milliseconds at a time, i.e. it gets lighted up 1/0.0045 times a seconds, that’s roughly equal to 222 times/second. Our eyes cannot sense a change so fast, and thus what we see is that all the displays are working simultaneously. What is actually happening in the hardware is that the MCU gives ‘1’ to the pin (remember, giving ‘1’ to the base of a BJT shorts the Collector and emitter junction?), which is connected to the base of the transistor of the respective displays, keeps the port ‘ON’ for 5 milliseconds, and then turns it off again. This procedure is put in an endless loop, so that we see the display continuously.
So now we are clear with what actually is multiplexing and how it works! But how to achieve this? There are two methods:
Using timers and delay loops
Using ( _delay_ ) commands
We will here use the 2nd option because it is simpler to understand. If you wish to use to use the first option, then I would suggest you to read Mayank’s awesome timer tutorials.
#endif //0



#endif /* SEVENSEG_H_ */