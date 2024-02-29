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
    Hashmap *keywordsMap;
    String *scope;
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
    this->scope = NULL;
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
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->uyghur = uyghur;
    Hashmap *map = Hashmap_new();
    _fill_keyword(map, TVALUE_WHILE);
    _fill_keyword(map, TVALUE_IF);
    _fill_keyword(map, TVALUE_IF_ELSE);
    _fill_keyword(map, TVALUE_IF_OK);
    _fill_keyword(map, TVALUE_IF_NO);
    _fill_keyword(map, TVALUE_CODE_END);
    _fill_keyword(map, TVALUE_OUTPUT);
    _fill_keyword(map, TVALUE_INPUT);
    _fill_keyword(map, TVALUE_DOT);
    _fill_keyword(map, TVALUE_STR);
    _fill_keyword(map, TVALUE_NUM);
    _fill_keyword(map, TVALUE_BOOLEAN);
    _fill_keyword(map, TVALUE_AND);
    _fill_keyword(map, TVALUE_OR);
    _fill_keyword(map, TVALUE_NOT);
    _fill_keyword(map, TVALUE_ADD);
    _fill_keyword(map, TVALUE_SUB);
    _fill_keyword(map, TVALUE_MUL);
    _fill_keyword(map, TVALUE_DIV);
    _fill_keyword(map, TVALUE_LESS);
    _fill_keyword(map, TVALUE_MORE);
    _fill_keyword(map, TVALUE_CONCAT);
    _fill_keyword(map, TVALUE_EQUAL);
    _fill_keyword(map, TVALUE_TARGET_FROM);
    _fill_keyword(map, TVALUE_TARGET_TO);
    _fill_keyword(map, TVALUE_VARIABLE);
    _fill_keyword(map, TVALUE_VALUE);
    _fill_keyword(map, TVALUE_MADE);
    _fill_keyword(map, TVALUE_FUNCTION);
    _fill_keyword(map, TVALUE_CONTENT);
    _fill_keyword(map, TVALUE_WITH);
    _fill_keyword(map, TVALUE_CALL);
    _fill_keyword(map, TVALUE_RETURN);
    _fill_keyword(map, TVALUE_FURTHER);
    _fill_keyword(map, TVALUE_RESULT);
    _set_keyword(map, TVALUE_TRUE, TTYPE_BOOL);
    _set_keyword(map, TVALUE_FALSE, TTYPE_BOOL);
    _set_keyword(map, TVALUE_EMPTY, TTYPE_EMPTY);
    _set_keyword(map, TVALUE_BOX, TTYPE_BOX);
    _fill_keyword(map, TVALUE_CALCULATOR);
    _set_keyword(map, TVALUE_SIGN_ADD, TTYPE_CALCULATION);
    _set_keyword(map, TVALUE_SIGN_SUB, TTYPE_CALCULATION);
    _set_keyword(map, TVALUE_SIGN_MUL, TTYPE_CALCULATION);
    _set_keyword(map, TVALUE_SIGN_DIV, TTYPE_CALCULATION);
    _set_keyword(map, TVALUE_SIGN_POW, TTYPE_CALCULATION);
    _set_keyword(map, TVALUE_SIGN_PER, TTYPE_CALCULATION);
    _fill_keyword(map, TVALUE_OPEN);
    _fill_keyword(map, TVALUE_CLOSE);
    tokenizer->keywordsMap = map;
    Tokenizer_reset(tokenizer);
    return tokenizer;
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

