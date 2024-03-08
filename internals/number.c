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
    Bridge_startBox(bridge, "san");
    //
    Bridge_bindNumber(bridge, "pi", acos(-1));
    Bridge_bindNumber(bridge, "e", exp(1));
    //
    Bridge_bindNative(bridge, "ustigePutunlesh", ug_number_ceil);
    Bridge_bindNative(bridge, "astighaPutunlesh", ug_number_floor);
    Bridge_bindNative(bridge, "texminiyPutunlesh", ug_number_round);
    //
    Bridge_bindNative(bridge, "ixtiyariyUruqBikitish", ug_number_seed);
    Bridge_bindNative(bridge, "ixtiyariySanHasillash", ug_number_random);
    // 
    Bridge_bindNative(bridge, "radianghaAylandurush", ug_number_radian);
    Bridge_bindNative(bridge, "gradusqaAylandurush", ug_number_degree);
    //
    Bridge_bindNative(bridge, "sinosHisablash", ug_number_sin);
    Bridge_bindNative(bridge, "kosinosHisablash", ug_number_cos);
    Bridge_bindNative(bridge, "tanginosHisablash", ug_number_tan);
    Bridge_bindNative(bridge, "teturSinosHisablash", ug_number_asin);
    Bridge_bindNative(bridge, "teturCosinosHisablash", ug_number_acos);
    Bridge_bindNative(bridge, "teturTanginosHisablash", ug_number_atan);
    //
    Bridge_bindNative(bridge, "derijigeKurutush", ug_number_power);
    Bridge_bindNative(bridge, "yiltizdinChiqirish", ug_number_square);
    //
    Bridge_bindNative(bridge, "logarifmaEHisablash", ug_number_log_e);
    Bridge_bindNative(bridge, "logarifma10Hisablash", ug_number_log_10);
    //
    Bridge_bindNative(bridge, "bulunminiHisablash", ug_number_quotient);
    Bridge_bindNative(bridge, "qalduqniHisablash", ug_number_reminder);
    //
    Bridge_bindNative(bridge, "putunQismighaIrishish", ug_number_int_part);
    Bridge_bindNative(bridge, "parcheQismighaIrishish", ug_number_dec_part);
    // 
    Bridge_register(bridge);
}
