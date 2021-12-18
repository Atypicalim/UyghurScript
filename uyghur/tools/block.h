// token

#ifndef H_BLOCK
#define H_BLOCK

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct _Block {
    void *data;
    void *next;
    void *last;
} Block;

void Block_init(void *_block, void *data)
{
    Block *block = _block;
    block->data = data;
    block->next = NULL;
    block->last = NULL;
}

Block *Block_new(void *data)
{
    Block *block = (Block *)malloc(sizeof(Block));
    Block_init(block, data);
    return block;
}

void Block_print(void *_this)
{
    if (_this == NULL)
    {
        printf("[(Block) => NULL]\n");
    }
    else
    {
        Block *this = _this;
        printf("[(Block) => address:%d, data:%d]\n", this, (char *)this->data);
    }
}

void Block_link(void *_first, void *_second)
{
    Block *first = _first;
    Block *second = _second;
    first->next = second;
    second->last = first;
}

void Block_append(void *_this, void *_other)
{
    Block_link(_this, _other);
}

void Block_prepend(void *_this, void *_other)
{
    Block_link(_other, _this);
}

void Block_remove(void *_this)
{
    Block *this = _this;
        Block *next = this->next;
        Block *last = this->last;
    if (next != NULL && last != NULL)
    {
        last->next = next;
        next->last = last;
    }
    else if (next != NULL)
    {
        next->last = NULL;
    }
    else if (last != NULL)
    {
        last->next = NULL;
    }
    this->next = NULL;
    this->last = NULL;
}

void *Block_next(void *_this)
{
    Block *this = _this;
    return this->next;
}

void *Block_last(void *_this)
{
    Block *this = _this;
    return this->last;
}

void Block_free(void *_this)
{
    Block *this = _this;
    Block *tmpNext = this->next;
    Block *tmpLast =this->last;
    if (tmpNext != NULL) tmpNext->last = NULL;
    if (tmpLast != NULL) tmpLast->next = NULL;
    this->next = NULL;
    this->last = NULL;
    tmpNext = NULL;
    tmpLast = NULL;
    if (this->data != NULL)
    {
        free(this->data);
        this->data = NULL;
    }
    free(this);
}

#endif
