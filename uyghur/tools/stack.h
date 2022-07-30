// stack

#ifndef H_PCT_STACK
#define H_PCT_STACK

#include "block.h"

typedef struct _Stack {
    struct _Object;
    Block *head;
    Block *tail;
    Block *cursor;
} Stack;

Stack *Stack_new()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    Object_init(stack, PCT_OBJ_STACK);
    stack->head = NULL;
    stack->tail = NULL;
    stack->cursor = NULL;
    return stack;
}

void Stack_print(Stack *this)
{
    printf("[(STACK_START) => address:%d]\n", this);
    Block *current = this->head;
    while (current != NULL)
    {
        Block_print(current);
        current = current->next;
    }
    printf("[(STACK_END) => address:%d]\n", this);
}

void Stack_push(Stack *this, void *data)
{
    Block *block = Block_new(data);
    Object_retain(block);
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

void *Stack_pop(Stack *this)
{
    if (this->tail == NULL)
    {
        return NULL;
    }
    else
    {
        void *data = this->tail->data;
        Block *tail = this->tail;
        if (this->tail == this->head)
        {
            this->head = NULL;
            this->tail = NULL;
        }
        else
        {
            this->tail = this->tail->last;
            this->tail->next = NULL;
        }
        Object_release(tail);
        return data;
    }
}

void Stack_clear(Stack *this)
{
    void *data = Stack_pop(this);
    while (data != NULL)
    {
        data = Stack_pop(this);
    }
}

void Stack_free(Stack *this)
{
    Block *tail = this->tail;
    while (tail != NULL)
    {
        this->tail = tail->last;
        Object_release(tail);
        tail = this->tail;
    }
    Object_free(this);
}

Cursor *Stack_reset(Stack *this)
{
    return Cursor_new(this->tail);
}

void *Stack_next(Stack *this, Cursor *cursor)
{
    Block *temp = Cursor_get(cursor);
    if (temp == NULL) return NULL;
    Cursor_set(cursor, temp->last);
    return temp->data;
}

void Stack_reverse(Stack *this)
{
    Queue *queue = Queue_new();
    Cursor *cursor1 = Stack_reset(this);
    void *data1 = Stack_next(this, cursor1);
    while (data1 != NULL)
    {
        Queue_push(queue, data1);
        data1 = Stack_next(this, cursor1);
    }
    Stack_clear(this);
    Cursor *cursor2 = Queue_reset(queue);
    void *data = Queue_next(queue, cursor2);
    while (data != NULL)
    {
        Stack_push(this, data);
        data = Queue_next(queue, cursor2);
    }
    Cursor_free(cursor1);
    Cursor_free(cursor2);
    Queue_free(queue);
}

#endif
