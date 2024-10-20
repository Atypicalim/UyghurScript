// global

#include "../uyghur.c"

// get the value of current language
void native_get_language(Bridge *bridge)
{
    USTRING r = bridge->uyghur->language;
    Bridge_returnString(bridge, r);
}

// get the name of current language
void native_get_name(Bridge *bridge)
{
    USTRING lang = bridge->uyghur->language;
    int size = configs_get_size_by_name(LANG_LANGUAGE_TRANSLATION);
    PAIR_CONFIGS *conf = configs_get_conf_by_name(LANG_LANGUAGE_TRANSLATION);
    USTRING trans = helper_filter_multilang_by_key(lang, size, conf);
    Bridge_returnString(bridge, trans);
}

// input a value from termial
void native_read(Bridge *bridge)
{
    USTRING r = system_scanf();
    Bridge_returnString(bridge, r);
    pct_free(r);
}

// output a value to terminal
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

// print trace of the func call
void native_trace(Bridge *bridge)
{
    USTRING _msg = "TRACE";
    if (Bridge_topType(bridge) == UG_TYPE_STR) {
        USTRING msg = Bridge_receiveString(bridge);
        _msg = msg;
    }
    printf("\n[%s] %s\n", UG_TAG_LOG, _msg);
    Debug_writeTrace(bridge->uyghur->debug);
    Bridge_returnEmpty(bridge);
}

// inport a module and return a box
void native_import(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    Uyghur *uyghur = bridge->uyghur;
    Holdable *global = uyghur->executer->globalScope;
    Value *box = Dictable_getLocation(global, path);
    if (box == NULL) box = Uyghur_runPath(uyghur, path);
    if (box == NULL) box = Value_newEmpty(NULL);
    Bridge_returnValue(bridge, box);
}

// clean a module from module cache
void native_clean(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    Dictable_delLocation(bridge->uyghur->executer->globalScope, path);
    Bridge_returnEmpty(bridge);
}

void native_sweep(Bridge *bridge)
{
    Machine_runGC(__uyghur->machine);
    Bridge_returnEmpty(bridge);
}
