/*
 * AppGateWayDataTypes.h
 *
 * Created: 7/7/2021 10:42:49 PM
 *  Author: ZeroX
 */ 


#ifndef APPGATEWAYDATATYPES_H_
#define APPGATEWAYDATATYPES_H_
#include <stdint.h>
#define COMM_ERROR_BASE (0x70U)

typedef enum{
    COMM_ERR_NO = COMM_ERROR_BASE,
    COMM_ERR_INV_PARAMS,
    COMM_ERR_BUSY
}CommRetT;

typedef enum{
    CHANNEL_AUTO,
    CHANNEL_USART,
    CHANNEL_TWI,
    CHANNEL_SPI,
}CommChannelT;

typedef enum{
    DATA_LITTLE_ENDIAN,
    DATA_BIG_ENDIAN
}CommEndianessT;

typedef struct{
    const void*    ObjDataPtr;
    CommChannelT   ObjChannel;
    CommEndianessT ObjDataEndianess;
    uint16_t       ObjDataBytesNumb;
}CommT;



#endif /* APPGATEWAYDATATYPES_H_ */