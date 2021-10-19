// queue

#ifndef H_QUEUE
#define H_QUEUE

#include "block.h"

typedef struct _Queue {
    Block *head;
    Block *tail;
    Block *cursor;
} Queue;

Queue *Queue_new()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->cursor = NULL;
    return queue;
}

void Queue_print(Queue *this)
{
    printf("[(QUEUE_START) => address:%d]\n", this);
    Block *current = this->head;
    while (current != NULL)
    {
        Block_print(current);
        current = current->next;
    }
    printf("[(QUEUE_END) => address:%d]\n", this);
}

void Queue_push(Queue *this, void *data)
{
    Block *block = Block_new(data);
    if (this->head == NULL)
    {
        this->head = block;
    }
    else
    {
        Block_append(this->tail, block);
    }
    this->tail = block;
}

void *Queue_pop(Queue *this)
{
    if (this->head == NULL)
    {
        return NULL;
    }
    else
    {
        void *data = this->head->data;
        Block *head = this->head;
        if (this->head == this->tail)
        {
            this->head = NULL;
            this->tail = NULL;
        }
        else
        {
            this->head = this->head->next;
            this->head->last = NULL;
        }
        head->data = NULL;
        Block_free(head);
        return data;
    }
}

void Queue_clear(Queue *this)
{
    this->head = NULL;
    this->tail = NULL;
    this->cursor = NULL;
}

void Queue_free(Queue *this)
{
    Block *head = this->head;
    while (head != NULL)
    {
        this->head = head->next;
        Block_free(head);
        head = this->head;
    }
    free(this);
}

void *Queue_next(Queue *this)
{
    Block *temp = this->cursor;
    if (temp == NULL)
    {
        return NULL;
    }
    else
    {
        this->cursor = temp->next;
        return temp->data;
    }

}

void Queue_reset(Queue *this)
{
    this->cursor = this->head;
}

#endif
