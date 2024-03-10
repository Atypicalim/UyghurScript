// string

#ifndef H_PCT_UG_STRING
#define H_PCT_UG_STRING

#define STRING_MIN_CAPACITY 128

typedef struct _String {
    struct _Object;
    int length;
    int capacity;
    char *data;
} String;

String *String_new()
{
    String *string = (String *)pct_mallloc(sizeof(String));
    Object_init(string, PCT_OBJ_STRING);
    string->length = 0;
    string->capacity = STRING_MIN_CAPACITY + 1;
    string->data = pct_mallloc(string->capacity);
    string->data[string->length] = '\0';
    return string;
}

void String_free(String *this)
{
    pct_free(this->data);
    Object_free(this);
}

void _string_check_capacity(String *this, int length)
{
    if (this->capacity >= length + 1) return;
    while (this->capacity < length + 1 && this->capacity * 2 < INT_MAX) {
        this->capacity *= 2;
        if (this->capacity <= 0) this->capacity--;
    }
    this->data = pct_realloc(this->data, this->capacity);
}

String *String_appendChar(String *this, char c)
{
    _string_check_capacity(this, this->length + 1);
    this->data[this->length] = c;
    this->length++;
    this->data[this->length] = '\0';
    return this;
}

String *String_prependChar(String *this, char c)
{
    _string_check_capacity(this, this->length + 1);
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
    _string_check_capacity(this, this->length + len);
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
    _string_check_capacity(this, this->length + len);
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
    _string_check_capacity(this, this->length + len);
    memmove(this->data + len, this->data, this->length);
    memmove(this->data, str, len);
    this->length += len;
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

String *String_insert(String *this, int at, String *that)
{
    if (at < 0 || at > this->length || that == NULL || that->length <= 0) return this;
    _string_check_capacity(this, this->length + that->length);
    char *temp = pct_mallloc(this->length - at);
    memmove(temp, this->data + at, this->length - at);
    memmove(this->data + at, that->data, that->length);
    memmove(this->data + at + that->length, temp, this->length - at);
    pct_free(temp);
    this->length += that->length;
    this->data[this->length] = '\0';
    return this;
}

String *String_delete(String *this, int from, int to)
{
    limit_range(this->length, true, &from, &to);
    int len = this->length;
    char *dst = this->data + from;
    char *src = this->data + to + 1;
    size_t cnt = this->length - to - 1;
    memmove(dst, src, cnt);
    this->length -= (to - from + 1);
    this->data[this->length] = '\0';
    return this;
}

String *String_deleteStarting(String *this, int to)
{
    return String_delete(this, 0, to);
}

String *String_deleteEnding(String *this, int from)
{
    return String_delete(this, from, this->length);
}

String *String_clear(String *this)
{
    this->length = 0;
    this->data[this->length] = '\0';
    return this;
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

int String_findNext(String *this, int from, char *target)
{
    int len = strlen(target);
    if (this->length <= 0 || from < 0 || len <= 0) return -1;
    char *ptr = strstr(this->data + from, target);
    if (ptr == NULL) return -1;
    int pos = ptr - this->data;
    return pos;
}

int String_findLast(String *this, int to, char *target)
{
    int len = strlen(target);
    if (this->length <= 0 || to > this->length || len <= 0) return -1;
    int containIndex = to - len;
    int nextIndex = this->length - 1;
    int foundIndex = -1;
    char *ptr = NULL;
    while (nextIndex >= 0) {
        if ((ptr = strstr(this->data + nextIndex, target)) != NULL && (foundIndex = ptr - this->data) <= containIndex) return foundIndex;
        nextIndex--;
    }
    return -1;
}

// [N, index1, index2, ... indexN]
int *String_findAll(String *this, char *target)
{
    int size = 128;
    int *result = (int *)pct_mallloc(sizeof(int) * size);
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
                result = (int *)pct_realloc(result, size);
            }
            result[result[0]] = foundIndex;
            lastRecorded = foundIndex;
        }
    }
    return result;
}

int String_capacity(String *this)
{
    return this->capacity;
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
    String_appendStr(this, str);
    return this;
}

void String_print(String *this)
{
    printf("<STRING:%d,%lu,%s>\n", this->length, String_hash(this), this->data);
}

char *String_dump(String *this)
{
    char *s = pct_mallloc(this->length + 1);
    memcpy(s, this->data, this->length + 1);
    return s;
}

String *String_clone(String *this)
{
    String *s = String_new();
    String_appendStr(s, String_get(this));
    return s;
}

String *String_format(char *template, ...)
{
    va_list lst;
    va_list lstCopy;
    va_start(lst, template);
    va_copy(lstCopy, lst);
    int bufsz = vsnprintf(NULL, 0, template, lst);
    char *t = pct_mallloc(bufsz + 1);
    vsnprintf(t, bufsz + 1, template, lstCopy);
    va_end(lst);
    va_end(lstCopy);
    String *s = String_new();
    String_appendStr(s, t);
    pct_free(t);
    return s;
}

String* String_repeat(String *this, int count)
{
    if (count <= 0 || this->length <= 0) return this;
    char *data = String_dump(this);
    while (--count > 0) {
        String_appendStr(this, data);
    }
    pct_free(data);
    return this;
}

String *String_subString(String *this, int from, int to)
{
    limit_range(this->length, true, &from, &to);
    String *s = String_new();
    int len = to - from + 1;
    char *temp = pct_mallloc(len + 1);
    memmove(temp, this->data + from, len);
    temp[len] = '\0';
    String_appendStr(s, temp);
    pct_free(temp);
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

char String_getChar(String *this, int index)
{
    if (index < 0 || index >= this->length) return '\0';
    return this->data[index];
}

String *String_setChar(String *this, int index, char c)
{
    if (index < 0 || index >= this->length || c == '\0') return this;
    this->data[index] = c;
    return this;
}

String *String_reverse(String *this)
{
    int left = 0;
    int right = this->length - 1;
    char temp;
    while(left < right) {
        temp = this->data[left];
        this->data[left] = this->data[right];
        this->data[right] = temp;
        left++;
        right--;
    }
}

String *String_upper(String *this)
{
    int index = 0;
    while(index < this->length) this->data[index++] = toupper(this->data[index]);
}

String *String_lower(String *this)
{
    int index = 0;
    while(index < this->length) this->data[index++] = tolower(this->data[index]);
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
    limit_range(this->length, true, &from, &to);
    int thisLen = String_length(this);
    int targetLen = strlen(target);
    int replacementLen = strlen(relacement);
    if (targetLen <= 0 || count == 0) return this;
    int fromIndex = from;
    int foundIndex = -1;
    int replaceCount = 0;
    String *result = String_new();
    String *temp = NULL;
    while((foundIndex = String_findNext(this, fromIndex, target)) >= 0) {
        if (foundIndex >= to) break;
        temp = String_subString(this, fromIndex, foundIndex - 1);
        String_append(result, String_get(temp));
        String_free(temp);
        String_append(result, relacement);
        fromIndex = foundIndex + targetLen;
        replaceCount++;
        if (count > 0 && replaceCount >= count) break;
    }
    temp = String_subString(this, fromIndex, this->length - 1);
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
        if (start > 0)  String_deleteStarting(this, start);
    }
    if (isRight) {
        int end = this->length - 1;
        while(end >= 0 && isspace(this->data[end])) end--;
        if (end < this->length - 1) String_deleteEnding(this, end + 1);
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
