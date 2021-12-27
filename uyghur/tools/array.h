// array

#ifndef H_ARRAY
#define H_ARRAY

#define ARRAY_DEFAULT_CAPACITY 1024
#define ARRAY_INVALID_INDEX 0

typedef int (* ArrayCompareFunction)(void const*, void const*);

typedef struct _Array {
    void **elements;
    int size;
    int capacity;
} Array;

void _array_clear(Array *this)
{
    for (int i = this->size; i < this->capacity; i++) this->elements[i] = NULL;
}

Array *Array_new(int evementSize)
{
    Array *array = (Array *)malloc(sizeof(Array));
    array->capacity = ARRAY_DEFAULT_CAPACITY;
    array->size = 0;
    array->elements = (void *)malloc(sizeof(void *) * array->capacity);
    _array_clear(array);
    return array;
}

bool _array_resize(Array *this, int minSize)
{
    int capacity = ARRAY_DEFAULT_CAPACITY;
    while (minSize > capacity) capacity = capacity + ARRAY_DEFAULT_CAPACITY;
    void **elements = realloc(this->elements, sizeof(void *) * capacity);
    if (elements == NULL) return false;
    this->capacity = capacity;
    this->elements = elements;
    _array_clear(this);
    return true;
}

bool Array_set(Array *this, int index, void *element)
{
    if (index == 0) return false;
    if (index < 0) index = this->size + index + 1;
    if (index < 1 || index > this->size) return NULL;
    this->elements[index - 1] = element;
    return true;
}

void *Array_get(Array *this, int index)
{
    if (index == 0 || this->size == 0) return NULL;
    if (index < 0) index = this->size + index + 1;
    if (index < 1 || index > this->size) return NULL;
    return this->elements[index - 1];
}

void *Array_delete(Array *this, int index)
{
    if (index == 0 || this->size == 0) return NULL;
    if (index < 0) index = this->size + index + 1;
    if (index < 1 || index > this->size) return NULL;
    void *item = this->elements[index - 1];
    for(int i = index; i <= this->size; i++)
    {
        this->elements[i - 1] = i != this->size ? this->elements[i] : NULL;
    }
    this->elements[this->size] = NULL;
    this->size = this->size - 1;
    return item;
}

bool _array_insert(Array *this, void *element, int index, bool isBefore)
{
    // first item
    if (this->size == 0)
    {
        if (index > 1 || index < -1) return false;
        this->elements[0] = element;
        this->size = 1;
        return true;
    }
    // validate
    if (index == 0) return false;
    if (index < 0) index = this->size + index + 1;
    if (index < 1 || index > this->size) return false;
    // resize
    if (this->size + 1 > this->capacity) {
        bool isOk = _array_resize(this, this->size + ARRAY_DEFAULT_CAPACITY);
        if (!isOk) return false;
    }
    // insert
    int i = this->size;
    int to = isBefore ? index : index + 1;
    for (i == this->size; i > to; i--)
    {
        this->elements[i + 1 - 1] = this->elements[i - 1];
    }
    this->elements[to - 1] = element;
    this->size = this->size + 1;
    return true;
}

bool Array_insertBefore(Array *this, void *element, int at)
{
    return _array_insert(this, element, at, true);
}

bool Array_insertAfter(Array *this, void *element, int at)
{
    return _array_insert(this, element, at, false);
}

bool Array_prepend(Array *this, void *element)
{
    return _array_insert(this, element, 1, true);
}

bool Array_append(Array *this, void *element)
{
    return _array_insert(this, element, -1, false);
}

int Array_size(Array *this)
{
    return this->size;
}

// int compare(const void * num1, const void * num2) { return 0; }
void Array_sort(Array *this, ArrayCompareFunction func)
{
    qsort(this->elements, this->size, sizeof(void *), func);
}

void Array_print(Array *this)
{
    printf("[(ARRAY) => p:%d, s:%d, c:%d]\n", this, this->size, this->capacity);
}

void Array_free(Array *this)
{
    free(this->elements);
    this->elements = NULL;
    free(this);
}

#endif
