// helpers

#ifndef H_HELPERS
#define H_HELPERS

#include "header.h"

Queue *S2Q(int num, char *s, ...) {
    Queue *queue = Queue_new(false);
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

bool is_calculator_common(UTFCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_ALL);
}

bool is_calculation_number(UTFCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_NUM);
}

bool is_calculation_bool(UTFCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_BOL);
}

bool is_calculation_string(UTFCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_STR);
}

bool is_calculation_logicals(UTFCHAR c)
{
    return is_eq_strings(c, TVAUE_GROUP_CALCULATION_ALL) || is_eq_strings(c, TVAUE_GROUP_CALCULATION_BOL);
}

bool is_calculation_char(UTFCHAR c)
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

bool is_apply_action(char *v) {
    return is_eq_string(v, LETTER_APPEAL) || is_eq_string(v, LETTER_WORKER) || is_eq_string(v, LETTER_CREATOR) || is_eq_string(v, LETTER_ASSISTER);
}

bool is_command_action(char *value) {
    return value != NULL && (is_eq_string(value, LETTER_CMD_INPUT) || is_eq_string(value, LETTER_CMD_OUTPUT));
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
    if (Token_isBool(token)) return Value_newBoolean(is_eq_string(token->value, LETTER_TRUE), token);
    if (Token_isNumber(token)) return Value_newNumber(tools_string_to_number(token->value), token);
    if (Token_isString(token)) return Value_newString(token->value, token);
    return NULL;
}

char convert_ttype_to_rtype(char *tType)
{
    if (is_eq_string(tType, UG_TTYPE_NIL)) return UG_TYPE_NIL;
    if (is_eq_string(tType, UG_TTYPE_BOL)) return UG_TYPE_BOL;
    if (is_eq_string(tType, UG_TTYPE_NUM)) return UG_TYPE_NUM;
    if (is_eq_string(tType, UG_TTYPE_STR)) return UG_TYPE_STR;
    if (is_eq_string(tType, UG_TTYPE_NAM)) return UG_TYPE_STR;
    if (is_eq_string(tType, UG_TTYPE_WRD)) return UG_TYPE_STR;
    tools_error("invalid token type for convert %s", tType);
    return UG_TYPE_NON;
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
    printf("%s %s -> %p \n", prefix, this->key->data, this->value);
}

void _hashmap_key_print_callback(Hashkey *this, char *prefix) {
    printf("%s %s -> %p \n", prefix, this->key->data, this->value);
}

