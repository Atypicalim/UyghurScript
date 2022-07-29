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
    char *scope;
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

Tokenizer *Tokenizer_new(Uyghur *uyghur)
{
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->uyghur = uyghur;
    Hashmap *map = Hashmap_new();
    Hashmap_fill(map, TVALUE_WHILE);
    Hashmap_fill(map, TVALUE_IF);
    Hashmap_fill(map, TVALUE_IF_ELSE);
    Hashmap_fill(map, TVALUE_IF_OK);
    Hashmap_fill(map, TVALUE_IF_NO);
    Hashmap_fill(map, TVALUE_CODE_END);
    Hashmap_fill(map, TVALUE_OUTPUT);
    Hashmap_fill(map, TVALUE_INPUT);
    Hashmap_fill(map, TVALUE_DOT);
    Hashmap_fill(map, TVALUE_STR);
    Hashmap_fill(map, TVALUE_NUM);
    Hashmap_fill(map, TVALUE_BOOLEAN);
    Hashmap_fill(map, TVALUE_AND);
    Hashmap_fill(map, TVALUE_OR);
    Hashmap_fill(map, TVALUE_NOT);
    Hashmap_fill(map, TVALUE_ADD);
    Hashmap_fill(map, TVALUE_SUB);
    Hashmap_fill(map, TVALUE_MUL);
    Hashmap_fill(map, TVALUE_DIV);
    Hashmap_fill(map, TVALUE_LESS);
    Hashmap_fill(map, TVALUE_MORE);
    Hashmap_fill(map, TVALUE_CONCAT);
    Hashmap_fill(map, TVALUE_EQUAL);
    Hashmap_fill(map, TVALUE_TARGET_FROM);
    Hashmap_fill(map, TVALUE_TARGET_TO);
    Hashmap_fill(map, TVALUE_VARIABLE);
    Hashmap_fill(map, TVALUE_VALUE);
    Hashmap_fill(map, TVALUE_MADE);
    Hashmap_fill(map, TVALUE_FUNCTION);
    Hashmap_fill(map, TVALUE_CONTENT);
    Hashmap_fill(map, TVALUE_WITH);
    Hashmap_fill(map, TVALUE_CALL);
    Hashmap_fill(map, TVALUE_RETURN);
    Hashmap_fill(map, TVALUE_FURTHER);
    Hashmap_fill(map, TVALUE_RESULT);
    Hashmap_set(map, TVALUE_TRUE, TTYPE_BOOL);
    Hashmap_set(map, TVALUE_FALSE, TTYPE_BOOL);
    Hashmap_set(map, TVALUE_EMPTY, TTYPE_EMPTY);
    Hashmap_set(map, TVALUE_BOX, TTYPE_BOX);
    Hashmap_fill(map, TVALUE_CALCULATOR);
    Hashmap_set(map, TVALUE_SIGN_ADD, TTYPE_CALCULATION);
    Hashmap_set(map, TVALUE_SIGN_SUB, TTYPE_CALCULATION);
    Hashmap_set(map, TVALUE_SIGN_MUL, TTYPE_CALCULATION);
    Hashmap_set(map, TVALUE_SIGN_DIV, TTYPE_CALCULATION);
    Hashmap_set(map, TVALUE_SIGN_POW, TTYPE_CALCULATION);
    Hashmap_set(map, TVALUE_SIGN_PER, TTYPE_CALCULATION);
    Hashmap_fill(map, TVALUE_OPEN);
    Hashmap_fill(map, TVALUE_CLOSE);
    tokenizer->keywordsMap = map;
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

void Tokenizer_addToken(Tokenizer *this, char *type, char *value)
{
    if (type == TTYPE_NAME)
    {
        char *v = Hashmap_get(this->keywordsMap, value);
        if (v != NULL)
        {
            type = is_equal(v, value) ? TTYPE_WORD : v;
        }
    }
    //
    Token *token = NULL;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, value, this->line, this->column, this->path);
    if (this->keyBorder != ' ')
    {
        tools_assert(is_values(type, TTYPES_GROUP_KEYS), "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
        tools_assert(this->scope != NULL, "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
        token = Token_key(value, this->scope, type);
    }
    else
    {
        tools_assert(this->scope == NULL, "%s%s", LANG_ERR_NO_VALID_TOKEN, s);
        token = Token_new(type, value);
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
    char* str = tools_str_apent(tools_str_new("", 0), currentChar, false);
    int i = 0;
    char c;
    bool b = false;
    while (true)
    {
        c = Tokenizer_getchar(this, i);
        if (!isdigit(c) && !(c == '.' && !b)) break;
        if (c == '.') b = true;
        str = tools_str_apent(str, c, false);
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
            char* scope = tools_str_new("", 0);
            int i = 1;
            char c = Tokenizer_getchar(this, i);
            while (c != '{' && c != '[')
            {
                scope = tools_str_apent(scope, c, false);
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
            char* str = tools_str_new("", 0);
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
                str = tools_str_apent(str, c, false);
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
            Tokenizer_addToken(this, TTYPE_NAME, TVALUE_CALCULATOR);
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
                char *str = tools_str_apent(str_new(""), currentChar, false);
                Tokenizer_addToken(this, TTYPE_NAME, str);
                Tokenizer_skipN(this, 1);
                continue;
            }
        }
        // open
        if (currentChar == '(')
        {
            Tokenizer_addToken(this, TTYPE_NAME, TVALUE_OPEN);
            Tokenizer_skipN(this, 1);
            continue;
        }
        // close
        if (currentChar == ')')
        {
            Tokenizer_addToken(this, TTYPE_NAME, TVALUE_CLOSE);
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
            char* str = tools_str_apent(tools_str_new("", 0), currentChar, false);
            int i = 1;
            char c;
            while (true)
            {
                c = Tokenizer_getchar(this, i);
                if (!isalnum(c) && c != '_' && c != '$') break;
                str = tools_str_apent(str, c, false);
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
        Hashmap_free(this->keywordsMap);
        this->keywordsMap = NULL;
    }
    this->uyghur = NULL;
    Tokenizer_reset(this);
    free(this);
}
