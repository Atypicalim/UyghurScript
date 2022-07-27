// string

#ifndef H_UG_STRING
#define H_UG_STRING

#define STRING_MIN_SIZE 128

typedef struct _String {
    struct _Object;
    size_t length;
    size_t size;
    char *data;
} String;

String *String_new()
{
    String *string = (String *)malloc(sizeof(String));
    Object_init(string, UG_OBJECT_STRING);
    string->length = 0;
    string->size = STRING_MIN_SIZE + 1;
    string->data = malloc(string->size);
    string->data[string->length] = '\0';
    return string;
}

void _string_check_size(String *this, size_t length)
{
    if (this->size >= length + 1) return;
    while (this->size < length + 1) {
        this->size <<= 2;
        if (this->size <= 0) this->size--;
    }
    this->data = realloc(this->data, this->size);
}

int _string_validate_index(String *this, int index)
{
    if (index < 0) index = this->length + index;
    if (index < 0) index = 0;
    if (index >= this->length) index = this->length - 1;
    return index;
}

String *String_appendChar(String *this, char c)
{
    _string_check_size(this, this->length + 1);
    this->data[this->length] = c;
    this->length++;
    this->data[this->length] = '\0';
    return this;
}

String *String_prependChar(String *this, char c)
{
    _string_check_size(this, this->length + 1);
    memmove(this->data + 1, this->data, this->length);
    this->data[0] = c;
    this->length++;
    this->data[this->length] = '\0';
    return this;
}

String *String_appendStr(String *this, char *str)
{
    if (str == NULL || *str == '\0') return this;
    size_t len = strlen(str);
    if (len == 0) return this;
    _string_check_size(this, this->length + len);
    memmove(this->data + this->length, str, len);
    this->length += len;
    this->data[this->length] = '\0';
    return this;
}

String *String_prependStr(String *this, char *str)
{
    if (str == NULL || *str == '\0') return this;
    size_t len = strlen(str);
    if (len == 0) return this;
    _string_check_size(this, this->length + len);
    memmove(this->data + len, this->data, this->length);
    memmove(this->data, str, len);
    this->length += len;
    this->data[this->length] = '\0';
    return this;
}

String *String_delete(String *this, int from, int to)
{
    from = _string_validate_index(this, from);
    to = _string_validate_index(this, to);
    if (from > to) return this;
    memmove(this->data + from, this->data + to + 1, this->length - to - 1);
    this->length -= to - from + 1;
    this->data[this->length] = '\0';
    return this;
}

String *String_deleteBeginning(String *this, int position)
{
    return String_delete(this, 0, position);
}

String *String_deleteEnding(String *this, int position)
{
    return String_delete(this, position, -1);
}

String *String_clear(String *this)
{
    this->length = 0;
    this->data[this->length] = '\0';
}

String *String_append(String *this, char *str)
{
    return String_appendStr(this, str);
}

String *String_prepend(String *this, char *str)
{
    return String_prependStr(this, str);
}

int String_findNext(String *this, int from, char *target)
{
    from = _string_validate_index(this, from);
    size_t len = strlen(target);
    if (len <= 0) return -1;
    char *ptr = strstr(this->data + from, target);
    if (ptr == NULL) return -1;
    int pos = ptr - this->data;
    return pos;
}

int String_findLast(String *this, int from, char *target)
{
    from = _string_validate_index(this, from);
    size_t len = strlen(target);
    if (len <= 0) return -1;
    // TODO:
    return -1;
}

size_t String_length(String *this)
{
    return this->length;
}

char *String_get(String *this)
{
    return this->data;
}

char *String_set(String *this, char *str)
{
    String_delete(this, 0, -1);
    String_append(this, str);
}

void String_print(String *this)
{
    printf("<STRING:%s>\n", this->data);
}

char *String_data(String *this)
{
    char *s = malloc(this->length + 1);
    memcpy(s, this->data, this->length + 1);
    return s;
}

String *String_clone(String *this)
{
    String *s = String_new();
    String_append(s, String_get(this));
    return s;
}

void String_free(String *this)
{
    free(this->data);
    Object_free(this);
}

#endif
