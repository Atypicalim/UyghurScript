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

int _string_validate_index(String *this, int *from, int *to)
{
    if (*from < 0) *from = this->length + *from;
    if (*to < 0) *to = this->length + *to;
    if ((*from < 0 && *to < 0) || (*from > this->length && *to > this->length) || (*from > *to)) {
        *from = -1;
        *to = -1;
    } else if (*from < 0) {
        *from = 0;
    } else if (*to > this->length) {
        *to = this->length - 1;
    }
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

String *String_appendArr(String *this, char arr[])
{
    if (arr == NULL) return this;
    size_t len = strlen(arr);
    if (len == 0) return this;
    _string_check_size(this, this->length + len);
    memmove(this->data + this->length, arr, len);
    this->length += len;
    this->data[this->length] = '\0';
    return this;
}

String *String_prependArr(String *this, char arr[])
{
    if (arr == NULL) return this;
    size_t len = strlen(arr);
    if (len == 0) return this;
    memmove(this->data + len, this->data, this->length);
    memmove(this->data, arr, len);
    this->length += len;
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

String *String_delete(String *this, int _from, int _to)
{
    int from = _from;
    int to = _to;
    _string_validate_index(this, &from, &to);
    if (from < 0) return this;
    memmove(this->data + from, this->data + to + 1, this->length - to - 1);
    this->length -= (to - from + 1);
    this->data[this->length] = '\0';
    return this;
}

String *String_deleteStarting(String *this, int position)
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
    return this;
}

String *String_append(String *this, char *str)
{
    return String_appendStr(this, str);
}

String *String_prepend(String *this, char *str)
{
    return String_prependStr(this, str);
}

int String_findNext(String *this, int _from, char *target)
{
    int from = _from;
    int to = this->length;
    _string_validate_index(this, &from, &to);
    if (from < 0) return -1;
    size_t len = strlen(target);
    if (len <= 0) return -1;
    char *ptr = strstr(this->data + from, target);
    if (ptr == NULL) return -1;
    int pos = ptr - this->data;
    return pos;
}

int String_findLast(String *this, int _to, char *target)
{
    int from = 0;
    int to = _to;
    _string_validate_index(this, &from, &to);
    if (from < 0) return -1;
    size_t len = strlen(target);
    if (len <= 0) return -1;
    int containIndex = to - len + 1;
    int nextIndex = this->length - 1;
    int foundIndex = -1;
    char *ptr = NULL;
    while (nextIndex >= 0) {
        if ((ptr = strstr(this->data + nextIndex, target)) != NULL && (foundIndex = ptr - this->data) <= containIndex) {
            return foundIndex;
        }
        nextIndex--;
    }
    return -1;
}

// [N, index1, index2, ... indexN]
int *String_findAll(String *this, char *target)
{
    int size = 128;
    int *result = (int *)malloc(sizeof(int) * size);
    result[0] = 0;
    int foundIndex = -1;
    int nextIndex = 0;
    int lastRecorded = -1;
    while ((foundIndex = String_findNext(this, nextIndex, target)) >= 0) {
        nextIndex++;
        if (foundIndex != lastRecorded) {
            result[0]++;
            if (result[0] > size - 1) {
                size <<= 2;
                result = (int *)realloc(result, size);
            }
            result[result[0]] = foundIndex;
            lastRecorded = foundIndex;
        }
    }
    return result;
}

size_t String_length(String *this)
{
    return this->length;
}

char *String_get(String *this)
{
    return this->data;
}

String *String_set(String *this, char *str)
{
    String_delete(this, 0, -1);
    String_append(this, str);
    return this;
}

void String_print(String *this)
{
    printf("<STRING:%d,%s>\n", this->length, this->data);
}

char *String_dump(String *this)
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

String *String_format(char *template, ...)
{
    va_list lst;
    va_start(lst, template);
    int bufsz = vsnprintf(NULL, 0, template, lst);
    char* t = malloc(bufsz + 1);
    vsnprintf(t, bufsz + 1, template, lst);
    va_end(lst);
    String *s = String_new();
    String_append(s, t);
    free(t);
    return s;
}

String *String_subString(String *this, int _from, int _to)
{
    int from = _from;
    int to = _to;
    _string_validate_index(this, &from, &to);
    String *s = String_new();
    if (from > to) return s;
    int len = to - from + 1;
    char *temp = malloc(len + 1);
    memmove(temp, this->data + from, len);
    temp[len] = '\0';
    String_appendStr(s, temp);
    free(temp);
    return s; 
}

int String_compare(String *this, String *that)
{
    return strcmp(this->data, that->data);
}

bool String_equal(String *this, String *that)
{
    return String_compare(this, that) == 0;
}

char String_charAt(String *this, int _index)
{
    int from = _index;
    int to = _index;
    _string_validate_index(this, &from, &to);
    if (from < 0) return '\0';
    return this->data[from];
}

bool String_startsWith(String *this, char *target)
{
    return String_findNext(this, 0, target) == 0;
}

bool String_endsWith(String *this, char *target)
{
    return String_findLast(this, -1, target) == this->length - strlen(target);
}

bool String_contains(String *this, char *target)
{
    return String_findNext(this, 0, target) >= 0;
}

bool String_replace(String *this, char *target, char *relacement, bool isReverce, int count)
{
    // 
}

String *String_trim(String *this, bool isLeft, bool isRight)
{

}

int String_hash(String *this)
{
    //
}

void String_free(String *this)
{
    free(this->data);
    Object_free(this);
}

#endif
