/*
 * AppGateWay.h
 *
 * Created: 7/6/2021 7:45:14 PM
 *  Author: ZeroX
 */ 


#ifndef APPGATEWAY_H_
#define APPGATEWAY_H_

//include the stdlib for dynamic memory allocation
#include <stdlib.h>
#include <stdbool.h>
#include "AppGateWayDataTypes.h"
#include "AppGateWayCfg.h"
//Including Channels
#include "Usart.h"
#include "Twi.h"
#include "Spi.h"
//include the queue
#include "GateWayQueue.h"


//Comm-APIs
CommRetT CommInit(void);
CommRetT CommSend(CommT* Comm);
CommRetT CommRecieve();
CommRetT CommDisp(void);


/*
cts and rts pins in the uart?
pointer to the data not the data
i will need to port the interrupts and their service routines
ZeroWayInit
-channel select or let the comm decide
--channel = SPI? I2C? UART? then CAN? LIN?
-location in mem
-size
-target id and the comm handle multiple connection
-callback
--state
--handle multiple tasks send 4k, send 2k, send n-k linked list of tasks
--frame of the comm / comm header 
---frame = size - command - endianess - data
-dispatcher? or inside interrupts?  -=dispatcher
*/
#endif /* APPGATEWAY_H_ */