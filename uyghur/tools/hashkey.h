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
    if (this->value != NULL) Object_release(this->value);
    Object_retain(value);
    this->value = value;
    return this;
}

Hashkey *Hashkey_new(String *key, void *value)
{
    Hashkey *hashkey = (Hashkey *)pct_mallloc(sizeof(Hashkey));
    hashkey->key = NULL;
    hashkey->value = NULL;
    hashkey->next = NULL;
    // 
    hashkey->key = key;
    Object_retain(key);
    hashkey->value = value;
    Object_retain(value);
    //
    Object_init(hashkey, PCT_OBJ_HASHKEY);
    return hashkey;
}

void Hashkey_free(void *_this)
{
    Hashkey *this = _this;
    this->next = NULL;
    Object_release(this->value);
    Object_release(this->key);
    Object_free(this);
}

#endif
