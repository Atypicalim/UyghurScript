// tools

#ifndef H_TOOLS
#define H_TOOLS

void tools_error(const char* msg, ...) {
    va_list lst;
    va_start(lst, msg);
    printf("[%s] => ", LANG_ERR);
    vfprintf(stdout, msg, lst);
    printf("\n");
    va_end(lst);
    exit(1);
}

void tools_assert(bool value, const char *msg, ...)
{
    if (value == true) return;
    va_list lst;
    va_start(lst, msg);
    printf("[%s] => ", LANG_ERR);
    vfprintf(stdout, msg, lst);
    printf("\n");
    va_end(lst);
    exit(1);
}

char *tools_format(char *msg, ...)
{
    va_list lst;
    va_start(lst, msg);
    int bufsz = vsnprintf(NULL, 0, msg, lst);
    char* t = malloc(bufsz + 1);
    vsnprintf(t, bufsz + 1, msg, lst);
    va_end(lst);
    return t;
}

void tools_write_file(const char *path, const char *data)
{
    if (data == NULL) return;
    FILE *fp = fopen(path, "ab");
    if (fp != NULL)
    {
        fputs(data, fp);
        fclose(fp);
    }
}

char *tools_read_file(const char *path)
{
    char *text;
    FILE *file = fopen(path, "rb");
    if (file == NULL) return NULL;
    fseek(file, 0, SEEK_END);
    long lSize = ftell(file);
    text = (char *)malloc(lSize + 1);
    rewind(file);
    fread(text, sizeof(char), lSize, file);
    text[lSize] = '\0';
    fclose(file);
    return text;
}

char *tools_str_new(char *str, int extraLen)
{
    size_t len = strlen(str);
    char *dest = malloc(len + 1 + extraLen);
    dest[len + extraLen] = '\0';
    return dest;
}

char *tools_str_apent(char *str, char c, bool notFree)
{
    size_t len = strlen(str);
     char *dest = tools_str_new(str, 1);
    strcpy(dest, str);
    dest[len] = c;
    if (!notFree)
    {
        free(str);
    }
    return dest;
}

char *str_concat(char *one, char *two)
{
    int bufsz = snprintf(NULL, 0, "%s%s", one, two);
    char* buf = malloc(bufsz + 1);
    snprintf(buf, bufsz + 1, "%s%s", one, two);
    return buf;
}

bool is_equal(char *this, char*other)
{
    return strcmp(this, other) == 0;
}

char *b2s(bool value)
{
    if (value == true) return "true";
    if (value == false) return "false";
    return "empty";
}

char *o2s(void *value)
{
    if (value == NULL) return "<NULL>";
    return tools_format("<Object => p:%d>", value);
}


char decode_escape(char c)
{
    switch (c)
    {
    case '[':
        return '[';
        break;
    case ']':
        return ']';
        break;
    case '\\':
        return '\\';
        break;
    case 'n':
        return '\n';
        break;
    case 'a':
        return '\a';
        break;
    case 'b':
        return '\b';
        break;
    case 'f':
        return '\f';
        break;
    case 'v':
        return '\v';
        break;
    case 'r':
        return '\r';
        break;
    case 't':
        return '\t';
        break;
    default:
        return '\0';  
        break;
    }       
}

char *str_replace(char *origin, char *from, char *to, int direction, int num)
{
    if (origin == NULL || from == NULL || to == NULL || direction == 0 || num == 0) return origin;
    int lenOrigin = strlen(origin);
    int lenFrom = strlen(from);
    int lenTo = strlen(to);
    if (lenOrigin == 0 || lenFrom == 0) return origin;
    //
    int countFound = 0;
    for(char *left = origin;(left = strstr(left, from)) != 0;left = left + lenFrom) countFound++;
    if (countFound == 0) return origin;
    int countReplace = num < countFound & num > 0 ? num : countFound;
    //
    int size = lenOrigin - lenFrom * countReplace + lenTo * countReplace;
    char *target = malloc(size + 1);
    if (target == NULL) return origin;
    //
    char *tmp = target;    // varies
    int lenFront; // distance between rep and end of last rep
    char *foundPoint;    // the next insert point
    int index = 1;
    while (index <= countFound) {
        foundPoint = strstr(origin, from);
        lenFront = foundPoint - origin;
        tmp = strncpy(tmp, origin, lenFront);
        tmp = tmp + lenFront;
        //
        if ((direction > 0 && index <= countReplace) || (direction < 0 && index > countFound - countReplace))
        {
            tmp = strcpy(tmp, to);
            tmp = tmp + lenTo;
        }
        else
        {
            tmp = strcpy(tmp, from);
            tmp = tmp + lenFrom;
        }
        //
        origin += lenFront + lenFrom;
        index++;
    }
    strcpy(tmp, origin);
    // need free
    return target;
}

int str_find(char *origin, char *find, int from, int to, int index)
{
    //
    int lenOrigin = strlen(origin);
    int lenFind = strlen(find);
    if (lenOrigin == 0 || lenFind == 0 || index == 0) return 0;
    //
    to = to > 0 ? to : (lenOrigin + to + 1);
    from = from >= 1 ? from : 1;
    to = to <= lenOrigin ? to : lenOrigin;
    if (from > to) return 0;
    //
    char *fromPointer = origin + from - 1;
    char *toPointer = origin + to - 1;
    int countFound;
    char *left;
    //
    countFound = 0;
    left = strstr(fromPointer, find);
    while(left != 0)
    {
        if (toPointer - left < lenFind) break;
        countFound++;
        left = left + lenFind;
        left = strstr(left, find);
    }
    //
    int targetIndex = index > 0 ? index : countFound + index + 1;
    if (targetIndex > countFound || targetIndex < 1) return 0;  
    //
    countFound = 0;
    left = strstr(fromPointer, find);
    while(left != 0)
    {
        if (toPointer - left < lenFind) break;
        countFound++;
        if (countFound == targetIndex)
        {
            return left - origin + 1;
        }
        left = left + lenFind;
        left = strstr(left, find);
    }
    //
    return 0;
}

char *str_cut(char *origin, int from, int to)
{
    int len = strlen(origin);
    to = to > 0 ? to : (len + to + 1);
    from = from >= 1 ? from : 1;
    to = to <= len ? to : len;
    if (from > to) return "";
    int size = to - from + 1;
    char* new = malloc(size);
    strncpy(new, origin + from - 1, size);
    new[size] = '\0';
    return new;
}

int str_count(char *origin)
{
    return strlen(origin);
}

char *str_link(char *origin, char *other)
{
    int size = snprintf(NULL, 0, "%s%s", origin, other);
    char* buf = malloc(size + 1);
    snprintf(buf, size + 1, "%s%s", origin, other);
    return buf;
}

char *str_format(char *template, ...)
{
    va_list lst;
    va_start(lst, template);
    int bufsz = vsnprintf(NULL, 0, template, lst);
    char* t = malloc(bufsz + 1);
    vsnprintf(t, bufsz + 1, template, lst);
    va_end(lst);
    return t;
}

int num_random(int from, int to)
{
    int big = to > from ? to : from;
    int small = to > from ? from : to;
    int count = big - small + 1;
    int num = rand() % count;
    int r = small + num;
    return r;
}

#endif
