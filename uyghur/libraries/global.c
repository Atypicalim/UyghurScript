// global

#include "../uyghur.c"

void oqush(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, system_read_terminal());
    Bridge_return(bridge);
}

void yezish(Bridge *bridge)
{
    Value *v = Bridge_popValue(bridge);
    while (v->type != RTYPE_EMPTY)
    {
        system_write_terminal(Value_toString(v));
        v = Bridge_popValue(bridge);
    }
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ekirish(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    Uyghur *uyghur = bridge->uyghur;
    Value *box = Container_get(uyghur->executer->globalContainer, path);
    if (box == NULL) box = Uyghur_execute(uyghur, path, false);
    if (box == NULL) box = Value_newEmpty(NULL);
    Bridge_startResult(bridge);
    Bridge_pushValue(bridge, box);
    Bridge_return(bridge);
}

void tazilash(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    Container_del(bridge->uyghur->executer->globalContainer, path);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void lib_global_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    // variables
    Bridge_pushKey(bridge, "NESHIR_ISMI");
    Bridge_pushString(bridge, UG_VERSION_NAME);
    Bridge_pushKey(bridge, "NESHIR_NUMIRI");
    Bridge_pushNumber(bridge, UG_VERSION_CODE);
    // console
    Bridge_pushKey(bridge, "oqush");
    Bridge_pushFunction(bridge, oqush);
    Bridge_pushKey(bridge, "yezish");
    Bridge_pushFunction(bridge, yezish);
    // module
    Bridge_pushKey(bridge, "ekirish");
    Bridge_pushFunction(bridge, ekirish);
    Bridge_pushKey(bridge, "tazilash");
    Bridge_pushFunction(bridge, tazilash);
    //
    Bridge_register(bridge);
}
