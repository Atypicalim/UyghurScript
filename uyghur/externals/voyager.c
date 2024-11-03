// voyager

#include "../uyghur.c"

// do http request
void native_voyager_request(Bridge *bridge)
{
    CString toUrl = Bridge_receiveString(bridge);
    CString method = Bridge_receiveString(bridge);

    Waitable *task = Waitable_newTask(NULL, "http.get", toUrl, NULL);

    Bridge_returnValue(bridge, task);
}
