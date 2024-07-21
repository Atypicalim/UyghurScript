// number

#include "../uyghur/uyghur.c"

void ug_number_ceil(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    int r = ceil(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_floor(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    int r = floor(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_round(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    int r = round(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_seed(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    srand(num >= 0 ? num : time(NULL));
    Bridge_returnEmpty(bridge);
}

void ug_number_random(Bridge *bridge)
{
    double from = Bridge_receiveNumber(bridge);
    double to = Bridge_receiveNumber(bridge);
    double r = num_random(from, to);
    Bridge_returnNumber(bridge, r);
}

void ug_number_radian(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = num * M_PI / 180.0;
    Bridge_returnNumber(bridge, r);
}

void ug_number_degree(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = num * 180.0 / M_PI;
    Bridge_returnNumber(bridge, r);
}

void ug_number_sin(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = sin(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_cos(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = cos(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_tan(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = tan(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_asin(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = asin(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_acos(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = acos(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_atan(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = atan(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_power(Bridge *bridge)
{
    double x = Bridge_receiveNumber(bridge);
    double y = Bridge_receiveNumber(bridge);
    double r = pow(x, y);
    Bridge_returnNumber(bridge, r);
}

void ug_number_square(Bridge *bridge)
{
    double x = Bridge_receiveNumber(bridge);
    double y = Bridge_receiveNumber(bridge);
    double r = pow(x, 1 / y);
    Bridge_returnNumber(bridge, r);
}

void ug_number_log_e(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = log(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_log_10(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = log10(num);
    Bridge_returnNumber(bridge, r);
}

void ug_number_quotient(Bridge *bridge)
{
    double x = Bridge_receiveNumber(bridge);
    double y = Bridge_receiveNumber(bridge);
    double r = (x - fmod(x, y)) / y;
    Bridge_returnNumber(bridge, r);
}

void ug_number_reminder(Bridge *bridge)
{
    double x = Bridge_receiveNumber(bridge);
    double y = Bridge_receiveNumber(bridge);
    double r = fmod(x, y);
    Bridge_returnNumber(bridge, r);
}

void ug_number_int_part(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = num - modf(num, 0);
    Bridge_returnNumber(bridge, r);
}


void ug_number_dec_part(Bridge *bridge)
{
    double num = Bridge_receiveNumber(bridge);
    double r = modf(num, 0);
    Bridge_returnNumber(bridge, r);
}

void lib_number_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    Bridge_bindNumber(bridge, "pi", acos(-1));
    Bridge_bindNumber(bridge, "e", exp(1));
    //
    BRIDGE_BIND_NATIVE(number_ceil);
    BRIDGE_BIND_NATIVE(number_floor);
    BRIDGE_BIND_NATIVE(number_round);
    //
    BRIDGE_BIND_NATIVE(number_seed);
    BRIDGE_BIND_NATIVE(number_random);
    // 
    BRIDGE_BIND_NATIVE(number_radian);
    BRIDGE_BIND_NATIVE(number_degree);
    //
    BRIDGE_BIND_NATIVE(number_sin);
    BRIDGE_BIND_NATIVE(number_cos);
    BRIDGE_BIND_NATIVE(number_tan);
    BRIDGE_BIND_NATIVE(number_asin);
    BRIDGE_BIND_NATIVE(number_acos);
    BRIDGE_BIND_NATIVE(number_atan);
    //
    BRIDGE_BIND_NATIVE(number_power);
    BRIDGE_BIND_NATIVE(number_square);
    //
    BRIDGE_BIND_NATIVE(number_log_e);
    BRIDGE_BIND_NATIVE(number_log_10);
    //
    BRIDGE_BIND_NATIVE(number_quotient);
    BRIDGE_BIND_NATIVE(number_reminder);
    //
    BRIDGE_BIND_NATIVE(number_int_part);
    BRIDGE_BIND_NATIVE(number_dec_part);
    // 
    Bridge_register(bridge, "san");
}
