// token

#ifndef H_PCT_FOLIAGE
#define H_PCT_FOLIAGE

typedef struct _Foliage {
    struct _Object;
    void *data;
    void *left;
    void *right;
} Foliage;

void Foliage_init(void *_foliage, void *data)
{
    Foliage *foliage = _foliage;
    foliage->data = data;
    foliage->left = NULL;
    foliage->right = NULL;
}

Foliage *Foliage_new(void *data)
{
    Foliage *foliage = (Foliage *)pct_mallloc(sizeof(Foliage));
    Object_init(foliage, PCT_OBJ_FOLIAGE);
    Foliage_init(foliage, data);
    return foliage;
}

void Foliage_print(void *_this)
{
    if (_this == NULL)
    {
        printf("[(Foliage) => NULL]\n");
    }
    else
    {
        Foliage *this = _this;
        printf("[(Foliage) => address:%d, data:%d]\n", this, this->data);
    }
}

void Foliage_free(Foliage *this)
{
    this->left = NULL;
    this->right = NULL;
    if (this->data != NULL)
    {
        pct_free(this->data);
        this->data = NULL;
    }
    Object_free(this);
}

#endif
