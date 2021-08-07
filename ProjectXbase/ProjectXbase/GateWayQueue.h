/*
 * GateWayQueue.h
 *
 * Created: 7/7/2021 6:31:03 AM
 *  Author: ZeroX
 */ 


#ifndef GATEWAYQUEUE_H_
#define GATEWAYQUEUE_H_
#include "AppGateWayCfg.h"
#include "AppGateWay.h"
#include "Queue_ArrBased.h"
#include "Queue_LinkBased.h"



#define MAX_QUEUE_LENGTH GATEWAY_QUEUE_SIZE //NOT IMPORTANT IN THE LINKED BASED

#if (USE_DYNAMIC_ALLOCATION == TRUE)

typedef Queue_LinkBased GatewayQueueT;
typedef QLB_return_t    GatewayQueueRetT;

#endif //USE_DYNAMIC_ALLOCATION

#if (USE_STATIC_ALLOCATION == TRUE)

typedef Queue_ArrBased GatewayQueueT;
typedef QAB_return_t   GatewayQueueRetT;

#endif //USE_STATIC_ALLOCATION

GatewayQueueRetT GatewayQueueCreate  (GatewayQueueT* ptr_queue);
GatewayQueueRetT GatewayQueueEmpty   (GatewayQueueT* ptr_queue);
GatewayQueueRetT GatewayQueueFull    (GatewayQueueT* ptr_queue);
uint32_t         GatewayQueueSize    (GatewayQueueT* ptr_queue);
GatewayQueueRetT GatewayQueueAppend  (GatewayQueueT* ptr_queue,QueueEntry* ptr_entry);
GatewayQueueRetT GatewayQueueServe   (GatewayQueueT* ptr_queue,QueueEntry* ptr_entry);
GatewayQueueRetT GatewayQueueRetrive (GatewayQueueT* ptr_queue,QueueEntry* ptr_entry);
GatewayQueueRetT GatewayQueueClear   (GatewayQueueT* ptr_queue);
GatewayQueueRetT GatewayQueueTraverse(GatewayQueueT* ptr_queue,void(*ptr_func)(QueueEntry));

#endif /* GATEWAYQUEUE_H_ */