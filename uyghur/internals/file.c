// file

#include "../uyghur.c"

void native_file_write(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    CString content = Bridge_receiveString(bridge);
    bool r = file_write(path, content);
    Bridge_returnBoolean(bridge, r);
}

void native_file_read(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    CString r = file_read(path);
    if (r != NULL) {
        Bridge_returnString(bridge, r);
        pct_free(r);
    } else {
        Bridge_returnEmpty(bridge);
    }
}

void native_file_copy(Bridge *bridge)
{
    CString from = Bridge_receiveString(bridge);
    CString to = Bridge_receiveString(bridge);
    bool r = file_copy(from, to);
    Bridge_returnBoolean(bridge, r);
}

void native_file_rename(Bridge *bridge)
{
    CString from = Bridge_receiveString(bridge);
    CString to = Bridge_receiveString(bridge);
    double r = file_rename(from, to);
    Bridge_returnNumber(bridge, r);
}

void native_file_remove(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    double r = file_remove(path);
    Bridge_returnNumber(bridge, r);
}

void native_file_is_exist(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    bool r = file_exist(path);
    Bridge_returnBoolean(bridge, r);
}

void native_file_is_file(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    bool r = file_is_file(path);
    Bridge_returnBoolean(bridge, r);
}

void native_file_is_directory(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    bool r = file_is_directory(path);
    Bridge_returnBoolean(bridge, r);
}

void native_file_create_directory(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    int r = file_create_directory(path);
    Bridge_returnNumber(bridge, r);
}
