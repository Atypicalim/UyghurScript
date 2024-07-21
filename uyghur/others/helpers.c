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
    return tp == UG_TYPE_NIL || tp == UG_TYPE_BOL || tp == UG_TYPE_NUM || tp == UG_TYPE_STR;
}

bool is_bridge_type(char tp)
{
    return is_base_type(tp) || tp == UG_TYPE_CNT || tp == UG_TYPE_FUN;
}

bool is_calculator_common(UCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_ALL);
}

bool is_calculation_number(UCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_NUM);
}

bool is_calculation_bool(UCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_BOL);
}

bool is_calculation_string(UCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_STR);
}

bool is_calculation_logicals(UCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_ALL) || is_eq_strings(c, TVAUE_GROUP_CALCULATION_BOL);
}

bool is_calculation_char(UCHAR c)
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

char convert_ttype_to_rtype(char *tType)
{
    if (is_eq_string(tType, UG_TTYPE_EMP)) return UG_TYPE_NIL;
    if (is_eq_string(tType, UG_TTYPE_BOL)) return UG_TYPE_BOL;
    if (is_eq_string(tType, UG_TTYPE_NUM)) return UG_TYPE_NUM;
    if (is_eq_string(tType, UG_TTYPE_STR)) return UG_TYPE_STR;
    if (is_eq_string(tType, UG_TTYPE_NAM)) return UG_TYPE_STR;
    if (is_eq_string(tType, UG_TTYPE_WRD)) return UG_TYPE_STR;
    tools_error("invalid token type for convert %s", tType);
    return UG_TYPE_NON;
}

char *convert_string_to_location(char *str, char rType)
{
    if (rType == UG_TYPE_NUM) {
        return tools_number_to_string(atof(str));
    } else if (rType == UG_TYPE_STR) {
        return tools_format("%c%s", UG_TYPE_NON, str);
    } else {
        return tools_format("%s", str);
    }
}

void Object_initByType(char type, void *object)
{
    //
}

void Object_freeByType(char type, void *object)
{
    if (type == PCT_OBJ_VALUE) return Value_free(object);
    if (type == PCT_OBJ_CNTNR) return Container_free(object);
    if (type == PCT_OBJ_TOKEN) return Token_free(object);
    if (type == PCT_OBJ_LEAF) return Leaf_free(object);
    pct_object_free_by_type(type, object);
}

void Object_printByType(char type, void *object)
{
    if (type == PCT_OBJ_VALUE) return Value_print(object);
}

// 

