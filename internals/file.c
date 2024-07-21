// file

#include "../uyghur/uyghur.c"

void native_file_write(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    char *content = Bridge_receiveString(bridge);
    bool r = file_write(path, content);
    Bridge_returnBoolean(bridge, r);
}

void native_file_read(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    char *r = file_read(path);
    if (r != NULL) {
        Bridge_returnString(bridge, r);
        pct_free(r);
    } else {
        Bridge_returnEmpty(bridge);
    }
}

void native_file_copy(Bridge *bridge)
{
    char *from = Bridge_receiveString(bridge);
    char *to = Bridge_receiveString(bridge);
    bool r = file_copy(from, to);
    Bridge_returnBoolean(bridge, r);
}

void native_file_rename(Bridge *bridge)
{
    char *from = Bridge_receiveString(bridge);
    char *to = Bridge_receiveString(bridge);
    double r = file_rename(from, to);
    Bridge_returnNumber(bridge, r);
}

void native_file_remove(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    double r = file_remove(path);
    Bridge_returnNumber(bridge, r);
}

void native_file_exist(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    bool r = file_exist(path);
    Bridge_returnBoolean(bridge, r);
}

void native_file_is_file(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    bool r = file_is_file(path);
    Bridge_returnBoolean(bridge, r);
}

void native_file_is_directory(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    bool r = file_is_directory(path);
    Bridge_returnBoolean(bridge, r);
}

void native_file_create_directory(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    int r = file_create_directory(path);
    Bridge_returnNumber(bridge, r);
}

void lib_file_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(file_write);
    BRIDGE_BIND_NATIVE(file_read);
    BRIDGE_BIND_NATIVE(file_copy);
    BRIDGE_BIND_NATIVE(file_rename);
    BRIDGE_BIND_NATIVE(file_remove);
    BRIDGE_BIND_NATIVE(file_exist);
    BRIDGE_BIND_NATIVE(file_is_file);
    BRIDGE_BIND_NATIVE(file_is_directory);
    BRIDGE_BIND_NATIVE(file_create_directory);
    //
    Bridge_register(bridge, ALIAS_FILE);
}
