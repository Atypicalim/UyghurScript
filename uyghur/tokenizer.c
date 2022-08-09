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
    char keyBorder;
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
    this->keyBorder = ' ';
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

void Tokenizer_addToken(Tokenizer *this, char *type, String *value)
{
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
    if (this->keyBorder != ' ')
    {
        tools_assert(is_values(type, TTYPES_GROUP_KEYS), "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
        tools_assert(this->scope != NULL, "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
        token = Token_key(String_get(this->scope), type, String_get(value));
    }
    else
    {
        tools_assert(this->scope == NULL, "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
        token = Token_new(type, String_get(value));
    }
    Token_bindInfo(token, this->path, this->line, this->column);
    this->scope = NULL;
    this->keyBorder = ' ';
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

void Tokenizer_skipBorder(Tokenizer *this)
{
    if (this->scope != NULL && this->keyBorder != ' ') {
        if (Tokenizer_getchar(this, 0) == '}') Tokenizer_skipN(this, 1);
    }
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
    Tokenizer_skipBorder(this);
    Tokenizer_addToken(this, TTYPE_NUMBER, str); // strtod(str, NULL)
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
        if (isspace(currentChar))
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
            String* scope = String_new();
            int i = 1;
            char c = Tokenizer_getchar(this, i);
            while (c != '{' && c != '[')
            {
                String_appendChar(scope, c);
                c = Tokenizer_getchar(this, ++i);
            }
            tools_assert(i > 1, "invalid box name in %s %d %d %c", this->path, this->line, this->column, currentChar);
            this->scope = scope;
            this->keyBorder = c;
            if (c != '[') i++;
            Tokenizer_skipN(this, i);
            continue;
        }

        // string
        if (currentChar == '[')
        {
            String *str = String_new();
            int i = 1;
            char c = Tokenizer_getchar(this, i);
            while (c != ']')
            {
                if (c == '\\')
                {
                    char r = decode_escape(Tokenizer_getchar(this, i + 1));
                    if (r == '\0')
                    {
                        c = r;
                        i++;
                    }
                }
                String_appendChar(str, c);
                c = Tokenizer_getchar(this, ++i);
            }
            Tokenizer_skipN(this, ++i);
            Tokenizer_skipBorder(this);
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
        // letter
        if (isalpha(currentChar) || currentChar == '_'|| currentChar == '$')
        {
            String *str = String_format("%c", currentChar);
            int i = 1;
            char c;
            while (true)
            {
                c = Tokenizer_getchar(this, i);
                if (!isalnum(c) && c != '_' && c != '$') break;
                String_appendChar(str, c);
                i++;
            }
            Tokenizer_skipN(this, i);
            Tokenizer_skipBorder(this);
            Tokenizer_addToken(this, TTYPE_NAME, str);
            continue; 
        }
        // unsupported
        char s[1024];
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
