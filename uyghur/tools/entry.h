// entry

#ifndef H_PCT_ENTRY
#define H_PCT_ENTRY

typedef struct EntryNode{
    struct _Object;
    String *key;
    void *value;
    struct EntryNode *next;
} Entry;

Entry *Entry_set(Entry *this, String *key, void *value)
{
    if (key != NULL) {
        if (this->key != NULL) Object_release(this->key);
        Object_retain(key);
        this->key = key;
    }
    if (value != NULL) {
        if (this->value != NULL) Object_release(this->value);
        Object_retain(value);
        this->value = value;
    }
    return this;
}

Entry *Entry_new(String *key, void *value)
{
    Entry *entry = (Entry *)pct_mallloc(sizeof(Entry));
    entry->key = NULL;
    entry->value = NULL;
    entry->next = NULL;
    Entry_set(entry, key, value);
    Object_init(entry, PCT_OBJ_ENTRY);
    return entry;
}

void Entry_free(void *_this)
{
    Entry *this = _this;
    this->next = NULL;
    Object_release(this->value);
    Object_release(this->key);
    Object_free(this);
}

#endif
