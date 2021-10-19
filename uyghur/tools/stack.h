// stack

#ifndef H_STACK
#define H_STACK

#include "block.h"

typedef struct _Stack {
    Block *head;
    Block *tail;
    Block *cursor;
} Stack;

Stack *Stack_new()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
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
        tail->data = NULL;
        Block_free(tail);
        return data;
    }
}

void Stack_clear(Stack *this)
{
    this->head = NULL;
    this->tail = NULL;
    this->cursor = NULL;
}

void Stack_free(Stack *this)
{
    Block *tail = this->tail;
    while (tail != NULL)
    {
        this->tail = tail->last;
        Block_free(tail);
        tail = this->tail;
    }
    free(this);
}

void *Stack_next(Stack *this)
{
    Block *temp = this->cursor;
    if (temp == NULL)
    {
        return NULL;
    }
    else
    {
        this->cursor = temp->last;
        return temp->data;
    }

}

void Stack_reset(Stack *this)
{
    this->cursor = this->tail;
}

#endif