void Token_insertToken(Tokenizer *this, Token *token) {
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

void Tokenizer_addToken(Tokenizer *this, char *type, String *value)
{
    // printf("addToken:%s->%s\n", type, value->data);
    if (type == TTYPE_NAME)
    {
        String *v = Hashmap_get(this->keywordsMap, String_get(value));
        if (v != NULL)
        {
            type = String_equal(v, value) ? TTYPE_WORD : String_get(v);
        }
    }
    //
    Token *token = NULL;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, String_get(value), this->line, this->column, this->path);
    tools_assert(this->scope == NULL, "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
    token = Token_new(type, String_get(value));
    Token_insertToken(this, token);
}

void Tokenizer_addNumber(Tokenizer *this, char currentChar)
{
    String *str = String_format("%c", currentChar);
    int i = 0;
    char c;
    bool b = false;
    while (true)
    {
        c = Tokenizer_getchar(this, i);
        if (!isdigit(c) && !(c == '.' && !b)) break;
        if (c == '.') b = true;
        String_appendChar(str, c);
        i++;
    }
    Tokenizer_skipN(this, i);
    Tokenizer_addToken(this, TTYPE_NUMBER, str); // strtod(str, NULL)
}

String *Tokenizer_readLetter(Tokenizer *this) {
    String *str = String_new();
    char c = Tokenizer_getchar(this, 0);
    //
    tools_assert(is_letter_begin(c), "invalid letter in %s %d %d %c", this->path, this->line, this->column, c);
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

String *Tokenizer_readWord(Tokenizer *this) {
    String *str = String_new();
    char c = Tokenizer_getchar(this, 0);
    //
    tools_assert(is_word_begin(c), "invalid word in %s %d %d %c", this->path, this->line, this->column, c);
    String_appendChar(str, c);
    Tokenizer_skipN(this, 1);
    c = Tokenizer_getchar(this, 0);
    //
    while (is_word_body(c)) {
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
        tools_assert(isdigit(c), "invalid number in %s %d %d %c", this->path, this->line, this->column, c);
        String_appendChar(str, c);
    } else if (isdigit(c)) {
        String_appendChar(str, '+');
        String_appendChar(str, c);
    } else {
        tools_error("invalid number in %s %d %d %c", this->path, this->line, this->column, c);
    }
    // 
    bool hasDot = false;
    Tokenizer_skipN(this, 1);
    c = Tokenizer_getchar(this, 0);
    while (isdigit(c) || (c == '.' && !hasDot)) {
        if (c == '.') hasDot = true;
        String_appendChar(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
    }
    //
    tools_assert(canBeFloat || !hasDot, "invalid number in %s %d %d %c", this->path, this->line, this->column, c);
    return str;
}

String *Tokenizer_readString(Tokenizer *this) {
    String *str = String_new();
    char c = Tokenizer_getchar(this, 0);
    while (is_string_body(c)) {
        //
        if (c == '\\')
        {
            char next = Tokenizer_getchar(this, 1);
            char escp = decode_escape(next);
            if (escp != '\0')
            {
                c = escp;
                Tokenizer_skipN(this, 1);
            }
        }
        //
        String_appendChar(str, c);
        Tokenizer_skipN(this, 1);
        c = Tokenizer_getchar(this, 0);
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
            String *scope = Tokenizer_readLetter(this);
            this->scope = scope;
            continue;
        }
        // key
        if (is_border_open(currentChar) && this->scope != NULL) {
            char openChar = currentChar;
            char closeChar = convert_border_pair(openChar);
            // not empty
            Tokenizer_skipN(this, 1);
            char c = Tokenizer_getchar(this, 0);
            tools_assert(c != closeChar, "invalid box key start in %s %d %d %c", this->path, this->line, this->column, c);
            // get key
            String *txt = NULL;
            char *typ = NULL;
            if (openChar == '{') {
                tools_assert(is_letter_begin(c), "invalid box letter key in %s %d %d %c", this->path, this->line, this->column, c);
                txt = Tokenizer_readLetter(this);
                typ = TTYPE_NAME;
            } else if (openChar == '[') {
                txt = Tokenizer_readString(this);
                typ = TTYPE_STRING;
            } else if (openChar == '(') {
                tools_assert(c != ')', "invalid box number key in %s %d %d %c", this->path, this->line, this->column, c);
                txt = Tokenizer_readNumber(this, false);
                typ = TTYPE_NUMBER;
            }
            // normal close
            bool isClosed = Tokenizer_isChar(this, 0, closeChar);
            tools_assert(isClosed, "invalid box key close in %s %d %d %c", this->path, this->line, this->column, c);
            Tokenizer_skipN(this, 1);
            // scoped key
            tools_assert(is_values(typ, TTYPES_GROUP_KEYS), "%s", LANG_ERR_NO_VALID_TOKEN);
            tools_assert(this->scope != NULL, "%s", LANG_ERR_NO_VALID_TOKEN);
            Token *token = Token_key(String_get(this->scope), typ, String_get(txt));
            this->scope = NULL;
            // insert token
            Token_insertToken(this, token);
            continue;
        }
        // string
        if (is_string_open(currentChar))
        {
            Tokenizer_skipN(this, 1);
            String *str = Tokenizer_readString(this);
            char c = Tokenizer_getchar(this, 0);
            tools_assert(is_string_close(c), "invalid string close in %s %d %d %c", this->path, this->line, this->column, c);
            Tokenizer_skipN(this, 1);
            Tokenizer_addToken(this, TTYPE_STRING, str);
            continue;
        }
        // calculate
        if (currentChar == '=')
        {
            Tokenizer_addToken(this, TTYPE_NAME, String_format("%s", TVALUE_CALCULATOR));
            Tokenizer_skipN(this, 1);
            isCalculator = true;
            continue;
        }
        if (isCalculator && is_calculation(currentChar))
        {
            char lastC = Tokenizer_getValidChar(this, -1);
            char nextC = Tokenizer_getValidChar(this, 1);
            if (!is_calculation(lastC) && lastC != '=' && lastC != '(' && !is_calculation(nextC))
            {
                String *str = String_format("%c", currentChar);
                Tokenizer_addToken(this, TTYPE_NAME, str);
                Tokenizer_skipN(this, 1);
                continue;
            }
        }
        // open
        if (currentChar == '(')
        {
            Tokenizer_addToken(this, TTYPE_NAME, String_format("%s", TVALUE_OPEN));
            Tokenizer_skipN(this, 1);
            continue;
        }
        // close
        if (currentChar == ')')
        {
            Tokenizer_addToken(this, TTYPE_NAME, String_format("%s", TVALUE_CLOSE));
            Tokenizer_skipN(this, 1);
            continue;
        }
        // number
        if ((currentChar == '-' || currentChar == '+') && isdigit(Tokenizer_getchar(this, 1)))
        {
            Tokenizer_skipN(this, 1);
            Tokenizer_addNumber(this, currentChar);
            continue; 
        }
        else if (isdigit(currentChar))
        {
            Tokenizer_addNumber(this, '+');
            continue; 
        }
        // 
        if (is_word_body(currentChar))
        {
            String *str = Tokenizer_readWord(this);
            Tokenizer_addToken(this, TTYPE_NAME, str);
            continue; 
        }
        // unsupported
        char s[1024];
        printf("--->err!\n");
        sprintf(s, LANG_ERR_NO_VALID_CHAR, this->path, this->line, this->column, currentChar);
        tools_error(s);
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
    if (this->keywordsMap != NULL)
    {
        Object_release(this->keywordsMap);
        this->keywordsMap = NULL;
    }
    this->uyghur = NULL;
    Tokenizer_reset(this);
    free(this);
}
