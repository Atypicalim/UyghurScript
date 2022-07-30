// cursor

#ifndef H_PCT_CURSOR
#define H_PCT_CURSOR

typedef struct _Cursor {
    struct _Object;
    void *cursor;
} Cursor;

Cursor *Cursor_new(void *cursor)
{
    Cursor *queue = (Cursor *)pct_mallloc(sizeof(Cursor));
    Object_init(queue, PCT_OBJ_CURSOR);
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
    Object_free(this);
}

#endif
