// system

#include "../uyghur.c"

void ug_system_get_name(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_get_name(str));
    Bridge_return(bridge);
}

void ug_system_exit_program(Bridge *bridge)
{
    system_exit_program(Bridge_popNumber(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_system_execute_command(Bridge *bridge)
{
    char *command = Bridge_popString(bridge);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_execute_command(command));
    Bridge_return(bridge);
}

void ug_system_set_env(Bridge *bridge)
{
    system_set_env(Bridge_popString(bridge), Bridge_popString(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_system_get_env(Bridge *bridge)
{
    char *name = Bridge_popString(bridge);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_get_env(name));
    Bridge_return(bridge);
}

void ug_system_write_terminal(Bridge *bridge)
{
    system_write_terminal(Bridge_popString(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_system_read_terminal(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_read_terminal());
    Bridge_return(bridge);
}


void lib_time_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "sestima");
    //
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_get_name);
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_exit_program);
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_execute_command);
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_set_env);
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_get_env);
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_write_terminal);
    Bridge_pushKey(bridge, "");
    Bridge_pushFunction(bridge, ug_system_read_terminal);
    //
    Bridge_register(bridge);
}
