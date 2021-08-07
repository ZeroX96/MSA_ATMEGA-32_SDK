/*
 * AppGateWay.c
 *
 * Created: 7/6/2021 7:44:23 PM
 *  Author: ZeroX
 */ 
#include "AppGateWay.h"

#if (USE_STATIC_ALLOCATION == TRUE)
static CommT* GatewayQueue[GATEWAY_QUEUE_SIZE] = {NULL};
#endif //USE_STATIC_ALLOCATION

#if (USE_DYNAMIC_ALLOCATION == TRUE)
static CommT* GatewayQueue = calloc(GATEWAY_QUEUE_SIZE, sizeof(CommT*));
#endif //USE_STATIC_ALLOCATION

//here in the atmega32 there are three communication channels,
//    so edit according to your system
#warning "edit this part of the code, on this mcu, i have only three channels"
static CommT* SpiDataHandler = NULL;
static CommT* TwiDataHandler = NULL;
static CommT* UsartDataHandler = NULL;

//internals to handle the hardware
static UsartT UsartChannelHandler;
static SpiT   SpiChannelHandler;
static TwiT   TwiChannelHandler;

//internals to handle callbacks from the interrupts
static void UsartCallbackHandler(void);
static void SpiCallbackHandler(void);
static void TwiCallbackHandler(void);

//internal queues handle the requests
static CommT  UsartRequestsHandler;
static CommT  SpiRequestsHandler;
static CommT  TwiRequestsHandler;
//called before any other function 
CommRetT CommInit(void){
    /*--initialize the communication channels--*/
    
    //initialize the USART
    UsartInit(&UsartChannelHandler, A_SENDER, ONE_STP_BIT, EVEN_PARITY, 
               EIGHT_DATA_BITS, INTERRUPT_DRIVEN, Rising_EDGE);
    //define the usart callback
    UsartSetIsrTxcCb(&UsartChannelHandler, UsartCallbackHandler);
    //initialize the SPI
    SpiInit(&SpiChannelHandler, SPI_1_base, FREQ_BY_4, MASTER_EN, 
              SPI_INTERRUPTING, MODE_2, MSB_FIRST);
    //define the spi callback
    SpiSetIsrCallback(&SpiChannelHandler,SpiCallbackHandler);
    //initialize the TWI
    TwiChannelHandler.TwiBase = TWI_0_BASE;
    TwiChannelHandler.TwiBitRate = 0x50;
    TwiChannelHandler.TwiCallBkFunc = TwiCallbackHandler;
    TwiChannelHandler.TwiGenCallRecognition = TWI_GEN_CALL_RECOGNIZED;
    TwiChannelHandler.TwiMySlaveAdd = 0x35;
    TwiChannelHandler.TwiPrescaller = TWI_PRESCALLER_4;
    TwiChannelHandler.TwiTargetedSlaveAdd = 0x20; //Todo: Let the user define it
    TwiChannelHandler.TwiTWAR_Offset = TWAR_0_OFFSET;
    TwiChannelHandler.TwiTWBR_Offset = TWBR_0_OFFSET;
    TwiChannelHandler.TwiTWCR_Offset = TWCR_0_OFFSET;
    TwiChannelHandler.TwiTWDR_Offset = TWDR_0_OFFSET,
    TwiChannelHandler.TwiTWSR_Offset = TWSR_0_OFFSET;
    TwiInit(&TwiChannelHandler);
    
    /*--initialize the Internal QueueT--*/
    GatewayQueueCreate(&UsartRequestsHandler);
    GatewayQueueCreate(&SpiRequestsHandler);
    GatewayQueueCreate(&TwiRequestsHandler);
}
CommRetT CommSend(CommT* Comm);