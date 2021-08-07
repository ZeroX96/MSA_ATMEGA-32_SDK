
#include "Queue_ArrBased.h"


QAB_return_t QAB_queue_create(Queue_ArrBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        ptr_queue->QAB_InitStatus=TRUE;
        ptr_queue->Queue_ArrBased_front=0;
        ptr_queue->Queue_ArrBased_rear=-1;
        ptr_queue->Queue_ArrBased_size=0;
        return QAB_NO_ERRORS;
    }
    return QAB_INVALID_ARG;
}
QAB_return_t QAB_queue_empty(Queue_ArrBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if((ptr_queue->Queue_ArrBased_size) == 0)
        {
            return QAB_QUEUE_EMPTY;
        }
        else
        {
            return QAB_QUEUE_NOT_EMPTY;
        }
    }
    return QAB_INVALID_ARG;

}
QAB_return_t QAB_queue_full(Queue_ArrBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if((ptr_queue->Queue_ArrBased_size) == MAX_QUEUE_LENGTH)
        {
            return QAB_QUEUE_FULL;
        }
        else
        {
            return QAB_QUEUE_NOT_FULL;
        }
    }
    return QAB_INVALID_ARG;

}
uint32_t QAB_queue_size(Queue_ArrBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        return ptr_queue->Queue_ArrBased_size;
    }
    return QAB_INVALID_ARG;
}
QAB_return_t QAB_queue_append(Queue_ArrBased *ptr_queue,QueueEntry *ptr_entry)
{
    if((ptr_queue != NULL) && (ptr_entry != NULL))
    {
        if(ptr_queue->QAB_InitStatus == TRUE)
        {
            if(ptr_queue->Queue_ArrBased_rear== MAX_QUEUE_LENGTH-1)
                ptr_queue->Queue_ArrBased_rear=0;
            else
                ptr_queue->Queue_ArrBased_rear++;

            ptr_queue->entry_arr[ptr_queue->Queue_ArrBased_rear]=*ptr_entry;
            ptr_queue->Queue_ArrBased_size++;
            return QAB_NO_ERRORS;
        }
        return QAB_QUEUE_NOT_INITIALIZED;
    }
    return QAB_INVALID_ARG;
}
QAB_return_t QAB_queue_serve(Queue_ArrBased *ptr_queue,QueueEntry *ptr_entry)
{
    if((ptr_queue != NULL) && (ptr_entry != NULL))
    {
        if(ptr_queue->QAB_InitStatus == TRUE)
        {
           *ptr_entry = ptr_queue->entry_arr[ptr_queue->Queue_ArrBased_front];
            ptr_queue->Queue_ArrBased_size--;
            if(ptr_queue->Queue_ArrBased_front== MAX_QUEUE_LENGTH-1)
                {ptr_queue->Queue_ArrBased_front=0;}
            else
                {ptr_queue->Queue_ArrBased_front++;}
            return QAB_NO_ERRORS;
        }
        return QAB_QUEUE_NOT_INITIALIZED;
    }
    return QAB_INVALID_ARG;
}//01113670061 faid
QAB_return_t QAB_queue_retrive(Queue_ArrBased *ptr_queue,QueueEntry *ptr_entry)
{
    if((ptr_queue != NULL) && (ptr_entry != NULL))
    {
        if(ptr_queue->QAB_InitStatus == TRUE)
        {
           *ptr_entry = ptr_queue->entry_arr[ptr_queue->Queue_ArrBased_front];
            return QAB_NO_ERRORS;
        }
        return QAB_QUEUE_NOT_INITIALIZED;
    }
    return QAB_INVALID_ARG;
}
QAB_return_t QAB_queue_clear(Queue_ArrBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if(ptr_queue->QAB_InitStatus == TRUE)
        {
            ptr_queue->Queue_ArrBased_size=0;
            ptr_queue->Queue_ArrBased_front=0;
            ptr_queue->Queue_ArrBased_rear=-1;
            ptr_queue->QAB_InitStatus=FALSE;
            return QAB_NO_ERRORS;
        }
        return QAB_QUEUE_NOT_INITIALIZED;
    }
    return QAB_INVALID_ARG;
}
QAB_return_t QAB_queue_traverse(Queue_ArrBased *ptr_queue,void(*ptr_func)(QueueEntry))
{
    if((ptr_queue != NULL) && (ptr_func != NULL))
    {
        if(ptr_queue->QAB_InitStatus == TRUE)
        {
            uint32_t i=0;
            for(i=ptr_queue->Queue_ArrBased_front;i != ptr_queue->Queue_ArrBased_rear;)
            {
                (*ptr_func)(ptr_queue->entry_arr[i]);
                if(i== MAX_QUEUE_LENGTH-1)
                {i=0;}
                else
                {i++;}
            }
            return QAB_NO_ERRORS;
        }
        return QAB_QUEUE_NOT_INITIALIZED;
    }
    return QAB_INVALID_ARG;

}
