// token

#ifndef H_BLOCK
#define H_BLOCK

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct _Block {
    void *next;
    void *last;
} Block;

Block *Block_new(void *obj)
{
    Block *block = obj != NULL ? (Block *)obj : (Block *)malloc(sizeof(Block));
    block->next = NULL;
    block->last = NULL;
    return block;
}

void Block_print(void *_this)
{
    Block *this = _this;
    printf("[(Block) => address:%d]\n", this);
}

void Block_append(void *_this, void *_other)
{
    Block *this = _this;
    Block *other = _other;
    this->next = other;
    other->last = this;
}

void Block_prepend(void *_this, void *_other)
{
    Block *this = _this;
    Block *other = _other;
    this->last = other;
    other->next = this;
}

void Block_link(void *_first, void *_second)
{
    Block *first = _first;
    Block *second = _second;
    first->next = second;
    second->last = first;
}

void Block_remove(void *_this)
{
    Block *this = _this;
    if ((this->next == this->last && this->next != this) || this->next != this->last)
    {
        Block *next = this->next;
        Block *last = this->last;
        last->next = this->next;
        next->last = this->last;
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
    free(this);
}

#endif
