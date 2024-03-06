// queue

#ifndef H_PCT_QUEUE
#define H_PCT_QUEUE

#include "block.h"

typedef struct _Queue {
    struct _Object;
    int size;
    Block *head;
    Block *tail;
    Block *cursor;
} Queue;

Queue *Queue_new()
{
    Queue *queue = (Queue *)pct_mallloc(sizeof(Queue));
    Object_init(queue, PCT_OBJ_QUEUE);
    queue->size = 0;
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
    this->size++;
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
        this->size = 0;
        return NULL;
    }
    else
    {
        void *data = this->head->data;
        Block *head = this->head;
        if (this->head == this->tail)
        {
            this->size = 0;
            this->head = NULL;
            this->tail = NULL;
        }
        else
        {
            this->size--;
            this->head = this->head->next;
            this->head->last = NULL;
        }
        Object_release(head);
        return data;
    }
}

void Queue_clear(Queue *this)
{
    void *data = Queue_pop(this);
    while (data != NULL)
    {
        data = Queue_pop(this);
    }
}

void Queue_free(Queue *this)
{
    Block *head = this->head;
    while (head != NULL)
    {
        this->head = head->next;
        Object_release(head);
        head = this->head;
    }
    Object_free(this);
}

Cursor *Queue_reset(Queue *this)
{
    return Cursor_new(this->head);
}

void *Queue_next(Queue *this, Cursor *cursor)
{
    Block *temp = Cursor_get(cursor);
    if (temp == NULL) return NULL;
    Cursor_set(cursor, temp->next);
    return temp->data;
}

#endif
