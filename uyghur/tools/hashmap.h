// hashmap

#ifndef H_PCT_HASHMAP
#define H_PCT_HASHMAP

#define HASHMAP_DEFAULT_CAPACITY 4097

typedef struct _Hashmap {
    struct _Object;
    int size;
    Entry *position;
} Hashmap;

Hashmap* Hashmap_new() {
    Hashmap *map = (Hashmap *)pct_mallloc(sizeof(Hashmap) * HASHMAP_DEFAULT_CAPACITY);
    map->size = HASHMAP_DEFAULT_CAPACITY;
    for (int i = 0; i < map->size; ++i ) map[i].position = NULL;
    Object_init(map, PCT_OBJ_HASHMAP);
    return map;
}

// TODO: release removed value
void Hashmap_free(Hashmap *this) {
    Entry *ptr;
    Entry *head;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = this[i].position;
        while (ptr != NULL) {
            head = ptr;
            ptr = ptr->next;
            Object_release(head);
        }
    }
    Object_free(this);
}

void *Hashmap_set(Hashmap *this, char *_key, void *value) {
    assert(this != NULL);
    String *key = String_format(_key);
    int pos = String_hash(key) % HASHMAP_DEFAULT_CAPACITY;
    //
    void *tmp = NULL;
    Entry *ptr = this[pos].position;
    if (ptr == NULL) {
        this[pos].position = Entry_new(key, value);
        Object_release(key);
        return NULL;
    }
    while (ptr != NULL) {
        if (String_equal(key, ptr->key)) {
            tmp = ptr->value;
            Entry_set(ptr, key, value);
            Object_release(key);
            return tmp;
        }
        ptr = ptr->next;
    }
    Entry *pnode = Entry_new(key, value);
    Object_release(key);
    pnode->next = this[pos].position;
    this[pos].position = pnode;
    return NULL;
}

void *Hashmap_get(Hashmap *this, char *_key) {
    assert(this != NULL);
    String *key = String_format(_key);
    int pos = String_hash(key) % HASHMAP_DEFAULT_CAPACITY;
    //
    Entry *ptr = this[pos].position;
    while (ptr != NULL) {
        if (String_equal(key, ptr->key)) {
            Object_release(key);
            return ptr->value;
        }
        ptr = ptr->next;
    }
    Object_release(key);
    return NULL;
}

// TODO: release removed value
void Hashmap_del(Hashmap *this, char *_key) {
    assert(this != NULL);
    String *key = String_format(_key);
    int pos = String_hash(key) % HASHMAP_DEFAULT_CAPACITY;
    //
    Entry *ptr = this[pos].position;
    if (ptr == NULL) {
        //
    } else if (ptr->next == NULL && String_equal(key, ptr->key)) {
        this[pos].position = NULL;
        Object_release(ptr);
    } else {
        Entry *pre = ptr;
        ptr = pre->next;
        while (ptr != NULL) {
            if (String_equal(key, ptr->key)) {
                pre->next = ptr->next;
                Object_release(ptr);
                break;
            }
            pre = ptr;
            ptr = ptr->next;
        }
    }
    Object_release(key);
}

#endif
