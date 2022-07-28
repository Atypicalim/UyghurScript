// string

#ifndef H_UG_STRING
#define H_UG_STRING

#define STRING_MIN_SIZE 128

typedef struct _String {
    struct _Object;
    int length;
    int size;
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

void String_free(String *this)
{
    free(this->data);
    Object_free(this);
}

void _string_check_size(String *this, int length)
{
    if (this->size >= length + 1) return;
    while (this->size < length + 1 && this->size * 2 < INT_MAX) {
        this->size *= 2;
        if (this->size <= 0) this->size--;
    }
    this->data = realloc(this->data, this->size);
}

void _string_validate_index(String *this, int *from, int *to)
{
    if (*from < 0) *from = this->length + *from;
    if (*to < 0) *to = this->length + *to;
    // invalid
    if ((*from < 0 && *to < 0) || (*from >= this->length && *to >= this->length) && *from > *to) {
        *from = -1;
        *to = -1;
        return;
    } else if (*from < 0) {
        *from = 0;
        return;
    } else if (*to >= this->length) {
        *to = this->length - 1;
        return;
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
    int len = strlen(arr);
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
    int len = strlen(arr);
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
    int len = strlen(str);
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
    int len = strlen(str);
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

unsigned long String_hash(String *this)
{
    char *str = this->data;
    unsigned long hash = 5381;
    int c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

int String_findNext(String *this, int _from, char *target)
{
    int from = _from;
    int to = this->length;
    _string_validate_index(this, &from, &to);
    if (from < 0) return -1;
    int len = strlen(target);
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
    int len = strlen(target);
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

int String_length(String *this)
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
    printf("<STRING:%d,%lu,%s>\n", this->length, String_hash(this), this->data);
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

String *String_subString(String *this, int from, int to)
{
    String *s = String_new();
    int length = this->length;
    if (to <= 0 || from >= length || from >= to) return s;
    int len = to - from;
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
    int index = _index;
    int temp = _index;
    _string_validate_index(this, &index, &temp);
    if (index < 0) return '\0';
    return this->data[index];
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

String *String_replace(String *this, char *target, char *relacement, int from, int to, int count)
{
    int length = this->length;
    if (to <= 0 || from >= length || from >= to) return this;
    int targetLen = strlen(target);
    int replacementLen = strlen(relacement);
    if (targetLen <= 0 || count == 0) return this;
    int fromIndex = from;
    int foundIndex = -1;
    int replaceCount = 0;
    String *result = String_subString(this, -1, fromIndex);
    String *temp = NULL;
    while((foundIndex = String_findNext(this, fromIndex, target)) >= 0) {
        if (foundIndex >= to) break;
        temp = String_subString(this, fromIndex, foundIndex);
        String_append(result, String_get(temp));
        String_free(temp);
        String_append(result, relacement);
        fromIndex = foundIndex + targetLen;
        replaceCount++;
        if (count > 0 && replaceCount >= count) break;
    }
    temp = String_subString(this, fromIndex, this->length);
    String_append(result, String_get(temp));
    String_free(temp);
    String_set(this, String_get(result));
    String_free(result);
    return this;

}

String *_string_trim_both(String *this, bool isLeft, bool isRight)
{
    if (this->length == 0) return this;
    if (isLeft) {
        int start = 0;
        while(start < this->length && isspace(this->data[start])) start++;
        if (start > 0)  String_deleteStarting(this, start - 1);
    }
    if (isRight) {
        int end = this->length - 1;
        while(end >= 0 && isspace(this->data[end])) end--;
        if (end < this->length) String_deleteEnding(this, end + 1);
    }
    return this;
}

String *String_trimLeft(String *this)
{
    return _string_trim_both(this, true, false);
}

String *String_trimRight(String *this)
{
    return _string_trim_both(this, false, true);
}

String *String_trim(String *this)
{
    return _string_trim_both(this, true, true);
}

#endif
