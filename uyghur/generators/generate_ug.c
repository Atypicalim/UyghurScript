
#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

//////////////////////////////////////////////////////////////////////////////

CString _ug_generate_translate(CString name) {
    CString translation = helper_translate_letter(name, __uyghur->compiler->dialect);
    return translation == NULL ? name : translation;
}

#define T _ug_generate_translate

//////////////////////////////////////////////////////////////////////////////

CString convert_number_2_ug(Token *token) {
    double numD = tools_string_to_number(token->value);
    int numI = (int)numD;
    if (numD == numI) {
        return tools_format("%d", numI);
    } else {
        return tools_format("%f", numD);
    }
}

CString receive_string_2_ug(Compiler *compiler) {
    CString value = Compiler_popPass(compiler);
    tools_assert(value != NULL, "invalid pass for generate");
    return  value;
}

CString receive_token_2_ug(Compiler *compiler) {
    Token *token = Compiler_popPass(compiler);
    if (!token) {
        return NULL;
    }
    CString value = T(LETTER_NIL);
    if (Token_isEmpty(token)) {
        value = T(LETTER_NIL);
    } else if (Token_isName(token)) {
        value = token->value;
    } else if (Token_isKey(token)) {
        Token *extra = token->extra;
        if (Token_isKeyOfName(token)) {
            value = tools_format("@%s{%s}", extra->value, token->value);
        } else if (Token_isKeyOfString(token)) {
            value = tools_format("@%s.%s", extra->value, token->value);
        } else if (Token_isKeyOfNumber(token)) {
            value = tools_format("@%s.%s", extra->value, convert_number_2_ug(token));
        }
    } else if (Token_isWord(token)) {
        if (is_eq_string(token->value, LETTER_BOL)) {
            value = T(LETTER_BOL);
        } else if (is_eq_string(token->value, LETTER_NUM)) {
            value = T(LETTER_NUM);
        } else if (is_eq_string(token->value, LETTER_STR)) {
            value = T(LETTER_STR);
        } else if (is_eq_string(token->value, LETTER_LST)) {
            value = T(LETTER_LST);
        } else if (is_eq_string(token->value, LETTER_DCT)) {
            value = T(LETTER_DCT);
        } else {
            value = token->value;
        }
    } else if (Token_isBool(token)) {
        value = tools_format("%s", is_eq_string(token->value, LETTER_TRUE) ? T(LETTER_TRUE) : T(LETTER_FALSE));
    } else if (Token_isNumber(token)) {
        value = convert_number_2_ug(token);
    } else if (Token_isString(token)) {
        value = tools_format("\"%s\"", escape_cstring(token->value));
    } else {
        value = token->value;
    }
    return value;
}

CString receive_args_2_ug(Compiler *compiler) {
    String *params = String_new();
    CString arg = receive_token_2_ug(compiler);
    while(arg != NULL) {
        String_append(params, arg);
        arg = receive_token_2_ug(compiler);
        if (arg != NULL) String_append(params, " ");
    }
    return String_get(params);
}

CString receive_judge_2_ug(Compiler *compiler) {
    CString first = receive_token_2_ug(compiler);
    CString clcltn = receive_token_2_ug(compiler);
    CString second = receive_token_2_ug(compiler);
    return tools_format(second == NULL ? "%s" : "%s %s %s", first, clcltn, second);
}

//////////////////////////////////////////////////////////////////////////////

CString ug_generate_variable(Compiler *compiler) {
    CString name = receive_token_2_ug(compiler);
    CString token = receive_token_2_ug(compiler);
    return tools_format("%s %s %s %s %s", T(LETTER_VARIABLE), name, T(LETTER_VALUE), token, T(LETTER_MADE));
}

CString ug_generate_command(Compiler *compiler) {
    CString action = receive_token_2_ug(compiler);
    CString args = receive_args_2_ug(compiler);
    if (is_eq_string(action, LETTER_CMD_OUTPUT)) {
        return tools_format("%s %s %s", T(LETTER_COMMAND), args, T(LETTER_CMD_OUTPUT));
    } else if (is_eq_string(action, LETTER_CMD_INPUT)) {
        return tools_format("%s %s %s", T(LETTER_COMMAND), args, T(LETTER_CMD_INPUT));
    }
}

CString ug_generate_if(Compiler *compiler) {
    CString _judge = receive_string_2_ug(compiler);
    CString judge = receive_judge_2_ug(compiler);
    return tools_format("%s %s %s", T(LETTER_IF), judge, T(_judge));
}

