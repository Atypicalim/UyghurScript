// system

#include "../uyghur/uyghur.c"

void ug_system_get_name(Bridge *bridge)
{
    Bridge_returnString(bridge, system_get_name());
}

void ug_system_exit_program(Bridge *bridge)
{
    int code = 0;
    if (Bridge_topType(bridge) != UG_TYPE_NON) code = Bridge_receiveNumber(bridge);
    system_exit_program(code);
    Bridge_returnEmpty(bridge);
}

void ug_system_execute_command(Bridge *bridge)
{
    char *command = Bridge_receiveString(bridge);
    char *r = system_execute_command(command);
    Bridge_returnString(bridge, r);
    pct_free(r);
}

void ug_system_set_env(Bridge *bridge)
{
    char *name = Bridge_receiveString(bridge);
    char *value = Bridge_receiveString(bridge);
    system_set_env(name, value);
    Bridge_returnEmpty(bridge);
}

void ug_system_get_env(Bridge *bridge)
{
    char *name = Bridge_receiveString(bridge);
    char *value = system_get_env(name);
    Bridge_returnString(bridge, value);
}

void lib_system_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "sestima");
    //
    Bridge_bindNative(bridge, "isminiElish", ug_system_get_name);
    Bridge_bindNative(bridge, "programmidinChikinish", ug_system_exit_program);
    Bridge_bindNative(bridge, "ijraQilish", ug_system_execute_command);
    Bridge_bindNative(bridge, "muhitQimmitiYezish", ug_system_set_env);
    Bridge_bindNative(bridge, "muhitQimmitiOqush", ug_system_get_env);
    //
    Bridge_register(bridge);
}
