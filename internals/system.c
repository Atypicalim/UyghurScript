// system

#include "../uyghur/uyghur.c"

void ug_system_get_name(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_get_name());
    Bridge_return(bridge);
}

void ug_system_exit_program(Bridge *bridge)
{
    int code = 0;
    if (Bridge_topType(bridge) != RTYPE_EMPTY) code = Bridge_nextNumber(bridge);
    system_exit_program(code);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_system_execute_command(Bridge *bridge)
{
    char *command = Bridge_nextString(bridge);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_execute_command(command));
    Bridge_return(bridge);
}

void ug_system_set_env(Bridge *bridge)
{
    char *name = Bridge_nextString(bridge);
    char *value = Bridge_nextString(bridge);
    system_set_env(name, value);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_system_get_env(Bridge *bridge)
{
    char *name = Bridge_nextString(bridge);
    char *value = system_get_env(name);
    Bridge_startResult(bridge);
    if (value != NULL) Bridge_pushString(bridge, value);;
    Bridge_return(bridge);
}

void ug_system_read_terminal(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_read_terminal());
    Bridge_return(bridge);
}

void ug_system_write_terminal(Bridge *bridge)
{
    Value *v = Bridge_nextValue(bridge);
    while (v->type != RTYPE_EMPTY)
    {
        system_write_terminal(Value_toString(v));
        v = Bridge_nextValue(bridge);
    }
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void lib_system_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "sestima");
    //
    Bridge_pushKey(bridge, "isminiElish");
    Bridge_pushFunction(bridge, ug_system_get_name);
    Bridge_pushKey(bridge, "programmidinChikinish");
    Bridge_pushFunction(bridge, ug_system_exit_program);
    Bridge_pushKey(bridge, "ijraQilish");
    Bridge_pushFunction(bridge, ug_system_execute_command);
    Bridge_pushKey(bridge, "muhitQimmitiYezish");
    Bridge_pushFunction(bridge, ug_system_set_env);
    Bridge_pushKey(bridge, "muhitQimmitiOqush");
    Bridge_pushFunction(bridge, ug_system_get_env);
    Bridge_pushKey(bridge, "terminaldinOqush");
    Bridge_pushFunction(bridge, ug_system_read_terminal);
    Bridge_pushKey(bridge, "terminalghaYezish");
    Bridge_pushFunction(bridge, ug_system_write_terminal);
    //
    Bridge_register(bridge);
}
