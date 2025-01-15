// system

#include "../uyghur.c"

void native_system_get_name(Bridge *bridge)
{
    CString value = tools_get_env(IS_WINDOWS ? "USERNAME" : "USER");
    Bridge_returnString(bridge, value);
}

void native_system_get_type(Bridge *bridge)
{
    CString value = PLATFORM_NAME;
    Bridge_returnString(bridge, value);
}

void native_system_exit_program(Bridge *bridge)
{
    int _code = 0;
    if (Bridge_topType(bridge) != UG_TYPE_NON) {
        int code = Bridge_receiveNumber(bridge);
        _code = code;
    }
    exit(_code);
    Bridge_returnEmpty(bridge);
}

void native_system_execute_command(Bridge *bridge)
{
    CString command = Bridge_receiveString(bridge);
    CString r = system_execute(command);
    Bridge_returnString(bridge, r);
    pct_free(r);
}

void native_system_set_env(Bridge *bridge)
{
    CString name = Bridge_receiveString(bridge);
    CString value = Bridge_receiveString(bridge);
    tools_set_env(name, value);
    Bridge_returnEmpty(bridge);
}

void native_system_get_env(Bridge *bridge)
{
    CString name = Bridge_receiveString(bridge);
    CString value = tools_get_env(name);
    Bridge_returnString(bridge, value);
}
