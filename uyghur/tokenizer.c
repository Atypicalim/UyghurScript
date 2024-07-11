// tokenizer

#include "others/header.h"

struct Tokenizer{
    Uyghur *uyghur;
    int line;
    int column;
    int position;
    int length;
    const char *path;
    const char *code;
    Token *head;
    Token *tail;
    Hashmap *aliasMap;
    Hashmap *wordsMap;
};

void Tokenizer_reset(Tokenizer *this)
{
    this->line = 1;
    this->column = 1;
    this->position = 0;
    this->length = 0;
    this->head = NULL;
    this->tail = NULL;
    this->path = NULL;
    this->code = NULL;
}

void _set_keyword(Hashmap *map, char *tvalue, char *ttype)
{
    Hashmap_set(map, tvalue, String_format(ttype));
}

void _fill_keyword(Hashmap *map, char *tvalue)
{
    _set_keyword(map, tvalue, tvalue);
}

Tokenizer *Tokenizer_new(Uyghur *uyghur)
{
    // alias
    Hashmap *aliasMap = Hashmap_new();
    size_t aSize = sizeof(UG_ALIAS_MAP) / sizeof(UG_ALIAS_MAP[0]);
    for (size_t i = 0; i < aSize; i++) {
        char *key = (char *)UG_ALIAS_MAP[i].key;
        char *val = (char *)UG_ALIAS_MAP[i].val;
        Hashmap_set(aliasMap, key, String_format(val));
    }
    // languages
    for (int i = 0; i < YAML_SIZE_TOKENS_UG; i++) {
        UG_PAIRS pair = YAML_MAP_TOKENS_UG[i];
        Hashmap_set(aliasMap, pair.val, String_format(pair.key));
    }
    Hashmap_print(aliasMap);
    // words
    Hashmap *wordsMap = Hashmap_new();
    size_t wSize = sizeof(UG_WORDS_MAP) / sizeof(UG_WORDS_MAP[0]);
    for (size_t i = 0; i < wSize; i++) {
        char *key = (char *)UG_WORDS_MAP[i].key;
        char *val = (char *)UG_WORDS_MAP[i].val;
        if (val == NULL) val = key;
        Hashmap_set(wordsMap, key, String_format(val));
    }
    // init
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->uyghur = uyghur;
    tokenizer->aliasMap = aliasMap;
    tokenizer->wordsMap = wordsMap;
    Tokenizer_reset(tokenizer);
    return tokenizer;
}

Token *Tokenizer_parseLetter(Tokenizer *this, String *letter, bool isGetName)
{   
    // alias 
    String *alias = Hashmap_get(this->aliasMap, String_get(letter));
    if (alias != NULL) {
        // printf("-----------------------\n");
        // String_print(letter);
        // String_print(alias);
        letter = alias;
    }
    // name
    String *val = Hashmap_get(this->wordsMap, String_get(letter));
    if (val == NULL) return Token_new(UG_TTYPE_NAM, String_get(letter));
    // word
    char *type = String_equal(letter, val) ? UG_TTYPE_WRD : String_get(val);
    Token *token = Token_new(type, String_get(letter));
    return token;
}

char Tokenizer_getchar(Tokenizer *this, int indent)
{
    return this->code[this->position + indent];
}

bool Tokenizer_isChar(Tokenizer *this, int indent, char c) {
    return this->code[this->position + indent] == c;
}

// char Tokenizer_getValidChar(Tokenizer *, int)
char Tokenizer_getValidChar(Tokenizer *this, int indent)
{
    char c = this->code[this->position + indent];
    if (isspace(c))
    {
        return Tokenizer_getValidChar(this, indent + (indent < 0 ? -1 : 1));
    }
    return c;
}

char Tokenizer_skipN(Tokenizer *this, int n)
{
    this->column = this->column + n;
    this->position = this->position + n;
}

void Tokenizer_error(Tokenizer *this, char *msg) {
    char *m = msg != NULL ? msg : LANG_ERR_TOKENIZER_EXCEPTION;
    char c = Tokenizer_getchar(this, 0);
    char *s = tools_format(LANG_ERR_SIGN_PLACE, this->path, this->line, this->column, c);
    tools_error("Tokenizer: %s, %s", m, s);
}

void Tokenizer_assert(Tokenizer *this, bool value, char *msg)
{
    if (!value) Tokenizer_error(this, msg);
}

void Token_addToken(Tokenizer *this, Token *token) {
    // printf("addToken:%s->%s\n", token->type, token->value);
    Token_bindInfo(token, this->path, this->line, this->column);
    //
    if (this->head == NULL)
    {
        this->head = token;
        this->tail = token;
    }
    else
    {
        this->tail->next = token;
        token->last = this->tail;
        this->tail = token;
    }
}

