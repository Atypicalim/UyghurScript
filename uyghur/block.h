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
    Block *tmp;
    if (this->next != NULL)
    {
        tmp = this->next;
        tmp->last = NULL;
    }
    if (this->last != NULL)
    {
        tmp = this->last;
        tmp->next = NULL;
    }
    tmp = NULL;
    this->next = NULL;
    this->last = NULL;
    free(this);
}

#endif
