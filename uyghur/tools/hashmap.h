// hashmap

#ifndef H_PCT_HASHMAP
#define H_PCT_HASHMAP

#define HASHMAP_DEFAULT_CAPACITY 4097

typedef struct EntryNode{
    char *key;
    void *value;
    struct EntryNode *next;
} Entry;

typedef struct _Hashmap {
    struct _Object;
    int size;
    Entry *position;
} Hashmap;

Hashmap *Hashmap_new();
void* Hashmap_set(Hashmap *, char *, void *);
void* Hashmap_get(Hashmap *, char *);
void Hashmap_del(Hashmap *, char *);
void Hashmap_print(Hashmap *);
void Hashmap_free(Hashmap *);


static int _hashmap_hash_code(char *key) {
    register unsigned long hash = 0;
    unsigned long ch;
    while (ch = (unsigned long)*key++) {
        hash = hash * 131 + ch;
    }
    return hash % HASHMAP_DEFAULT_CAPACITY;
}

Entry *_hashmap_new_entry(char *key, char *value)
{
    Entry *pnode = (Entry *)pct_mallloc(sizeof(Entry));
    pnode->key = key;
    Object_retain(value);
    pnode->value = value;
    pnode->next = NULL;
    return pnode;
}

Hashmap* Hashmap_new() {
    Hashmap *map = (Hashmap *)pct_mallloc(sizeof(Hashmap) * HASHMAP_DEFAULT_CAPACITY);
    Object_init(map, PCT_OBJ_HASHMAP);
    map->size = HASHMAP_DEFAULT_CAPACITY;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i ) {
         map[i].position = NULL;
    }
    return map;
}

void Hashmap_free(Hashmap *this) {
    Entry *ptr;
    Entry *head;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = this[i].position;
        while (ptr != NULL) {
            head = ptr;
            ptr = ptr->next;
            if (head->value != NULL) Object_release(head->value);
            pct_free(head);
        }
    }
    Object_free(this);
}

void *Hashmap_setWithHash(Hashmap *this, char *key, void *value, int hashValue) {
    assert(this != NULL);
    int pos = hashValue;
    void *tmp = NULL;
    Entry *ptr = this[pos].position;
    if (ptr == NULL) {
        this[pos].position = _hashmap_new_entry(key, value);
        return NULL;
    } else {
        while (ptr != NULL) {
            if (!strcmp(key, ptr->key)) {
                tmp = ptr->value;
                Object_retain(value);
                ptr->value = value;
                return tmp;
            }
            ptr = ptr->next;
        }
        Entry *pnode = _hashmap_new_entry(key, value);
        pnode->next = this[pos].position;
        this[pos].position = pnode;
        return NULL;
    }
}

void *Hashmap_set(Hashmap *this, char *key, void *value) {
    int hashValue = _hashmap_hash_code(key);
    return Hashmap_setWithHash(this, key, value, hashValue);
}

void *Hashmap_getWithHash(Hashmap *this, char *key, int hashValue) {
    assert(this != NULL);
    int pos = hashValue;
    Entry *ptr = this[pos].position;
    while (ptr != NULL) {
        if (!strcmp(key, ptr->key)) {
            return ptr->value;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void *Hashmap_get(Hashmap *this, char *key) {
    int hashValue = _hashmap_hash_code(key);
    return Hashmap_getWithHash(this, key, hashValue);
}

void Hashmap_del(Hashmap *this, char *key) {
    assert(this != NULL);
    int pos = _hashmap_hash_code(key);
    Entry *ptr = this[pos].position;
    Entry *pre = ptr;
    if (ptr->next == NULL) {
        if (ptr->value != NULL) Object_release(ptr->value);
        pct_free(ptr);
        this[pos].position = NULL;
        return;
    }
    while (ptr != NULL) {
        if (!strcmp(key, ptr->key)) {
            pre->next = ptr->next;
            if (ptr->value != NULL) Object_release(ptr->value);
            pct_free(ptr);
            return;
        }
        pre = ptr;
        ptr = ptr->next;
    }
}

Cursor *Hashmap_reset(Hashmap *this)
{
    return Cursor_new(NULL);
}

char *Hashmap_next(Hashmap *this, Cursor *cursor)
{
    void *checkItem = Cursor_get(cursor);
    bool emptyCursor = checkItem == NULL;
    bool returnNext = emptyCursor == true;
    if (checkItem) returnNext = true;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; i++) {
        if (this[i].position != NULL) {
            Entry *ptr = this[i].position;
            while (ptr != NULL) {
                if (returnNext)
                {
                    if (emptyCursor)
                    {
                        Cursor_set(cursor, ptr);
                    }
                    return ptr->key;
                }
                else if (checkItem == ptr)
                {
                    returnNext = true;
                }
                ptr = ptr->next;
            }
        }
    } 
    return NULL;
}

void Hashmap_foreach(Hashmap *this, void *bindObj, void (*func)(void *, char *, void *))
{
    int i;
    for (i = 0; i < HASHMAP_DEFAULT_CAPACITY; i++) {
        if (this[i].position != NULL) {
            Entry *ptr = this[i].position;
            while (ptr != NULL) {
                func(bindObj, ptr->key, ptr->value);
                ptr = ptr->next;
            }
        }
    }  
}

void print_map_item(void *obj, char *key, void *data)
{
    printf(" %s -> %p\n", key, (void*)&data); 
}

void Hashmap_print(Hashmap *this) {
    int i;
    printf("[Hashmap_start] address:%p capacity:%d\n", (void*)&this, HASHMAP_DEFAULT_CAPACITY);
    Hashmap_foreach(this, NULL, print_map_item);
    printf("[Hashmap_end] address:%p", (void*)&this);
}

#endif