void Tokenizer_addLetter(Tokenizer *this, String *value) {
    Token *token = Tokenizer_parseLetter(this, value, true);
    Token_addToken(this, token);
}

String *Tokenizer_readLetter(Tokenizer *this) {
    String *str = String_new();
    char c = Tokenizer_getchar(this, 0);
    char n = Tokenizer_getchar(this, 1);
    //
    Tokenizer_assert(this, is_letter_begin(c, n), LANG_ERR_INVALID_LTTR);
    String_appendChar(str, c);
    Tokenizer_skipN(this, 1);
    c = Tokenizer_getchar(this, 0);
    //
    while (is_letter_body(c)) {
        String_appendChar(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
    }
    return str;
}

String *Tokenizer_readNumber(Tokenizer *this, bool canBeFloat) {
    String *str = String_new();
    char c = Tokenizer_getchar(this, 0);
    //
    if (c == '-' || c == '+') {
        String_appendChar(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
        Tokenizer_assert(this, isdigit(c), LANG_ERR_INVALID_NMBR);
        String_appendChar(str, c);
    } else if (isdigit(c)) {
        String_appendChar(str, '+');
        String_appendChar(str, c);
    } else {
        Tokenizer_error(this, LANG_ERR_INVALID_NMBR);
    }
    // 
    bool hasDot = false;
    Tokenizer_skipN(this, 1);
    c = Tokenizer_getchar(this, 0);
    while (isdigit(c) || (c == '.' && !hasDot)) {
        if (c == '.') {
            Tokenizer_assert(this, canBeFloat, LANG_ERR_CANNOT_BE_FLOAT);
            hasDot = true;
        }
        String_appendChar(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
    }
    //
    return str;
}

String *Tokenizer_readString(Tokenizer *this, bool isContainBorder) {
    if (isContainBorder) {
        char c = Tokenizer_getchar(this, 0);
        Tokenizer_assert(this, is_string_open(c), LANG_ERR_TOKENIZER_STRING_START_ERROR);
        Tokenizer_skipN(this, 1);
    }
    // 
    String *str = String_new();
    char c = Tokenizer_getchar(this, 0);
    while (is_string_body(c)) {
        //
        if (c == '\\') {
            char next = Tokenizer_getchar(this, 1);
            char escp = decode_escape(next);
            if (escp != '\0') {
                c = escp;
                Tokenizer_skipN(this, 1);
            }
        }
        //
        String_appendChar(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
    }
    // 
    if (isContainBorder) {
        char c = Tokenizer_getchar(this, 0);
        Tokenizer_assert(this, is_string_close(c), LANG_ERR_TOKENIZER_STRING_END_ERROR);
        Tokenizer_skipN(this, 1);
    }
    return str;
}

Token *Tokenizer_parseCode(Tokenizer *this, const char *path, const char *code)
{
    Tokenizer_reset(this);
    this->path = path;
    this->code = code;
    this->length = strlen(code);
    int currentChar;
    bool isCalculator = false;
    String *scopeObject = NULL;

    while (this->position < this->length)
    {
        currentChar = Tokenizer_getchar(this, 0);
        // line
        if (currentChar == '\n')
        {
            this->line++;
            this->column = 1;
            this->position++;
            isCalculator = false;
            continue;
        }
        // empty
        if (isspace(currentChar) || iscntrl(currentChar))
        {
            Tokenizer_skipN(this, 1);
            continue;
        }
        // comment
        if (currentChar == '#')
        {
            int i = 1;
            while(Tokenizer_getchar(this, i) != '\n') i++;
            Tokenizer_skipN(this, 1 + i - 1);
            continue;
        }
        // scope
        if (currentChar == '@')
        {
            Tokenizer_skipN(this, 1);
            char c = Tokenizer_getchar(this, 0);
            if (is_scope(c)) {
                Tokenizer_skipN(this, 1);
                scopeObject = String_format("%c", c);
            } else {
                scopeObject = Tokenizer_readLetter(this);
            }
            char t = Tokenizer_getchar(this, 0);
            Tokenizer_assert(this, is_border_open(t), LANG_ERR_TOKENIZER_INVALID_BOX);
            continue;
        }
        // key
        if (scopeObject != NULL) {
            Tokenizer_assert(this, is_border_open(currentChar), LANG_ERR_TOKENIZER_KEY_START_ERROR);
            char openChar = currentChar;
            char closeChar = convert_border_pair(openChar);
            // not empty
            Tokenizer_skipN(this, 1);
            char c = Tokenizer_getchar(this, 0);
            Tokenizer_assert(this, c != closeChar, LANG_ERR_TOKENIZER_KEY_START_ERROR);
            // get key
            String *txt = NULL;
            char *typ = NULL;
            if (openChar == '{') {
                txt = Tokenizer_readLetter(this);
                Token *_tkn = Tokenizer_parseLetter(this, txt, false);
                Tokenizer_assert(this, _tkn == NULL || Token_isName(_tkn), LANG_ERR_TOKENIZER_INVALID_KEY);
                typ = UG_TTYPE_NAM;
            } else if (openChar == '[') {
                txt = Tokenizer_readString(this, false);
                typ = UG_TTYPE_STR;
            } else if (openChar == '(') {
                txt = Tokenizer_readNumber(this, false);
                typ = UG_TTYPE_NUM;
            }
            // normal close
            bool isClosed = Tokenizer_isChar(this, 0, closeChar);
            Tokenizer_assert(this, isClosed, LANG_ERR_TOKENIZER_KEY_END_ERROR);
            Tokenizer_skipN(this, 1);
            // scoped key
            Tokenizer_assert(this, is_eq_strings(typ, TTYPES_GROUP_KEYS), LANG_ERR_INVALID_TYPE);
            Tokenizer_assert(this, scopeObject != NULL, NULL);
            Token *token = Token_key(typ, String_get(txt), String_get(scopeObject));
            scopeObject = NULL;
            // insert token
            Token_addToken(this, token);
            continue;
        }
        // calculator
        if (is_calculator(currentChar))
        {
            Tokenizer_addLetter(this, String_format("%s", TVALUE_CALCULATOR));
            Tokenizer_skipN(this, 1);
            isCalculator = true;
            continue;
        }
        // calculation
        if (isCalculator && is_calculation_char(currentChar))
        {
            char lastC = Tokenizer_getValidChar(this, -1);
            char nextC = Tokenizer_getValidChar(this, 1);
            if (!is_calculation_char(lastC) && !is_calculation_char(nextC) && lastC != '=' && lastC != '(')
            {
                Tokenizer_addLetter(this, String_format("%c", currentChar));
                Tokenizer_skipN(this, 1);
                continue;
            }
        }
        // open
        if (isCalculator && currentChar == '(')
        {
            Tokenizer_addLetter(this, String_format("%s", TVALUE_OPEN));
            Tokenizer_skipN(this, 1);
            continue;
        }
        // close
        if (isCalculator && currentChar == ')')
        {
            Tokenizer_addLetter(this, String_format("%s", TVALUE_CLOSE));
            Tokenizer_skipN(this, 1);
            continue;
        }
        // calculation
        if (is_calculation_logicals(currentChar))
        {
            char lastC = Tokenizer_getchar(this, -1);
            char nextC = Tokenizer_getchar(this, 1);
            Tokenizer_addLetter(this, String_format("%c", currentChar));
            Tokenizer_skipN(this, 1);
            continue;
        }
        // string
        if (is_string_open(currentChar))
        {
            String *str = Tokenizer_readString(this, true);
            Token *tkn = Token_new(UG_TTYPE_STR, String_get(str));
            Token_addToken(this, tkn);
            continue;
        }
        // number
        if (is_number_begin(currentChar, Tokenizer_getchar(this, 1)))
        {
            String *num = Tokenizer_readNumber(this, true);
            Token *tkn = Token_new(UG_TTYPE_NUM, String_get(num));
            Token_addToken(this, tkn);
            continue; 
        }
        // letter
        if (is_letter_begin(currentChar, Tokenizer_getchar(this, 1)))
        {
            String *ltr = Tokenizer_readLetter(this);
            Tokenizer_addLetter(this, ltr);
            continue; 
        }
        // unsupported
        Tokenizer_error(this, LANG_ERR_TOKENIZER_INVALID_SIGN);
        break;
    }
    return this->head;
}

void Tokenizer_free(Tokenizer *this)
{
    //
    Token *current = this->head;
    Token *temp;
    while (current != NULL)
    {
        temp = current->next;
        Token_free(current);
        current = temp;
    }
    if (this->aliasMap != NULL)
    {
        Object_release(this->aliasMap);
        this->aliasMap = NULL;
    }
    if (this->wordsMap != NULL)
    {
        Object_release(this->wordsMap);
        this->wordsMap = NULL;
    }
    this->uyghur = NULL;
    Tokenizer_reset(this);
    free(this);
}
