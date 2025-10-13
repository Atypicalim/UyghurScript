
#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

CString convert_number_2_js(Token *token) {
    double numD = tools_string_to_number(token->value);
    int numI = (int)numD;
    if (numD == numI) {
        return tools_format("%d", numI);
    } else {
        return tools_format("%f", numD);
    }
}

CString receive_token_2_js(Compiler *compiler) {
    Token *token = Compiler_popPass(compiler);
    if (!token) {
        return NULL;
    }
    CString value = "undefined";
    if (Token_isEmpty(token)) {
        value = "null";
    } else if (Token_isName(token)) {
        value = token->value;
    } else if (Token_isKey(token)) {
        Token *extra = token->extra;
        if (Token_isKeyOfName(token)) {
            value = tools_format("%s[%s]", extra->value, token->value);
        } else if (Token_isKeyOfString(token)) {
            value = tools_format("%s.%s", extra->value, token->value);
        } else if (Token_isKeyOfNumber(token)) {
            value = tools_format("%s[%s]", extra->value, convert_number_2_js(token));
        }
    } else if (Token_isWord(token)) {
        if (is_eq_string(token->value, LETTER_BOL)) {
            value = "false";
        } else if (is_eq_string(token->value, LETTER_NUM)) {
            value = "0";
        } else if (is_eq_string(token->value, LETTER_STR)) {
            value = "\"\"";
        } else if (is_eq_string(token->value, LETTER_LST)) {
            value = "[]";
        } else if (is_eq_string(token->value, LETTER_DCT)) {
            value = "{}";
        } else {
            value = token->value;
        }
    } else if (Token_isBool(token)) {
        value = tools_format("%s", is_eq_string(token->value, LETTER_TRUE) ? "true" : "false");
    } else if (Token_isNumber(token)) {
        value = convert_number_2_js(token);
    } else if (Token_isString(token)) {
        value = tools_format("\"%s\"", escape_cstring(token->value));
    } else {
        value = token->value;
    }
    return value;
}

CString receive_args_2_js(Compiler *compiler) {
    String *params = String_new();
    CString arg = receive_token_2_js(compiler);
    while(arg != NULL) {
        String_append(params, arg);
        arg = receive_token_2_js(compiler);
        if (arg != NULL) String_append(params, ", ");
    }
    return String_get(params);
}

CString receive_judge_2_js(Compiler *compiler) {
    Token *judge = Compiler_popPass(compiler);
    CString first = receive_token_2_js(compiler);
    CString clcltn = receive_token_2_js(compiler);
    CString second = receive_token_2_js(compiler);
    bool shouldOk = is_eq_string(judge->value, LETTER_THEN);
    if (shouldOk) {
        return tools_format(!second ? "%s" : "%s %s %s", first, clcltn, second);
    } else {
        return tools_format(!second ? "!%s" : "!(%s %s %s)", first, clcltn, second);
    }
}

//////////////////////////////////////////////////////////////////////////////

CString js_generate_variable(Compiler *compiler) {
    CString name = receive_token_2_js(compiler);
    CString token = receive_token_2_js(compiler);
    return tools_format("var %s = %s", name, token);
}

CString js_generate_command(Compiler *compiler) {
    CString action = receive_token_2_js(compiler);
    CString args = receive_args_2_js(compiler);
    if (is_eq_string(action, LETTER_CMD_OUTPUT)) {
        return tools_format("console.log(%s)", args);
    } else if (is_eq_string(action, LETTER_CMD_INPUT)) {
        return tools_format("var %s = prompt()", args);
    }
}

CString js_generate_if(Compiler *compiler) {
    CString judge = receive_judge_2_js(compiler);
    return tools_format("if(%s)", judge);
}

CString js_generate_spread(Compiler *compiler) {
    CString target = receive_token_2_js(compiler);
    CString iter1 = receive_token_2_js(compiler);
    CString iter2 = receive_token_2_js(compiler);
    return tools_format("for(const %s in %s)", iter1, iter2, target);
}

CString js_generate_while(Compiler *compiler) {
    CString judge = receive_judge_2_js(compiler);
    return tools_format("while(%s)", judge);
}

CString js_generate_calculate(Compiler *compiler) {
    return "xyz = 1 * a";
}

CString js_generate_appliable(Compiler *compiler) {
    return "fun xyz() {}";
}

CString js_generate_result(Compiler *compiler) {
    return "return null";
}

CString js_generate_apply(Compiler *compiler) {
    CString name = receive_token_2_js(compiler);
    CString result = receive_token_2_js(compiler);
    CString args = receive_args_2_js(compiler);
    if (!result || is_eq_string(result, "null")) {
        return tools_format("%s(%s)", name, args);
    } else {
        return tools_format("var %s = %s(%s)", result, name, args);
    }
    
}

CString js_generate_then(Compiler *compiler) {
    return "{";
}

CString js_generate_end(Compiler *compiler) {
    return "}";
}

void generator_js_register(Compiler *compiler) 
{
    COMPILER_BIND_GENERATE(js_generate_variable);
    COMPILER_BIND_GENERATE(js_generate_command);
    COMPILER_BIND_GENERATE(js_generate_if);
    COMPILER_BIND_GENERATE(js_generate_spread);
    COMPILER_BIND_GENERATE(js_generate_while);
    COMPILER_BIND_GENERATE(js_generate_calculate);
    COMPILER_BIND_GENERATE(js_generate_appliable);
    COMPILER_BIND_GENERATE(js_generate_result);
    COMPILER_BIND_GENERATE(js_generate_apply);
    COMPILER_BIND_GENERATE(js_generate_then);
    COMPILER_BIND_GENERATE(js_generate_end);
}
