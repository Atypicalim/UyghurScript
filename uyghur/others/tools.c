// tools

#ifndef H_TOOLS
#define H_TOOLS

// os type
#define PLATFORM_WINDOWS "PLATFORM_WINDOWS"
#define PLATFORM_APPLE "PLATFORM_APPLE"
#define PLATFORM_LINUX "PLATFORM_LINUX"
#define PLATFORM_UNIX "PLATFORM_UNIX"
#define PLATFORM_FREEBSD "PLATFORM_FREEBSD"
#define PLATFORM_UNKNOWN "PLATFORM_UNKNOWN"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PLATFORM_NAME PLATFORM_WINDOWS
    #define IS_WINDOWS
#elif __APPLE__
    #define PLATFORM_NAME PLATFORM_APPLE
    #define IS_APPLE
#elif __linux__
    #define PLATFORM_NAME PLATFORM_LINUX
    #define IS_LINUX
#elif __unix || __unix__
    #define PLATFORM_NAME PLATFORM_UNIX
#elif __FreeBSD__
    #define PLATFORM_NAME PLATFORM_FREEBSD
#else
    #define PLATFORM_NAME PLATFORM_UNKNOWN
#endif

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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

void tools_warning(const char* msg, ...) {
    va_list lst;
    va_start(lst, msg);
    printf("[%s] => ", LANG_WAR);
    vfprintf(stdout, msg, lst);
    printf("\n");
    va_end(lst);
}

char *tools_format(char *msg, ...)
{
    va_list lst;
    va_list lstCopy;
    va_start(lst, msg);
    va_copy(lstCopy, lst);
    int bufsz = vsnprintf(NULL, 0, msg, lst);
    char* t = malloc(bufsz + 1);
    vsnprintf(t, bufsz + 1, msg, lstCopy);
    va_end(lst);
    va_end(lstCopy);
    return t;
}

bool is_equal(char *this, char*other)
{
    return strcmp(this, other) == 0;
}

bool is_number_begin(char c)
{
    return c == '+' || c == '-' || isdigit(c);
}

bool is_number_body(char c)
{
    return isdigit(c) || c == '.';
}

bool is_letter_begin(char c)
{
    return isalpha(c) || c == '_' || c == '$';
}

bool is_letter_body(char c)
{
    return isalnum(c) || c == '_';
}

bool is_letter_valid(const char* str)
{
    char c;
    for (int i = 0; str[i] != '\0'; i++) {
        c = str[i];
        if (i == 0 && !is_letter_begin(c)) return false;
        if (i != 0 && !is_letter_body(c)) return false;
    }
    return true;
}

bool is_string_open(char c) {
    return c == '[';
}

bool is_string_body(char c) {
    return c != '[' && c != ']';
}

bool is_string_close(char c) {
    return c == ']';
}

char convert_border_pair(char c) {
    if (c == '{') return '}';
    if (c == '}') return '{';
    if (c == '[') return ']';
    if (c == ']') return '[';
    if (c == '(') return ')';
    if (c == ')') return '(';
    return NULL;
}

bool is_border_open(char c) {
    return c == '{' || c == '[' || c == '(';
}

bool is_border_close(char c) {
    return c == '}' || c == ']' || c == ')';
}

bool is_calculator(char c)
{
    return c == '=';
}

bool is_calculation(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

bool is_scope(char c)
{
    return c == '*' || c == '+' || c == '-';
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

int p2i(void* value){
    return *((int*) value);
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

int char_to_int(char c)
{
    int i = c - '0';
    if (i >= 49) i-= 39;
    if (i < 0 || i > 15) return 0;
    return i;
}

char *arr_to_str(char arr[], int len)
{
    char* ptr = (char *)malloc(len * sizeof(char) + 1);
    for (int i = 0; i < len; i++)
    {
        ptr[i] = arr[i];
    }
    ptr[len] = '\0';
    return ptr;
}

int hex_to_int(char *str)
{
    int len = strlen(str);
    int result = 0;
    int number;
    for (int i = len - 1; i >= 0; i--)
    {
        number = char_to_int(str[len - i - 1]);
        result = result + number * pow(16, i);
    }
    return result;
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

bool file_write(char *path, char *data)
{
    if (data == NULL) return false;
    FILE *fp = fopen(path, "ab");
    if (fp == NULL) return false;
    fputs(data, fp);
    fclose(fp);
    return true;
}

char *file_read(char *path)
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

bool file_copy(char *path, char *to)
{
    int BUF_SIZE = 1024;
    FILE *src, *dst;
    size_t in, out;
    src = fopen(path, "rb");
    dst = fopen(to, "wb");
    if (src == NULL || dst == NULL) {
        if (src != NULL) return fclose(src);
        if (dst != NULL) return fclose(dst);
        return false;
    }
    char *buf = (char*) malloc(BUF_SIZE * sizeof(char));
    while (1) {
        in = fread(buf, sizeof(char), BUF_SIZE, src);
        if (0 == in) break;
        out = fwrite(buf, sizeof(char), in, dst);
        if (0 == out) break;
    }
    fclose(src);
    fclose(dst);
    free(buf);
    return true;
}

int file_rename(char *path, char *to)
{
    return rename(path, to);
}

int file_remove(char *path)
{
    return unlink(path);
}

bool file_exist(char *path)
{
    return access(path, F_OK) == 0;
}

bool file_is_file(char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool file_is_directory(char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int file_create_directory(char *path)
{
    char tmp[1024];
    char *p = NULL;
    size_t len;
    snprintf(tmp, sizeof(tmp),"%s",path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
    {
        if (*p != '/') continue;
        *p = 0;
        #ifdef IS_WINDOWS
            mkdir(tmp);
        #else
            mkdir(tmp, 777);
        #endif
        *p = '/';
    }
    #ifdef IS_WINDOWS
        return mkdir(tmp);
    #else
        return mkdir(tmp, 777);
    #endif
}

#endif
