// global

#include "../uyghur.c"

void native_read(Bridge *bridge)
{
    char *r = system_read_terminal();
    Bridge_returnString(bridge, r);
    pct_free(r);
}

void native_write(Bridge *bridge)
{
    Value *v = Bridge_nextValue(bridge);
    while (v != NULL)
    {
        printf("%s ", Value_toString(v));
        v = Bridge_nextValue(bridge);
    }
    Bridge_returnEmpty(bridge);
}

void native_trace(Bridge *bridge)
{
    char *msg = "TRACE";
    if (Bridge_topType(bridge) == UG_TYPE_STR) msg = Bridge_receiveString(bridge);
    printf("\n[%s] %s\n", UG_TAG_LOG, msg);
    Debug_writeTrace(bridge->uyghur->debug);
    Bridge_returnEmpty(bridge);
}

void native_import(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    Uyghur *uyghur = bridge->uyghur;
    Container *container = uyghur->executer->globalScope;
    Value *box = Container_getLocation(container, path);
    if (box == NULL) box = Uyghur_runPath(uyghur, path);
    if (box == NULL) box = Value_newEmpty(NULL);
    Bridge_returnValue(bridge, box);
}

void native_clean(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    Container_delLocation(bridge->uyghur->executer->globalScope, path);
    Bridge_returnEmpty(bridge);
}

void lib_global_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    // variables
    Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    Bridge_bindNumber(bridge, "NESHIR_NUMIRI", UG_VERSION_CODE);
    // console
    BRIDGE_BIND_NATIVE(read);
    BRIDGE_BIND_NATIVE(write);
    BRIDGE_BIND_NATIVE(trace);
    // module
    BRIDGE_BIND_NATIVE(import);
    BRIDGE_BIND_NATIVE(clean);
    //
    Bridge_register(bridge, NULL);
}
