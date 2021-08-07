/*
 * AppGateWayCfg.h
 *
 * Created: 7/7/2021 1:14:50 PM
 *  Author: ZeroX
 */ 


#ifndef APPGATEWAYCFG_H_
#define APPGATEWAYCFG_H_

#include <stdint.h>
#include <stdbool.h>
#include "AppGateWayDataTypes.h"

#define GATEWAY_QUEUE_SIZE (10U)

//decide whither to use static allocation or dynamic memory allocation
#define USE_STATIC_ALLOCATION   (1U)
#define USE_DYNAMIC_ALLOCATION  (0U)

//decide which module to be operating to save some code size
#define ENABLE_USART_MODULE (1U)
#define ENABLE_TWI_MODULE   (1U)
#define ENABLE_SPI_MODULE   (1U)


#ifndef NULL
#define NULL ((void*)0)
#endif //NULL

#ifndef TRUE
#define TRUE		(1U)
#endif //TRUE

#ifndef FALSE
#define FALSE		(0U)
#endif //FALSE


#endif /* APPGATEWAYCFG_H_ */