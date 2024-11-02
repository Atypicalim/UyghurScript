// voyager

#include "../uyghur.c"

// do http get request
void native_voyager_gain(Bridge *bridge)
{
    CString toUrl = Bridge_receiveString(bridge);

    Waitable *task = Waitable_newTask(NULL, "http.get", toUrl, NULL);

    Bridge_returnValue(bridge, task);
}

// do http post request
void native_voyager_give(Bridge *bridge)
{
    CString toUrl = Bridge_receiveString(bridge);
    Value *callback = Bridge_receiveValue(bridge, UG_TYPE_WKR);
    Bridge_returnEmpty(bridge);
}
