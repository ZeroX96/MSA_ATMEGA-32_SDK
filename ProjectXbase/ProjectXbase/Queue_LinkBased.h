#ifndef __QUEUE_LINKBASED__
#define __QUEUE_LINKBASED__
#include <stdint.h>
#include "AppGateWayCfg.h"
/*
*   QLB_ >> Queue Linked Based
*   Mahmoud Saad @ZeroX86
*/
#define MAX_QUEUE_LENGTH GATEWAY_QUEUE_SIZE //NOT IMPORTANT IN THE LINKED BASED

typedef CommT QueueEntry;//TODO: visible here or not?

typedef enum
{
    QLB_ERROR_FOUND,
    QLB_NO_ERRORS,
    QLB_INVALID_ARG,
    QLB_EMPTY,
    QLB_NOT_EMPTY,
    QLB_FULL,
    QLB_NOT_FULL,
    QLB_NOT_INITIALIZED,
    QLB_ERROR_CNT_MALLOC,

}QLB_return_t;

typedef struct qlb_node
{
    QueueEntry node_entry;
    struct qlb_node *node_next;
}QLB_node;

typedef struct{
    QLB_node *QLB_front;
    QLB_node *QLB_rear;
    uint32_t  QLB_size;
    bool      QLB_InitStatus;
}Queue_LinkBased;


QLB_return_t QLB_create     (Queue_LinkBased *ptr_queue);
QLB_return_t QLB_empty      (Queue_LinkBased *ptr_queue);
QLB_return_t QLB_full       (Queue_LinkBased *ptr_queue);
uint32_t     QLB_size       (Queue_LinkBased *ptr_queue);
QLB_return_t QLB_clear      (Queue_LinkBased *ptr_queue);
QLB_return_t QLB_append     (Queue_LinkBased *ptr_queue,QueueEntry *ptr_entry);
QLB_return_t QLB_serve      (Queue_LinkBased *ptr_queue,QueueEntry *ptr_entry);
QLB_return_t QLB_retrive    (Queue_LinkBased *ptr_queue,QueueEntry *ptr_entry);
QLB_return_t QLB_traverse   (Queue_LinkBased *ptr_queue,void(*ptr_func)(QueueEntry));
#endif //__QUEUE_LINKBASED__

