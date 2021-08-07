#include "Queue_LinkBased.h"


QLB_return_t QLB_create(Queue_LinkBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        ptr_queue->QLB_front=NULL;
        ptr_queue->QLB_rear=NULL;
        ptr_queue->QLB_InitStatus=TRUE;
        ptr_queue->QLB_size=0;
        return QLB_NO_ERRORS;
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_empty(Queue_LinkBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            if((ptr_queue->QLB_front == NULL) || (ptr_queue->QLB_size == 0))
            {
                return QLB_EMPTY;
            }
            else
            {
                return QLB_NOT_EMPTY;
            }
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_full(Queue_LinkBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            return QLB_NOT_FULL;
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

uint32_t QLB_size(Queue_LinkBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            return ptr_queue->QLB_size;
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_clear(Queue_LinkBased *ptr_queue)
{
    if(ptr_queue != NULL)
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            QLB_node*ptr_temp;
            while((ptr_temp != NULL) && (ptr_queue->QLB_front != NULL))
            {
                ptr_temp=ptr_queue->QLB_front;
                ptr_queue->QLB_front=ptr_temp->node_next;
                free(ptr_temp);
            }
            ptr_queue->QLB_size=0;
            return QLB_NO_ERRORS;
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_append(Queue_LinkBased *ptr_queue,QueueEntry *ptr_entry)
{
    if((ptr_queue != NULL)&&(ptr_entry != NULL))
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            QLB_node*ptr_temp=(QLB_node*)malloc(sizeof(QLB_node));
            if(ptr_temp != NULL)
            {
                ptr_temp->node_next=NULL;
                ptr_temp->node_entry=*ptr_entry;
                if(!ptr_queue->QLB_rear)
                    ptr_queue->QLB_front=ptr_temp;
                else
                    ptr_queue->QLB_rear->node_next=ptr_temp;
                ptr_queue->QLB_rear=ptr_temp;
                ptr_queue->QLB_size++;
                return QLB_NO_ERRORS;
            }
            else
            {
                return QLB_ERROR_CNT_MALLOC;
            }
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_serve(Queue_LinkBased *ptr_queue,QueueEntry *ptr_entry)
{
    if((ptr_queue != NULL)&&(ptr_entry != NULL))
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            QLB_node*ptr_temp=ptr_queue->QLB_front;
            ptr_queue->QLB_front=ptr_temp->node_next;
            *ptr_entry=ptr_temp->node_entry;
            free(ptr_temp);
            if(!ptr_queue->QLB_front)
                ptr_queue->QLB_rear=NULL;
            ptr_queue->QLB_size--;
            return QLB_NO_ERRORS;
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_retrive(Queue_LinkBased *ptr_queue,QueueEntry *ptr_entry)
{
    if((ptr_queue != NULL)&&(ptr_entry != NULL))
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            *ptr_entry=ptr_queue->QLB_front->node_entry;
            return QLB_NO_ERRORS;
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}

QLB_return_t QLB_traverse   (Queue_LinkBased *ptr_queue,void(*ptr_func)(QueueEntry))
{
    if((ptr_queue != NULL)&&(ptr_func != NULL))
    {
        if(ptr_queue->QLB_InitStatus == TRUE)
        {
            QLB_node*ptr_temp;
            for(ptr_temp=ptr_queue->QLB_front;ptr_temp!=NULL;ptr_temp=ptr_temp->node_next)
            {
                (*ptr_func)(ptr_temp->node_entry);
            }
            return QLB_NO_ERRORS;
        }
        else
        {
            return QLB_NOT_INITIALIZED;
        }
    }
    return QLB_INVALID_ARG;
}
