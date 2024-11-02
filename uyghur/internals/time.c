// time

#include "../uyghur.c"

double bind_time_get_zone()
{
    return time_zone();
}

double bind_time_get_seconds()
{
    return time_second();
}

void native_time_convert_to_seconds(Bridge *bridge)
{
    CString str = Bridge_receiveString(bridge);
    Bridge_returnNumber(bridge, time_convert_to_seconds(str));
}

void native_time_convert_from_seconds(Bridge *bridge)
{
    int seconds = Bridge_receiveNumber(bridge);
    CString str = Bridge_receiveString(bridge);
    CString time = time_convert_from_seconds(seconds, str);
    Bridge_returnString(bridge, time);
    pct_free(time);
}

CString bind_time_get_time()
{
    return time_time();
}

double bind_time_get_clock()
{
    return time_clock();
}

void native_time_sleep_seconds(Bridge *bridge)
{
    sleep(Bridge_receiveNumber(bridge));
    Bridge_returnEmpty(bridge);
}

double _time_delay_call_func(Value *function) {
    Executer *executer = __uyghur->executer;
    if (!Runnable_isWorker(function)) return -1;
    Value *result = Executer_applyWorker(executer, Token_empty(), function, NULL);
    if (!Value_isNumber(result)) return -1;
    double delay = result->number;
    return delay;
}

void native_time_delay_call(Bridge *bridge)
{
    VNumber second = Bridge_receiveValue(bridge, UG_TYPE_NON);
    tools_assert(Value_isNumber(second), "timer time not valid");
    VFunction function = Bridge_receiveValue(bridge, UG_TYPE_NON);
    tools_assert(Runnable_isWorker(function), "timer function not valid");
    Timer *timer = timer_delay(second->number, function, &_time_delay_call_func);
    Bridge_returnEmpty(bridge);
}
