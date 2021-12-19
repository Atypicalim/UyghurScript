// cursor

#ifndef H_CURSOR
#define H_CURSOR

typedef struct _Cursor {
    void *cursor;
} Cursor;

Cursor *Cursor_new(void *cursor)
{
    Cursor *queue = (Cursor *)malloc(sizeof(Cursor));
    queue->cursor = cursor;
    return queue;
}

void Cursor_set(Cursor *this, void *cursor)
{
    this->cursor = cursor;
}

void *Cursor_get(Cursor *this)
{
    return this->cursor;
}

void Cursor_print(Cursor *this)
{
    printf("[(CURSOR) => p:%s, c:%s]\n", this, this->cursor);
}

void Cursor_free(Cursor *this)
{
    this->cursor = NULL;
    free(this);
}

#endif
