// Hashkey

#ifndef H_PCT_HASHKEY
#define H_PCT_HASHKEY

typedef struct _Hashkey {
    struct _Object;
    String *key;
    void *value;
    struct _Hashkey *next;
} Hashkey;


Hashkey *Hashkey_set(Hashkey *this, void *value)
{
    this->value = value;
    return this;
}

Hashkey *Hashkey_new(String *key, void *value)
{
    Hashkey *hashkey = (Hashkey *)pct_mallloc(sizeof(Hashkey));
    hashkey->key = key;
    hashkey->value = value;
    hashkey->next = NULL;
    Object_retain(key);
    Object_init(hashkey, PCT_OBJ_HASHKEY);
    return hashkey;
}

void Hashkey_free(void *_this)
{
    Hashkey *this = _this;
    Object_release(this->key);
    this->key = NULL;
    this->value = NULL;
    this->next = NULL;
    Object_free(this);
}

#endif
