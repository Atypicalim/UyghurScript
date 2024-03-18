// helpers

#ifndef H_HELPERS
#define H_HELPERS

#include "header.h"

Queue *S2Q(int num, char *s, ...) {
    Queue *queue = Queue_new();
    va_list args;
    va_start(args, s);
    int i;
    for (i = 0; i < num; i++)
    {
        Queue_push(queue, s);
       s = va_arg(args, char *);
    }
    va_end(args);
    return queue;
}

char **S2A(int num, char *s, ...)
{
    va_list valist;
    char **arr = malloc((num + 1) * sizeof(char *));
    int i;
    va_start(valist, s);
    char *n = malloc(sizeof(char));
    n[0] = (char)num;;
    arr[0] = n;
    for (i = 0; i < num; i++)
    {
        arr[i + 1] = (char *)malloc((strlen(s) + 1) * sizeof(char));
        strcpy(arr[i + 1], s);
       s = va_arg(valist, char *);
    }
    va_end(valist);
    return arr;
}

// print ast leaf
void helper_print_leaf(Leaf *, char *);
void helper_print_leaf(Leaf *this, char *_space)
{
    String *str = String_format("%s | ", _space);
    char *space =  String_dump(str);
    Object_release(str);
    // TODO
    printf("%s[LEAF => type:%c]\n", space, this->type);
    //
    printf("%s [STACK]\n", space);
    Block *currentT = this->tokens->head;
    while (currentT != NULL)
    {
        Token *token =currentT->data;
        printf("%s | [(TOKEN) => type:%s, value:(%s)]\n", space, token->type, token->value);
        currentT = currentT != this->tokens->tail ? currentT->next : NULL;
    }
    printf("%s [STACK]\n", space);
    // 
    printf("%s [QUEUE]\n", space);
    Block *currentL = this->leafs->head;
    while (currentL != NULL)
    {
        Leaf *l = currentL->data;
        helper_print_leaf(l, space);
        currentL = currentL != this->leafs->tail ? currentL->next : NULL;
    }
    printf("%s [QUEUE]\n", space);
    //
    printf("%s[LEAF]\n", space);
}

void helper_print_tokens(Token *head)
{
    printf("[TOKENS => addr:%d]\n", head);
    Token *token = head;
    while(token != NULL)
    {
        Token_print(token);
        token = token->next;
    }
    printf("[TOKENS]\n", head);
}

void helper_print_btree(Foliage *, char *);
void helper_print_btree(Foliage *root, char *_space)
{
    String *str = String_format("%s | ", _space);
    char *space =  String_dump(str);
    Object_release(str);
    bool isRoot = true;
    Foliage *foliage = root;
    Token *token = NULL;
    while(foliage != NULL)
    {
        printf("%s", space);
        token = foliage->data;
        if (token != NULL)
        {
            Token_print(token);
        }
        else
        {
            if (isRoot)
            {
                printf("[BTREE => addr:%d]\n", root);
                isRoot = false;
            }
            else
            {
                printf("[HOLDER]\n");
            }
        }
        if (foliage->right != NULL)
        { 
            helper_print_btree(foliage->right, space);
        }
        foliage = foliage->left;
    }
    printf("%s[BTREE]\n", space);
}

bool is_base_type(char tp)
{
    return tp == UG_RTYPE_NIL || tp == UG_RTYPE_BOL || tp == UG_RTYPE_NUM || tp == UG_RTYPE_STR;
}

bool is_bridge_type(char tp)
{
    return is_base_type(tp) || tp == UG_RTYPE_CNT || tp == UG_RTYPE_FUN;
}

bool is_calculator_common(char c)
{
    return is_eq_characters(c, TVAUE_GROUP_CALCULATION_ALL);
}

bool is_calculation_number(char c)
{
    return is_eq_characters(c, TVAUE_GROUP_CALCULATION_NUM);
}

bool is_calculation_bool(char c)
{
    return is_eq_characters(c, TVAUE_GROUP_CALCULATION_BOL);
}

