// array

#ifndef H_ARRAY
#define H_ARRAY

#define ARRAY_DEFAULT_CAPACITY 1024
#define ARRAY_INVALID_INDEX 0

typedef int (* ArraySortFunction)(void const*, void const*);
typedef bool (* ArrayFindFunction)(void const*);

typedef struct _Array {
    struct _Object;
    void **elements;
    int size;
    int capacity;
} Array;

void _array_clear(Array *this)
{
    for (int i = this->size; i < this->capacity; i++) this->elements[i] = NULL;
}

Array *Array_new()
{
    Array *array = (Array *)malloc(sizeof(Array));
    Object_init(array, UG_OBJECT_ARRAY);
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

// int compare(const void *num1, const void *num2) { return 0; }
void Array_sort(Array *this, ArraySortFunction func)
{
    qsort(this->elements, this->size, sizeof(void *), func);
}

// int search(const void *num2) { return true; }
int Array_find(Array *this, int from, int to, bool isReverse, ArrayFindFunction func)
{
    if (from == 0 || to == 0 || this->size == 0) return 0;
    if (from < 0) from = this->size + from + 1;
    if (to < 0) to = this->size + to + 1;
    if (from < 1 || from > this->size) return 0;
    if (to < 1 || to > this->size) return 0;
    if (from > to) return 0;
    void *item;
    bool result;
    if (isReverse)
    {
        for (int i = to; i >= from; i--)
        {
            item = Array_get(this, i);
            result = func(item);
            if (result) return i;
        }
    }
    else
    {
        for (int i = from; i <= to; i++)
        {
            item = Array_get(this, i);
            result = func(item);
            if (result) return i;
        }
    }
    return 0;
}

Array *Array_slice(Array *this, int from, int to)
{
    if (from == 0 || to == 0 || this->size == 0) return NULL;
    if (from < 0) from = this->size + from + 1;
    if (to < 0) to = this->size + to + 1;
    if (from < 1 || from > this->size) return NULL;
    if (to < 1 || to > this->size) return NULL;
    if (from > to) return NULL;
    Array *other = Array_new();
    void *item;
    for (int i = from; i <= to; i++)
    {
        item = Array_get(this, i);
        Array_append(other, item);
    }
    return other;
}

void Array_clear(Array *this)
{
    this->size = 0;
    _array_clear(this);
}

void Array_print(Array *this)
{
    printf("[(ARRAY) => p:%d, s:%d, c:%d]\n", this, this->size, this->capacity);
}

void Array_free(Array *this)
{
    free(this->elements);
    this->elements = NULL;
    Object_free(this);
}

#endif
