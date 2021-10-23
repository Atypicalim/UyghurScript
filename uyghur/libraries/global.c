// global

#include "../uyghur.c"


void hojjetYezish(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    char *content = Bridge_popString(bridge);
    tools_write_file(path, content);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void hojjetOqush(Bridge *bridge)
{
    char *path = Bridge_popString(bridge);
    char *content = tools_read_file(path);
    Bridge_startResult(bridge);
    if (content != NULL) Bridge_pushString(bridge, content);
    Bridge_return(bridge);
}

void yezish(Bridge *bridge)
{
    if(is_equal(Bridge_topType(bridge), RTYPE_STRING))
        printf("%s", Bridge_popString(bridge));
    else if (is_equal(Bridge_topType(bridge), RTYPE_BOOLEAN))
        printf("%s", Bridge_popBoolean(bridge) ? "true" : "false");
    else if (is_equal(Bridge_topType(bridge), RTYPE_NUMBER))
        printf("%f", Bridge_popNumber(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void oqush(Bridge *bridge)
{
    char line[1024];
    scanf(" %[^\n]", line);
    char *s = tools_format("%s", line);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, s);
    Bridge_return(bridge);
}

void lib_global_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    Bridge_pushKey(bridge, "NESHIR_ISMI");
    Bridge_pushString(bridge, "0.0.1");
    Bridge_pushKey(bridge, "NESHIR_NUMIRI");
    Bridge_pushNumber(bridge, 0.1);
    Bridge_pushKey(bridge, "yezish");
    Bridge_pushFunction(bridge, yezish);
    Bridge_pushKey(bridge, "oqush");
    Bridge_pushFunction(bridge, oqush);
    Bridge_pushKey(bridge, "hojjetYezish");
    Bridge_pushFunction(bridge, hojjetYezish);
    Bridge_pushKey(bridge, "hojjetOqush");
    Bridge_pushFunction(bridge, hojjetOqush);
    Bridge_register(bridge);
}