bool is_calculation_string(char c)
{
    return is_eq_characters(c, TVAUE_GROUP_CALCULATION_STR);
}

bool is_calculation_logicals(char c)
{
    return is_eq_characters(c, TVAUE_GROUP_CALCULATION_ALL) || is_eq_characters(c, TVAUE_GROUP_CALCULATION_BOL);
}

bool is_calculation_char(char c)
{
    return is_calculator_common(c) || is_calculation_number(c) || is_calculation_bool(c) || is_calculation_string(c);
}

bool is_calculation_str(char *target)
{
    return is_eq_strings(target, TVAUE_GROUP_CALCULATION_ALL)
    || is_eq_strings(target, TVAUE_GROUP_CALCULATION_NUM)
    || is_eq_strings(target, TVAUE_GROUP_CALCULATION_BOL)
    || is_eq_strings(target, TVAUE_GROUP_CALCULATION_STR);
}

bool is_higher_priority_calculation(char *target, char *than)
{
    if (is_eq_strings(target, TVAUE_GROUP_CALCULATION_3) && is_eq_strings(than, TVAUE_GROUP_CALCULATION_2)) return true;
    if (is_eq_strings(target, TVAUE_GROUP_CALCULATION_3) && is_eq_strings(than, TVAUE_GROUP_CALCULATION_1)) return true;
    if (is_eq_strings(target, TVAUE_GROUP_CALCULATION_2) && is_eq_strings(than, TVAUE_GROUP_CALCULATION_1)) return true;
    return false;
}

char *format_token_place(Token *token)
{
    tools_format(LANG_ERR_TOKEN_PLACE, token->file, token->line, token->column, token->value);
}

char *format_some_place(Token *token)
{
    tools_format(LANG_ERR_SOME_PLACE, token->file, token->line, token->column);
}

Value *convert_token_to_value(Token *token)
{
    if (Token_isEmpty(token)) return Value_newEmpty(token);
    if (Token_isBool(token)) return Value_newBoolean(is_eq_string(token->value, TVALUE_TRUE), token);
    if (Token_isNumber(token)) return Value_newNumber(atof(token->value), token);
    if (Token_isString(token)) return Value_newString(String_format("%s", token->value), token);
    return NULL;
}

Value *convert_token_to_key(Token *token)
{
    if (Token_isEmpty(token)) return Value_newEmpty(token);
    if (Token_isBool(token)) return Value_newBoolean(is_eq_string(token->value, TVALUE_TRUE), token);
    if (Token_isNumber(token)) return Value_newNumber(atof(token->value), token);
    if (Token_isString(token)) return Value_newString(String_format("%s", token->value), token);
    if (Token_isName(token)) return Value_newString(String_format("%s", token->value), token);
    if (Token_isKey(token)) return Value_newString(String_format("%s", token->value), token);
    return NULL;
}

void Object_initByType(char type, void *object)
{
    //
}

void Object_freeByType(char type, void *object)
{
    if (type == PCT_OBJ_OBJECT) return Object_free(object);
    if (type == PCT_OBJ_STRING) return String_free(object);
    if (type == PCT_OBJ_ARRAY) return Array_free(object);
    if (type == PCT_OBJ_CURSOR) return Cursor_free(object);
    if (type == PCT_OBJ_STACK) return Stack_free(object);
    if (type == PCT_OBJ_QUEUE) return Queue_free(object);
    if (type == PCT_OBJ_HASHKEY) return Hashkey_free(object);
    if (type == PCT_OBJ_HASHMAP) return Hashmap_free(object);
    if (type == PCT_OBJ_FOLIAGE) return Foliage_free(object);
    if (type == PCT_OBJ_BLOCK) return Block_free(object);
    if (type == PCT_OBJ_VALUE) return Value_free(object);
    if (type == PCT_OBJ_CNTNR) return Container_free(object);
    if (type == PCT_OBJ_TOKEN) return Token_free(object);
    if (type == PCT_OBJ_LEAF) return Leaf_free(object);
}

void Object_printByType(char type, void *object)
{
    if (type == PCT_OBJ_VALUE) return Value_print(object);
}

#endif
