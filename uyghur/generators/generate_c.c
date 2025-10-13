
#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

CString c_generate_variable(Compiler *compiler) {
    return "";
}

void generator_c_register(Compiler *compiler) 
{

    COMPILER_BIND_GENERATE(c_generate_variable);
}

