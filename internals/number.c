// number

#include "../uyghur/uyghur.c"

void ug_number_ceil(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    int r = ceil(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_floor(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    int r = floor(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_round(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    int r = round(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_seed(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    srand(num >= 0 ? num : time(NULL));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_number_random(Bridge *bridge)
{
    double from = Bridge_nextNumber(bridge);
    double to = Bridge_nextNumber(bridge);
    double r = num_random(from, to);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_radian(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = num * M_PI / 180.0;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_degree(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = num * 180.0 / M_PI;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_sin(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = sin(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_cos(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = cos(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_tan(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = tan(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_asin(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = asin(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_acos(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = acos(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_atan(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = atan(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_power(Bridge *bridge)
{
    double x = Bridge_nextNumber(bridge);
    double y = Bridge_nextNumber(bridge);
    double r = pow(x, y);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_square(Bridge *bridge)
{
    double x = Bridge_nextNumber(bridge);
    double y = Bridge_nextNumber(bridge);
    double r = pow(x, 1 / y);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_log_e(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = log(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_log_10(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = log10(num);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_quotient(Bridge *bridge)
{
    double x = Bridge_nextNumber(bridge);
    double y = Bridge_nextNumber(bridge);
    double r = (x - fmod(x, y)) / y;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_reminder(Bridge *bridge)
{
    double x = Bridge_nextNumber(bridge);
    double y = Bridge_nextNumber(bridge);
    double r = fmod(x, y);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_number_int_part(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = num - modf(num, 0);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}


void ug_number_dec_part(Bridge *bridge)
{
    double num = Bridge_nextNumber(bridge);
    double r = modf(num, 0);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void lib_number_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "san");
    //
    Bridge_pushKey(bridge, "pi");
    Bridge_pushNumber(bridge, acos(-1));
    Bridge_pushKey(bridge, "e");
    Bridge_pushNumber(bridge, exp(1));
    //
    Bridge_pushKey(bridge, "ustigePutunlesh");
    Bridge_pushFunction(bridge, ug_number_ceil);
    Bridge_pushKey(bridge, "astighaPutunlesh");
    Bridge_pushFunction(bridge, ug_number_floor);
    Bridge_pushKey(bridge, "texminiyPutunlesh");
    Bridge_pushFunction(bridge, ug_number_round);
    //
    Bridge_pushKey(bridge, "ixtiyariyUruqBikitish");
    Bridge_pushFunction(bridge, ug_number_seed);
    Bridge_pushKey(bridge, "ixtiyariySanHasillash");
    Bridge_pushFunction(bridge, ug_number_random);
    // 
    Bridge_pushKey(bridge, "radianghaAylandurush");
    Bridge_pushFunction(bridge, ug_number_radian);
    Bridge_pushKey(bridge, "gradusqaAylandurush");
    Bridge_pushFunction(bridge, ug_number_degree);
    //
    Bridge_pushKey(bridge, "sinosHisablash");
    Bridge_pushFunction(bridge, ug_number_sin);
    Bridge_pushKey(bridge, "kosinosHisablash");
    Bridge_pushFunction(bridge, ug_number_cos);
    Bridge_pushKey(bridge, "tanginosHisablash");
    Bridge_pushFunction(bridge, ug_number_tan);
    Bridge_pushKey(bridge, "teturSinosHisablash");
    Bridge_pushFunction(bridge, ug_number_asin);
    Bridge_pushKey(bridge, "teturCosinosHisablash");
    Bridge_pushFunction(bridge, ug_number_acos);
    Bridge_pushKey(bridge, "teturTanginosHisablash");
    Bridge_pushFunction(bridge, ug_number_atan);
    //
    Bridge_pushKey(bridge, "derijigeKurutush");
    Bridge_pushFunction(bridge, ug_number_power);
    Bridge_pushKey(bridge, "yiltizdinChiqirish");
    Bridge_pushFunction(bridge, ug_number_square);
    //
    Bridge_pushKey(bridge, "logarifmaEHisablash");
    Bridge_pushFunction(bridge, ug_number_log_e);
    Bridge_pushKey(bridge, "logarifma10Hisablash");
    Bridge_pushFunction(bridge, ug_number_log_10);
    //
    Bridge_pushKey(bridge, "bulunminiHisablash");
    Bridge_pushFunction(bridge, ug_number_quotient);
    Bridge_pushKey(bridge, "qalduqniHisablash");
    Bridge_pushFunction(bridge, ug_number_reminder);
    //
    Bridge_pushKey(bridge, "putunQismighaIrishish");
    Bridge_pushFunction(bridge, ug_number_int_part);
    Bridge_pushKey(bridge, "parcheQismighaIrishish");
    Bridge_pushFunction(bridge, ug_number_dec_part);
    // 
    Bridge_register(bridge);
}
