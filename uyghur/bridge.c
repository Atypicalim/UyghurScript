// bridge

#include "uyghur.c"

struct Bridge {
    Uyghur *uyghur;
    Hashmap *cache;
};

void Bridge_reset(Bridge *this)
{
    this->cache = Hashmap_new(NULL);
}

Bridge *Bridge_new(Uyghur *uyghur)
{
    Bridge *bridge = malloc(sizeof(Bridge));
    bridge->uyghur = uyghur;
    Bridge_reset(bridge);
    return bridge;
}

void Bridge_pushBoolean(Bridge *this, char *key, bool value)
{
    Hashmap_set(this->cache, key, Value_newBoolean(value, NULL));
}

void Bridge_pushNumber(Bridge *this, char *key, double value)
{
    Hashmap_set(this->cache, key, Value_newNumber(value, NULL));
}

void Bridge_pushString(Bridge *this, char *key, char *value)
{
    Hashmap_set(this->cache, key, Value_newString(value, NULL));
}

void Bridge_pushFunction(Bridge *this, char *key, void (*value)(Queue *))
{
    // Hashmap_set(this->cache, key, Value_newNative(value, NULL));
}

void Bridge_createBox(Bridge *this, char *key)
{
    Hashmap *value = this->cache;
    Bridge_reset(this);
    // Hashmap_set(this->cache, key, Value_newBox(value, NULL));
}

void Bridge_commit(Bridge *this)
{
    Hashmap *scope = this->uyghur->executer->rootScope;
    Hashmap *map = this->cache;
    int size = map->size;
    int i;
    for (i = 0; i < size; i++) {
        if (map[i].position != NULL) {
            Entry *ptr = map[i].position;
            while (ptr != NULL) {
                printf(" %s ----> %p\n", ptr->key, (void*)&ptr->value);
                Hashmap_set(scope, ptr->key, ptr->value);
                ptr = ptr->next;
            }
        }
    }

    Bridge_reset(this);
}