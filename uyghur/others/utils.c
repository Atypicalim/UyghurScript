// utils

#ifndef H_UTILS
#define H_UTILS

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

char *system_get_name()
{
    return PLATFORM_NAME;
}

void system_exit_program(int code)
{
    exit(code);
}

char *system_execute_command(char *cmd)
{
    FILE *fp;
    if ((fp = popen(cmd, "r")) == NULL) return NULL;
    int BUFSIZE = 1024;
    char buf[BUFSIZE];
    String *result = String_new();
    char *temp1;
    char *temp2;
    while (fgets(buf, BUFSIZE, fp) != NULL) {
        String_appendArr(result, buf);
    }
    if(pclose(fp)) return NULL;
    char *r = String_dump(result);
    Object_release(result);
    return r;
}

void system_set_env(char *name, char *value)
{
    String *str = String_format("%s=%s", name, value);
    putenv(String_get(str));
    Object_release(str);
}

char *system_get_env(char *name)
{
    return getenv(name);
}

char *system_read_terminal()
{
    char value[1024];
    scanf(" %[^\n]", value);
    String *str = String_new();
    String_appendArr(str, value);
    char *data = String_dump(str);
    Object_release(str);
    return data;
}

void system_write_terminal(char *value)
{
    printf("%s", value);
}

int time_get_zone()
{
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    int hour1 = localTime->tm_hour;
    struct tm *globalTime = gmtime(&currentTime);
    int hour2 = globalTime->tm_hour;
    int distance = hour1 - hour2;
    return distance;
}

int time_get_seconds()
{
    time_t currentTime = time(NULL);
    int seconds = (int)currentTime;
    return seconds;
}

// "2000-02-02 22:22:22"
int time_create_seconds(char *str)
{
    if (strlen(str) != 19) return -1;
    int year = atoi(str + 0);
    int month = atoi(str + 5);
    int day = atoi(str + 8);
    int hour = atoi(str + 11);
    int minute = atoi(str + 14);
    int second = atoi(str + 17);
    struct tm info = {0};
    info.tm_year = year - 1900;
    info.tm_mon = month - 1;
    info.tm_mday = day;
    info.tm_hour = hour;
    info.tm_min = minute;
    info.tm_sec = second;
    info.tm_isdst = -1;
    time_t  result = mktime(&info);
    return (int) result;
}

// "%Y-%m-%d %H:%M:%S"
char *time_format_seconds(int seconds, char *format)
{
    time_t currentTime = seconds >= 0 ? seconds : time(NULL);
    struct tm *localTime = localtime(&currentTime);
    char formattedLocalDate[100];
    strftime(formattedLocalDate, 100, format, localTime);
    String *str = String_new();
    String_appendArr(str, formattedLocalDate);
    char *data = String_dump(str);
    Object_release(str);
    return data;
}

char *time_get_date()
{
    time_t currentTime = time(NULL);
    char *fullLocalDate = ctime(&currentTime);
    return fullLocalDate;
}

int time_get_clock()
{
    clock_t clockTime = clock();
    double seconds = (double)clockTime / CLOCKS_PER_SEC;
    return seconds;
}

void time_sleep_seconds(double seconds)
{
    sleep(seconds);
}

#endif
