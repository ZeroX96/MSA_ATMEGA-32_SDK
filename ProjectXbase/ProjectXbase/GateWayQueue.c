/*
 * GateWayQueue.c
 *
 * Created: 7/7/2021 6:30:33 AM
 *  Author: ZeroX
 */ 
#include "GateWayQueue.h"

GatewayQueueRetT GatewayQueueCreate (GatewayQueueT* ptr_queue){
    //check the arguments
    if(ptr_queue == NULL)
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_create(ptr_queue);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_create(ptr_queue);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}

GatewayQueueRetT GatewayQueueEmpty (GatewayQueueT* ptr_queue){
    //check the arguments
    if(ptr_queue == NULL)
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_empty(ptr_queue);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_empty(ptr_queue);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
GatewayQueueRetT GatewayQueueFull (GatewayQueueT* ptr_queue){
    //check the arguments
    if(ptr_queue == NULL)
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_full(ptr_queue);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_full(ptr_queue);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
uint32_t GatewayQueueSize (GatewayQueueT* ptr_queue){
    //check the arguments
    if(ptr_queue == NULL)
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_size(ptr_queue);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_size(ptr_queue);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
GatewayQueueRetT GatewayQueueAppend  (GatewayQueueT* ptr_queue,QueueEntry* ptr_entry){
    //check the arguments
    if((ptr_queue == NULL) || (ptr_entry == NULL))
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_append(ptr_queue,ptr_entry);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_append(ptr_queue,ptr_entry);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
GatewayQueueRetT GatewayQueueServe   (GatewayQueueT* ptr_queue,QueueEntry* ptr_entry){
    //check the arguments
    if((ptr_queue == NULL) || (ptr_entry == NULL))
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_serve(ptr_queue,ptr_entry);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_serve(ptr_queue,ptr_entry);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
GatewayQueueRetT GatewayQueueRetrive (GatewayQueueT* ptr_queue,QueueEntry* ptr_entry){
    //check the arguments
    if((ptr_queue == NULL) || (ptr_entry == NULL))
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_retrive(ptr_queue,ptr_entry);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_retrive(ptr_queue,ptr_entry);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
GatewayQueueRetT GatewayQueueClear (GatewayQueueT* ptr_queue){
    //check the arguments
    if(ptr_queue == NULL)
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_clear(ptr_queue);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_clear(ptr_queue);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}
GatewayQueueRetT GatewayQueueTraverse(GatewayQueueT* ptr_queue,void(*ptr_func)(QueueEntry)){
    //check the arguments
    if((ptr_queue == NULL) || (ptr_func == NULL))
    {
        return QAB_INVALID_ARG;
    }
    
    #if (USE_STATIC_ALLOCATION == TRUE)
    QAB_queue_traverse(ptr_queue,ptr_func);
    #endif //USE_STATIC_ALLOCATION

    #if (USE_DYNAMIC_ALLOCATION == TRUE)
    QLB_traverse(ptr_queue,ptr_func);
    #endif //USE_DYNAMIC_ALLOCATION
    
    return QAB_NO_ERRORS;
}

