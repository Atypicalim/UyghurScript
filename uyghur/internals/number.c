// number

#include "../uyghur.c"

double bind_number_ceil(double num)
{
    return ceil(num);
}

double bind_number_floor(double num)
{
    return floor(num);
}

double bind_number_round(double num)
{
    return round(num);
}

void bind_number_seed(double num) {
    srand(num >= 0 ? num : time(NULL));
}

double bind_number_random(double from, double to)
{
    return num_random(from, to);
}

double bind_number_radian(double num) {
    return num * M_PI / 180.0;
}

double bind_number_degree(double num) {
    return num * 180.0 / M_PI;
}

double bind_number_sin(double num) {
    return sin(num);
}

double bind_number_cos(double num) {
    return cos(num);
}

double bind_number_tan(double num) {
    return tan(num);
}

double bind_number_asin(double num) {
    return asin(num);
}

double bind_number_acos(double num) {
    return acos(num);
}

double bind_number_atan(double num) {
    return atan(num);
}

double bind_number_power(double x, double y)
{
    return pow(x, y);
}

double bind_number_square(double x, double y)
{
    return pow(x, 1 / y);
}

double bind_number_log_e(double num) {
    return log(num);
}

double bind_number_log_10(double num) {
    return log10(num);
}

double bind_number_quotient(double x, double y)
{
    return (x - fmod(x, y)) / y;
}

double bind_number_reminder(double x, double y)
{
    return fmod(x, y);
}

double bind_number_int_part(double num) {
    return num - modf(num, 0);
}


double bind_number_dec_part(double num) {
    return modf(num, 0);
}