CString ug_generate_elif(Compiler *compiler) {
    CString _judge = receive_string_2_ug(compiler);
    CString judge = receive_judge_2_ug(compiler);
    return tools_format("%s %s %s", T(LETTER_ELIF), judge, T(_judge));
}

CString ug_generate_else(Compiler *compiler) {
    return tools_format("%s ", T(LETTER_ELSE));
}

CString ug_generate_spread(Compiler *compiler) {
    CString target = receive_token_2_ug(compiler);
    CString iter1 = receive_token_2_ug(compiler);
    CString iter2 = receive_token_2_ug(compiler);
    return tools_format("%s %s %s %s %s %s", T(LETTER_SPREAD), target, T(LETTER_VARIABLE), iter1, iter2, T(LETTER_BECOME));
}

CString ug_generate_while(Compiler *compiler) {
    CString _judge = receive_string_2_ug(compiler);
    CString judge = receive_judge_2_ug(compiler);
    return tools_format("%s %s %s", T(LETTER_WHILE), judge, T(_judge));
}

CString ug_generate_calculate(Compiler *compiler) {
    return "xyz = 1 * a";
}

CString ug_generate_appliable(Compiler *compiler) {
    CString _type = receive_token_2_ug(compiler);
    CString name = receive_token_2_ug(compiler);
    CString args = receive_args_2_ug(compiler);
    return tools_format("%s %s %s %s %s", T(_type), name, T(LETTER_VARIABLE), args, T(LETTER_CONTENT));
}

CString ug_generate_result(Compiler *compiler) {
    return "return null;";
}

CString ug_generate_apply(Compiler *compiler) {
    CString flag = receive_string_2_ug(compiler);
    CString name = receive_token_2_ug(compiler);
    CString result = receive_token_2_ug(compiler);
    CString args = receive_args_2_ug(compiler);
    bool isExpanded = is_eq_string(flag, LETTER_TRUE);
    bool withResult = result != NULL && !is_eq_string(result, T(LETTER_NIL));
    bool withArgs = args != NULL && !is_eq_string(args, "");
    //
    if (withResult && withArgs) {
        if (isExpanded) {
            return tools_format(
                "%s %s %s %s %s %s %s %s",
                T(LETTER_APPEAL), name, T(LETTER_WITH), args, T(LETTER_APPLY),
                T(LETTER_FURTHER), result, T(LETTER_RECIEVED)
            );
        } else {
            return tools_format("%s = %s(%s)", result, name, args);
        }
    } else if (withResult) {
        if (isExpanded) {
            return tools_format(
                "%s %s %s %s %s %s",
                T(LETTER_APPEAL), name, T(LETTER_APPLY),
                T(LETTER_FURTHER), result, T(LETTER_RECIEVED)
            );
        } else {
            return tools_format("%s = %s()", result, name);
        }
    } else if (withArgs) {
        if (isExpanded) {
            return tools_format(
                "%s %s %s %s %s",
                T(LETTER_APPEAL), name, T(LETTER_WITH), args, T(LETTER_APPLY)
            );
        } else {
            return tools_format("%s(%s)", name, args);
        }
    } else {
        if (isExpanded) {
            return tools_format("%s %s %s", T(LETTER_APPEAL), name, T(LETTER_APPLY));
        } else {
            return tools_format("%s()", name);
        }
    }
}

CString ug_generate_then(Compiler *compiler) {
    return T(LETTER_THEN);
}

CString ug_generate_end(Compiler *compiler) {
    return T(LETTER_END);
}

void generator_ug_register(Compiler *compiler) 
{
    COMPILER_BIND_GENERATE(ug_generate_variable);
    COMPILER_BIND_GENERATE(ug_generate_command);
    COMPILER_BIND_GENERATE(ug_generate_if);
    COMPILER_BIND_GENERATE(ug_generate_elif);
    COMPILER_BIND_GENERATE(ug_generate_else);
    COMPILER_BIND_GENERATE(ug_generate_spread);
    COMPILER_BIND_GENERATE(ug_generate_while);
    COMPILER_BIND_GENERATE(ug_generate_calculate);
    COMPILER_BIND_GENERATE(ug_generate_appliable);
    COMPILER_BIND_GENERATE(ug_generate_result);
    COMPILER_BIND_GENERATE(ug_generate_apply);
    COMPILER_BIND_GENERATE(ug_generate_then);
    COMPILER_BIND_GENERATE(ug_generate_end);
}

#undef TRANSLATE
