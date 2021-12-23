// file

#include "../uyghur.c"

void ug_write_file(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    char *content = Bridge_popString(bridge);
    bool r = file_write(path, content);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_read_file(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    char *r = file_read(path);
    Bridge_startResult(bridge);
    if (r != NULL) Bridge_pushString(bridge, r);
    Bridge_return(bridge);
}

void ug_file_copy(Bridge *bridge)
{
    char *from = Bridge_popString(bridge);
    char *to = Bridge_popString(bridge);
    bool r = file_copy(from, to);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_file_rename(Bridge *bridge)
{
    char *from = Bridge_popString(bridge);
    char *to = Bridge_popString(bridge);
    double r = file_rename(from, to);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_file_remove(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    double r = file_remove(path);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_file_exist(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    bool r = file_exist(path);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_file_is_file(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    bool r = file_is_file(path);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_file_is_directory(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    bool r = file_is_directory(path);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_file_create_directory(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    int r = file_create_directory(path);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void lib_file_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "xojjet");
    //
    Bridge_pushKey(bridge, "yezish");
    Bridge_pushFunction(bridge, ug_write_file);
    Bridge_pushKey(bridge, "oqush");
    Bridge_pushFunction(bridge, ug_read_file);
    Bridge_pushKey(bridge, "kuchurush");
    Bridge_pushFunction(bridge, ug_file_copy);
    Bridge_pushKey(bridge, "yotkesh");
    Bridge_pushFunction(bridge, ug_file_rename);
    Bridge_pushKey(bridge, "uchurush");
    Bridge_pushFunction(bridge, ug_file_remove);
    Bridge_pushKey(bridge, "buMewjutmu");
    Bridge_pushFunction(bridge, ug_file_exist);
    Bridge_pushKey(bridge, "buXojjetmu");
    Bridge_pushFunction(bridge, ug_file_is_file);
    Bridge_pushKey(bridge, "buSomkimu");
    Bridge_pushFunction(bridge, ug_file_is_directory);
    Bridge_pushKey(bridge, "somkaQurush");
    Bridge_pushFunction(bridge, ug_file_create_directory);
    //
    Bridge_register(bridge);
}
