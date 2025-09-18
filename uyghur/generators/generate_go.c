
#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

CString generate_go_variable(Compiler *compiler) {
    return "";
}

void generator_go_register(Compiler *compiler) 
{

    COMPILER_BIND_GENERATE(generate_go_variable);
}