bool helper_token_is_types_list(Token *token, int num, char *s, va_list valist)
{
    if (token == NULL) return false;
    bool isMatch = false;
    int i;
    for (i = 0; i < num; i++)
    {
        if (is_eq_string(token->type, s))
        {
            isMatch = true;
            break;
        }
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return isMatch;
}

bool helper_token_is_types(Token *token, int num, char *s, ...)
{
    va_list valist;
    va_start(valist, s);
    return helper_token_is_types_list(token, num, s, valist);
}


bool helper_token_is_values_list(Token *token, int num, char *s, va_list valist)
{
    if (token == NULL) return false;
    bool isMatch = false;
    int i;
    for (i = 0; i < num; i++)
    {
        if (is_eq_string(token->value, s))
        {
            isMatch = true;
            break;
        }
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return isMatch;
}

bool helper_token_is_values(Token *token, int num, char *s, ...)
{
    va_list valist;
    va_start(valist, s);
    return helper_token_is_values_list(token, num, s, valist);
}

// printing

typedef void (*HASHKEY_PRINT_CALLBACK)(Hashkey *, char *);

void _container_key_print_callback(Hashkey *this, char *prefix) {
    printf("%s %s -> %s \n", prefix, this->key->data, Value_toString(this->value));
}

void _hashmap_key_print_callback(Hashkey *this, char *prefix) {
    printf("%s %s -> %i \n", prefix, this->key->data, this->value);
}

void _hashmap_print_with_callback(Hashmap *this, char *prefix, HASHKEY_PRINT_CALLBACK callback)
{
    if (prefix == NULL) prefix = "";
    Hashkey *ptr;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = this[i].position;
        while (ptr != NULL) {
            callback(ptr, prefix);
            ptr = ptr->next;
        }
    }
}

void _array_print_without_callback(Array *this, char *prefix)
{
    if (prefix == NULL) prefix = "";
    for (int i = 0; i < this->length; i++) {
        void *element = this->elements[i];
        printf("%s - %d => %i \n", prefix, i, element);
    } 
}

void Array_print(Array *this)
{
    printf("[Array -> p:%p l:%i]\n", this, this->length);
    _array_print_without_callback(this, "");
    printf("[Array]\n");
}

void Hashmap_print(Hashmap *this)
{
    printf("[Hashmap -> p:%p s:%i]\n", this, this->size);
    _hashmap_print_with_callback(this, "", _hashmap_key_print_callback);
    printf("[Hashmap]\n");
}

void Container_print(Container *this)
{
    printf("[Container -> p:%p t:%c]\n", this, this->type);
    _hashmap_print_with_callback(this->map, "|", _container_key_print_callback);
    printf("[Container]\n");
}

void utils_set_languages(Uyghur *uyghur, char *tp) {
    if (!!uyghur->language) {
        return;
    }
    uyghur->language = tp;
    log_warn("helper.laguages:");
    int size = languages_get_size(tp);
    PAIR_LANGUAGES* pairs = languages_get_conf(tp);
    for (size_t i = 0; i < size; i++)
    {
        PAIR_LANGUAGES pair = pairs[i];
        log_debug("helper.lang %s", pair.val);
        *pair.key = pair.val;
    }
}

void utils_add_languages(Uyghur *uyghur, char *tp) {
    Hashmap *lettersMap = uyghur->lettersMap;
    Hashmap *wordsMap = uyghur->wordsMap;
    // letters
    size_t aSize = sizeof(UG_LETTERS_MAP) / sizeof(UG_LETTERS_MAP[0]);
    for (size_t i = 0; i < aSize; i++) {
        char *key = (char *)UG_LETTERS_MAP[i].key;
        char *val = (char *)UG_LETTERS_MAP[i].val;
        Hashmap_set(lettersMap, key, String_format(val));
    }
    // hashmap
    size_t wSize = sizeof(UG_WORDS_MAP) / sizeof(UG_WORDS_MAP[0]);
    for (size_t i = 0; i < wSize; i++) {
        char *key = (char *)UG_WORDS_MAP[i].key;
        char *val = (char *)UG_WORDS_MAP[i].val;
        if (val == NULL) val = key;
        Hashmap_set(wordsMap, key, String_format(val));
    }
    // 
    log_warn("helper.letters:");
    int sizeLetters = letters_get_size(tp);
    PAIR_LETTERS* pairLetters = letters_get_conf(tp);
    for (size_t i = 0; i < sizeLetters; i++)
    {
        PAIR_LETTERS pair = pairLetters[i];
        log_debug("helper.lang %s %s", pair.key, pair.val);
        Hashmap_set(lettersMap, pair.val, String_format(pair.key));
    }
}

utils_add_aliases(Uyghur *uyghur) {
    void *aliasesMap = uyghur->aliasesMap;
    //
    for (size_t i = 0; i < UG_LANGUAGE_COUNT; i++)
    {
        char *lang = UG_LANGUAGE_ARRAY[i];
        log_warn("helper.alieases: %s", lang);
        //
        int sizeAliases = aliases_get_size(lang);
        PAIR_ALIASES* pairAliases = aliases_get_conf(lang);
        for (size_t i = 0; i < sizeAliases; i++)
        {
            PAIR_ALIASES pair = pairAliases[i];
            log_debug("helper.lang %s %s", pair.key, pair.val);
            Hashmap_set(aliasesMap, pair.val, String_format(pair.key));
        }
    }
}

helper_set_aliased_key(Uyghur *uyghur, Container *container, char *_key, Value *value) {
    //
    // log_warn("helper.aliased: %s", _key);
    char *location = convert_string_to_location(_key, UG_TYPE_STR);
    Container_setLocation(container, location, value);
    pct_free(location);
    //
    Hashmap *aliasesMap = uyghur->aliasesMap;
    Hashkey *ptr;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = aliasesMap[i].position;
        while (ptr != NULL) {
            char *alias = String_get(ptr->key);
            char *key = String_get(ptr->value);
            // log_debug("-- %s %s %s", _key, key, alias);
            if (is_eq_string(_key, key)) {
                // log_debug("helper.lang %s", alias);
                char *location = convert_string_to_location(alias, UG_TYPE_STR);
                Container_setLocation(container, location, value);
                pct_free(location);
            }
            ptr = ptr->next;
        }
    }
}

#endif
