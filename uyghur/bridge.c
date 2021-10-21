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


void add_map_item(void *targetMap, char *key, void *data)
{
    Hashmap_set(targetMap, key, data);
}

void Bridge_commit(Bridge *this)
{
    Hashmap *scope = this->uyghur->executer->rootScope;
    Hashmap_foreach(this->cache, scope, add_map_item);
    Bridge_reset(this);
}