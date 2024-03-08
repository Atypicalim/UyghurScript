// global

#include "../uyghur/uyghur.c"

void oqush(Bridge *bridge)
{
    char *r = system_read_terminal();
    Bridge_returnString(bridge, r);
    pct_free(r);
}

void yezish(Bridge *bridge)
{
    Value *v = Bridge_nextValue(bridge);
    while (v->type != UG_RTYPE_NIL)
    {
        system_write_terminal(Value_toString(v));
        v = Bridge_nextValue(bridge);
    }
    Bridge_returnEmpty(bridge);
}

void izlash(Bridge *bridge)
{
    char *msg = "izlash";
    if (Bridge_topType(bridge) == UG_RTYPE_STR) msg = Bridge_receiveString(bridge);
    printf("\n[%s] %s\n", LANG_LOG, msg);
    Debug_writeTrace(bridge->uyghur->debug);
    Bridge_returnEmpty(bridge);
}

void ekirish(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    Uyghur *uyghur = bridge->uyghur;
    Value *box = Container_get(uyghur->executer->globalContainer, path);
    if (box == NULL) box = Uyghur_runPath(uyghur, path);
    if (box == NULL) box = Value_newEmpty(NULL);
    Bridge_startResult(bridge);
    Bridge_pushValue(bridge, box);
    Bridge_return(bridge);
}

void tazilash(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    Container_del(bridge->uyghur->executer->globalContainer, path);
    Bridge_returnEmpty(bridge);
}

void lib_global_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    // variables
    Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    Bridge_bindNumber(bridge, "NESHIR_NUMIRI", UG_VERSION_CODE);
    // console
    Bridge_bindNative(bridge, "oqush", oqush);
    Bridge_bindNative(bridge, "yezish", yezish);
    Bridge_bindNative(bridge, "izlash", izlash);
    // module
    Bridge_bindNative(bridge, "ekirish", ekirish);
    Bridge_bindNative(bridge, "tazilash", tazilash);
    //
    Bridge_register(bridge);
}
