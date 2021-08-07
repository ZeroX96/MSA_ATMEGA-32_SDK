#ifndef __QUEUE_ARRBASED__
#define __QUEUE_ARRBASED__

#include "AppGateWayCfg.h"
/*
*   QAB_ >> Queue Array Based
*   Mahmoud Saad @ZeroX86
*/
#define MAX_QUEUE_LENGTH GATEWAY_QUEUE_SIZE
typedef CommT QueueEntry;//TODO: visible here or not?
typedef enum
{
    QAB_ERROR_FOUND,
    QAB_NO_ERRORS,
    QAB_INVALID_ARG,
    QAB_QUEUE_EMPTY,
    QAB_QUEUE_NOT_EMPTY,
    QAB_QUEUE_FULL,
    QAB_QUEUE_NOT_FULL,
    QAB_QUEUE_NOT_INITIALIZED,

}QAB_return_t;

typedef struct{
    QueueEntry entry_arr[MAX_QUEUE_LENGTH];
    uint32_t    Queue_ArrBased_front;
    uint32_t    Queue_ArrBased_rear;
    uint32_t    Queue_ArrBased_size;
    bool        QAB_InitStatus;//may add an extra variable to define whither initialized or not
}Queue_ArrBased;


QAB_return_t QAB_queue_create(Queue_ArrBased *ptr_queue);
QAB_return_t QAB_queue_empty(Queue_ArrBased *ptr_queue);
QAB_return_t QAB_queue_full(Queue_ArrBased *ptr_queue);
uint32_t     QAB_queue_size(Queue_ArrBased *ptr_queue);
QAB_return_t QAB_queue_append(Queue_ArrBased *ptr_queue,QueueEntry *ptr_entry);
QAB_return_t QAB_queue_serve(Queue_ArrBased *ptr_queue,QueueEntry *ptr_entry);
QAB_return_t QAB_queue_retrive(Queue_ArrBased *ptr_queue,QueueEntry *ptr_entry);
QAB_return_t QAB_queue_clear(Queue_ArrBased *ptr_queue);
QAB_return_t QAB_queue_traverse(Queue_ArrBased *ptr_queue,void(*ptr_func)(QueueEntry));
#endif //__QUEUE_ARRBASED__