void _hashmap_print_with_callback(Hashmap *this, char *prefix, HASHKEY_PRINT_CALLBACK callback)
{
    if (prefix == NULL) prefix = "";
    Hashkey *ptr;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = this->bucket[i];
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
        printf("%s - %d => %p \n", prefix, i, element);
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

void Object_initByType(char type, void *object)
{
    //
}

void Object_freeByType(char type, void *object)
{
    if (type == PCT_OBJ_VALUE) return Value_free(object);
    if (type == PCT_OBJ_DRAFT) return Draft_free(object);
    if (type == PCT_OBJ_TOKEN) return Token_free(object);
    if (type == PCT_OBJ_LEAF) return Leaf_free(object);
    pct_object_free(object);
}

void Object_printByType(char type, void *object)
{
    if (type == PCT_OBJ_VALUE) return Value_print(object);
    if (type == PCT_OBJ_DRAFT) return Draft_print(object);
    if (type == PCT_OBJ_TOKEN) return Token_print(object);
    if (type == PCT_OBJ_LEAF) return Leaf_print(object);
    pct_object_print(object);
}

CString helper_read_code_file(CString path)
{
    bool exist = file_exist(path);
    tools_assert(exist, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    char *code = file_read(path);
    tools_assert(code != NULL, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    return code;
}

CString _helper_translate_letter(char *letter, char *lang, char *def) {
    int size = letters_get_size_by_name(letter);
    const PAIR_LETTERS* pairs = letters_get_conf_by_name(letter);
    for (size_t i = 0; i < size; i++) {
        PAIR_LETTERS pair = pairs[i];
        if (pair.key == lang) return pair.val;
    }
    return def;
}

CString helper_translate_letter(char *letter, char *lang) {
    return _helper_translate_letter(letter, lang, NULL);
}

CString _helper_translate_alias(char *alias, char *lang, char *def) {
    int size = aliases_get_size_by_name(alias);
    const PAIR_ALIASES* pairs = aliases_get_conf_by_name(alias);
    for (size_t i = 0; i < size; i++) {
        PAIR_ALIASES pair = pairs[i];
        if (pair.key == lang) return pair.val;
    }
    return def;
}

CString helper_translate_alias(char *alias, char *lang) {
    return _helper_translate_alias(alias, lang, NULL);
}

CString helper_translate_something(char *something) {
    char *_something = NULL;
    char *lang = __uyghur->language;
    if (_something == NULL) _something = _helper_translate_letter(something, lang, NULL);
    if (_something == NULL) _something = _helper_translate_alias(something, lang, NULL);
    if (_something == NULL) _something = something;
    return _something;
}

CString helper_parse_language(char *path) {
    int size = UG_LANGUAGE_COUNT;
    for (size_t i = 0; i < size; i++)
    {
        char *tp = UG_LANGUAGE_ARRAY[i];
        if (pct_cstr_ends_with(path, tp)) {
            return tp;
        }
    }
    return NULL;
}

char *helper_filter_multilang_by_key(char *key, int size, PAIR_CONFIGS *multilangs) {
    for (size_t i = 0; i < size; i++)
    {
        PAIR_CONFIGS pair = multilangs[i];
        if (pct_cstr_ends_with(key, pair.key)) {
            return pair.val;
        }
    }
    return NULL;
}

void helper_set_languages(Uyghur *uyghur, char *tp) {
    if (!!uyghur->language) {
        return;
    }
    uyghur->language = tp;
    log_debug("helper.laguage:%s", tp);
    int size = languages_get_size(tp);
    PAIR_LANGUAGES* pairs = languages_get_conf(tp);
    for (size_t i = 0; i < size; i++)
    {
        PAIR_LANGUAGES pair = pairs[i];
        *pair.key = pair.val;
        // log_debug("helper.lang %s", pair.val);
    }
}

void helper_add_languages(Uyghur *uyghur, char *tp) {
    Hashmap *lettersMap = uyghur->lettersMap;
    Hashmap *aliasesMap = uyghur->aliasesMap;
    Hashmap *langsMap = uyghur->langsMap;
    Hashmap *wordsMap = uyghur->wordsMap;
    // words
    size_t wSize = sizeof(UG_WORDS_MAP) / sizeof(UG_WORDS_MAP[0]);
    for (size_t i = 0; i < wSize; i++) {
        char *key = (char *)UG_WORDS_MAP[i].key;
        char *val = (char *)UG_WORDS_MAP[i].val;
        if (val == NULL) val = key;
        Hashmap_set(wordsMap, key, String_format(val));
    }
    // letters
    int sizeLetters = letters_get_size(tp);
    PAIR_LETTERS* pairLetters = letters_get_conf(tp);
    log_debug("helper.letters:%i", sizeLetters);
    for (size_t i = 0; i < sizeLetters; i++) {
        PAIR_LETTERS pair = pairLetters[i];
        Hashmap_set(lettersMap, pair.val, String_format(pair.key));
    }
    // aliases
    int sizeAliases = aliases_get_size_by_lang(tp);
    PAIR_ALIASES* pairAliases = aliases_get_conf_by_lang(tp);
    log_debug("helper.aliases:%i", sizeAliases);
    for (size_t i = 0; i < sizeAliases; i++) {
        PAIR_ALIASES pair = pairAliases[i];
        Hashmap_set(aliasesMap, pair.val, String_format(pair.key));
    }
    // langs
    int sizeLangs = UG_LANGUAGE_COUNT;
    for (size_t i = 0; i < sizeLangs; i++) {
        char *typeLang = UG_LANGUAGE_ARRAY[i];
        int sizeLetters = letters_get_size(typeLang);
        PAIR_LETTERS* pairLetters = letters_get_conf(typeLang);
        for (size_t i = 0; i < sizeLetters; i++) {
            PAIR_LETTERS pair = pairLetters[i];
            Hashmap_set(langsMap, pair.val, typeLang);
        }
        int sizeAliases = aliases_get_size_by_lang(typeLang);
        PAIR_ALIASES* pairAliases = aliases_get_conf_by_lang(typeLang);
        for (size_t i = 0; i < sizeAliases; i++) {
            PAIR_ALIASES pair = pairAliases[i];
            Hashmap_set(langsMap, pair.val, typeLang);
        }
    }
    //
}

void helper_set_lettered_key(Value *container, char *_key, Value *value) {
    // log_warn("helper.set.lettered: %s", _key);
    // 
    int size = letters_get_size_by_name(_key);
    const PAIR_LETTERS* pairs = letters_get_conf_by_name(_key);
    for (size_t i = 0; i < size; i++)
    {
        PAIR_LETTERS pair = pairs[i];
        Dictable_setLocation(container, pair.val, value);
    }
}

Value *helper_get_lettered_key(Value *container, char *_key) {
    // log_warn("helper.get.lettered: %s", _key);
    int size = letters_get_size_by_name(_key);
    const PAIR_LETTERS* pairs = letters_get_conf_by_name(_key);
    for (size_t i = 0; i < size; i++)
    {
        PAIR_LETTERS pair = pairs[i];
        Value *result = Dictable_getLocation(container, pair.val);
        if (result!= NULL) return result;
    }
    return NULL;
}

void helper_set_aliased_key(Value *container, char *_key, Value *value) {
    // log_warn("helper.set.aliased: %s", _key);
    int size = aliases_get_size_by_name(_key);
    const PAIR_ALIASES* pairs = aliases_get_conf_by_name(_key);
    for (size_t i = 0; i < size; i++)
    {
        PAIR_ALIASES pair = pairs[i];
        Dictable_setLocation(container, pair.val, value);
    }
}

Value *helper_get_aliased_key(Value *container, char *_key) {
    // log_warn("helper.get.aliased: %s", _key);
    int size = aliases_get_size_by_name(_key);
    const PAIR_ALIASES* pairs = aliases_get_conf_by_name(_key);
    for (size_t i = 0; i < size; i++)
    {
        PAIR_ALIASES pair = pairs[i];
        Value *result = Dictable_getLocation(container, pair.val);
        if (result!= NULL) return result;
    }
    return NULL;
}

void *helper_set_proxy_value(char *proxyName, char *key, Value *value) {
    Executer *executer = __uyghur->executer;
    Holdable *global = executer->globalScope;
    Holdable *box = helper_get_aliased_key(global, proxyName);
    helper_set_aliased_key(box, key, value);
}

Value *helper_get_proxy_value(char *proxyName, char *key) {
    Executer *executer = __uyghur->executer;
    Holdable *global = executer->globalScope;
    Holdable *box = helper_get_aliased_key(global, proxyName);
    return helper_get_aliased_key(box, key);
}

char* helper_get_value_name(char tp, char* def) {
    char *name = NULL;
    switch (tp) {
        case UG_TYPE_NUM: name = LETTER_NUM; break;
        case UG_TYPE_STR: name = LETTER_STR; break;
        case UG_TYPE_LST: name = LETTER_LST; break;
        case UG_TYPE_DCT: name = LETTER_DCT; break;
        //
        case UG_TYPE_KND: name = LETTER_KIND;  break;
        case UG_TYPE_PXY: name = LETTER_PROXY;  break;
        case UG_TYPE_SCP: name = LETTER_SCOPE;  break;
        case UG_TYPE_MDL: name = LETTER_MODULE;  break;
        //
        case UG_TYPE_CTR: name = LETTER_CREATOR;  break;
        case UG_TYPE_ATR: name = LETTER_ASSISTER;  break;
        case UG_TYPE_OBJ: name = LETTER_OBJECT;  break;
        //
        case UG_TYPE_NTV: name = LETTER_NATIVE;  break;
        case UG_TYPE_WKR: name = LETTER_WORKER;  break;
        //
        case UG_TYPE_STF: name = LETTER_STUF;  break;
        case UG_TYPE_TSK: name = LETTER_TASK;  break;
        //
        default: name = def;
    }
    char *_name = helper_translate_something(name);
    return _name;
}

char* helper_value_to_string(CPointer target, CString failure, CString extra) {
    if (!target) {
        return tools_format("<null>");
    }
    Value *value = target;
    Token *token = value->token;
    char *desc = token != NULL ? token->value : "?";
    char *name = helper_get_value_name(value->type, failure);
    if (token != NULL && token->line > 0) {
        if (extra) {
            return tools_format("<%s:%p %s %s:%d %s>", name, value, desc, token->file, token->line, extra);
        } else {
            return tools_format("<%s:%p %s %s:%d>", name, value, desc, token->file, token->line);
        }
    } else {
        if (extra) {
            return tools_format("<%s:%p %s %s>", name, value, desc, extra);
        } else {
            return tools_format("<%s:%p %s>", name, value, desc);
        }
    }
}

char*helper_value_as_string(void *target, CString failure) {
    Value *value = target;
    char *content = value == NULL ? tools_format("<null>") : Value_toString(value);
    return content != NULL ? content : helper_value_to_string(value, failure, NULL);
}

#endif
