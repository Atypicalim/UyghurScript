// array

#ifndef H_PCT_ARRAY
#define H_PCT_ARRAY

#define ARRAY_DEFAULT_CAPACITY 1024
#define ARRAY_INVALID_INDEX 0

typedef int (* ArraySortFunction)(void const*, void const*);
typedef bool (* ArrayFindFunction)(void const*);

typedef struct _Array {
    struct _Object;
    void **elements;
    int length;
    int capacity;
    bool nullable;
} Array;

Array *Array_new()
{
    Array *array = (Array *)pct_mallloc(sizeof(Array));
    Object_init(array, PCT_OBJ_ARRAY);
    array->capacity = ARRAY_DEFAULT_CAPACITY;
    array->length = 0;
    array->nullable = false;
    array->elements = (void *)pct_mallloc(sizeof(void *) * array->capacity);
    for (int i = array->length; i < array->capacity; i++) array->elements[i] = NULL;
    return array;
}

void Array_clear(Array *this)
{
    for (int i = 0; i < this->length; i++) {
        Object_release(this->elements[i]);
        this->elements[i] = NULL;
    }
    this->length = 0;
}

void Array_free(Array *this)
{
    Array_clear(this);
    pct_free(this->elements);
    this->elements = NULL;
    Object_free(this);
}

bool _array_check_resize(Array *this, int length)
{
    if (length <= this->capacity) return true;
    int capacity = this->capacity;
    while (length > capacity) capacity = capacity + ARRAY_DEFAULT_CAPACITY;
    void **elements = pct_realloc(this->elements, sizeof(void *) * capacity);
    if (elements == NULL) return false;
    this->capacity = capacity;
    this->elements = elements;
    for (int i = this->length; i < this->capacity; i++) this->elements[i] = NULL;
    return true;
}

bool Array_set(Array *this, int index, void *element)
{
    if (!this->nullable && element == NULL) return false;
    if (index < 0 || (!this->nullable && index >= this->length)) return false;
    int length = index < this->length ? this->length : index + 1;
    bool isOk = _array_check_resize(this, length);
    if (!isOk) return false;
    if (this->elements[index] != NULL) {
        Object_release(this->elements[index]);
    }
    Object_retain(element);
    this->length = length;
    this->elements[index] = element;
    return true;
}

void *Array_get(Array *this, int index)
{
    if (index < 0 || index >= this->length) return NULL;
    return this->elements[index];
}

void *Array_getLast(Array *this)
{
    return Array_get(this, this->length - 1);
}

void *Array_getFirst(Array *this)
{
    return Array_get(this, 0);
}

void *Array_del(Array *this, int index)
{
    if (index < 0 || index >= this->length) return NULL;
    void *item = this->elements[index];
    for(int i = index; i < this->length; i++)
    {
        this->elements[i] = i != (this->length - 1) ? this->elements[i + 1] : NULL;
    }
    this->elements[this->length] = NULL;
    this->length = this->length - 1;
    Object_release(item);
    return item;
}

void *Array_delLast(Array *this)
{
    return Array_del(this, this->length - 1);
}

void *Array_delFirst(Array *this)
{
    return Array_del(this, 0);
}

bool _array_insert(Array *this, int index, void *element, bool isBefore)
{
    if (!this->nullable && element == NULL) return false;
    // validate
    if (index < 0) {
        index = 0;
        isBefore = true;
    } else if (index >= this->length) {
        index = this->length - 1;
        isBefore = false;
    }
    Object_retain(element);
    // 
    if (this->length == 0)
    {
        this->elements[0] = element;
        this->length = 1;
        return true;
    }
    // resize
    bool isOk = _array_check_resize(this, this->length + 1);
    if (!isOk) return false;
    // insert
    int i = this->length;
    int to = isBefore ? index : index + 1;
    for (i = this->length - 1; i >= to; i--)
    {
        this->elements[i + 1] = this->elements[i];
    }
    this->elements[to] = element;
    this->length = this->length + 1;
    return true;
}

bool Array_insertBefore(Array *this, int at, void *element)
{
    return _array_insert(this, at, element, true);
}

bool Array_insertAfter(Array *this, int at, void *element)
{
    return _array_insert(this, at, element, false);
}

bool Array_prepend(Array *this, void *element)
{
    return Array_insertBefore(this, 0, element);
}

bool Array_append(Array *this, void *element)
{
    return Array_insertAfter(this, this->length - 1, element);
}

bool Array_push(Array *this, void *element)
{
    return Array_insertAfter(this, this->length - 1, element);
}

void *Array_pop(Array *this)
{
    return Array_del(this, this->length - 1);
}

bool Array_unshift(Array *this, void *element)
{
    return Array_insertBefore(this, 0, element);
}

void *Array_shift(Array *this)
{
    return Array_del(this, 0);
}

// int compare(const void *num1, const void *num2) { return 0; }
void Array_sort(Array *this, ArraySortFunction func)
{
    qsort(this->elements, this->length, sizeof(void *), func);
}

// int search(const void *num2) { return true; }
int Array_find(Array *this, int from, int to, bool isReverse, ArrayFindFunction func)
{
    if (from < 0 || to > this->length || from >= to) return -1;
    int var = isReverse ? -1 : 1;
    if (isReverse) {
        int temp = from;
        from = to - 1;
        to = temp + 1;
    }
    void *item;
    bool result;
    for (int i = from; i < to; i + var) {
        item = Array_get(this, i);
        result = func(item);
        if (result) return i;
    }
    return -1;
}

Array *Array_slice(Array *this, int from, int to)
{
    Array *other = Array_new();
    if (from < 0 || to > this->length || from >= to) return other;
    for (int i = from; i < to; i++) Array_append(other, Array_get(this, i));
    return other;
}

char *Array_toString(Array *this)
{
    return tools_format("<Array p:%p s:%i>", this, this->length);
}

#endif
