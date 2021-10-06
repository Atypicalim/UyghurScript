// tokenizer

#ifndef HEAD_HEADER
#include "header.h"
#endif

#ifndef HEAD_TOKEN
#include "token.c"
#endif

typedef struct
{
    int line;
    int column;
    int position;
    int length;
    const char *path;
    const char *code;
    Token *head;
    Token *tail;
    Hashmap *keywordsMap;
} Tokenizer;

Tokenizer *Tokenizer_new()
{
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->position = 0;
    tokenizer->length = 0;
    tokenizer->head = NULL;
    tokenizer->tail = NULL;
    tokenizer->path = NULL;
    tokenizer->code = NULL;
    //
    Hashmap *map = Hashmap_new();
    Hashmap_fill(map, TVALUE_IF);
    Hashmap_fill(map, TVALUE_ELSE_IF);
    Hashmap_fill(map, TVALUE_ELSE);
    Hashmap_fill(map, TVALUE_OUTPUT);
    Hashmap_fill(map, TVALUE_INPUT);
    Hashmap_fill(map, TVALUE_EMPTY);
    Hashmap_fill(map, TVALUE_BOX);
    Hashmap_fill(map, TVALUE_DOT);
    Hashmap_fill(map, TVALUE_SOMEVALUE);
    Hashmap_fill(map, TVALUE_SOMETYPE);
    Hashmap_fill(map, TVALUE_STR);
    Hashmap_fill(map, TVALUE_NUM);
    Hashmap_fill(map, TVALUE_TRUE);
    Hashmap_fill(map, TVALUE_FALSE);
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
    Hashmap_fill(map, TVALUE_SCREEN_FROM);
    Hashmap_fill(map, TVALUE_SCREEN_TO);
    Hashmap_fill(map, TVALUE_WHILE);
    Hashmap_fill(map, TVALUE_CODE_START);
    Hashmap_fill(map, TVALUE_CODE_END);
    Hashmap_fill(map, TVALUE_VARIABLE);
    Hashmap_fill(map, TVALUE_VALUE);
    Hashmap_fill(map, TVALUE_MADE);
    Hashmap_fill(map, TVALUE_FUNC);
    Hashmap_fill(map, TVALUE_WITH);
    Hashmap_fill(map, TVALUE_CALL);
    Hashmap_fill(map, TVALUE_RETURN);
    Hashmap_fill(map, TVALUE_FURTHER);
    Hashmap_fill(map, TVALUE_RESULT);
    tokenizer->keywordsMap = map;
    //
    return tokenizer;
}

void Tokenizer_parseCharacter(Tokenizer *this, char character)
{
    printf("%d %d %c\n", this->line, this->column, character);
}

char Tokenizer_getchar(Tokenizer *this, int indent)
{
    return this->code[this->position + indent];
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
        if (v != NULL) type = TTYPE_WORD;
    }
    Token *token = Token_new(this->path, this->line, this->column, type, value);
    if (this->head == NULL)
    {
        this->head = token;
        this->tail = token;
    }
    else
    {
        Token_append(this->tail, token);
        this->tail = token;
    }
}

Token *Tokenizer_parseCode(Tokenizer *this, const char *path, const char *code)
{
    this->path = path;
    this->code = code;
    this->length = strlen(code);
    int currentChar;

    while (this->position < this->length)
    {
        currentChar = Tokenizer_getchar(this, 0);
        // line
        if (currentChar == '\n')
        {
            this->line++;
            this->column = 1;
            this->position++;
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
        // string
        if (currentChar == '[')
        {
            char* str = tools_str_new("", 0);
            int i = 1;
            char c;
            while ((c = Tokenizer_getchar(this, i)) != ']')
            {
                str = tools_str_apent(str, c, false);
                i++;
            }
            Tokenizer_addToken(this, TTYPE_STRING, str);
            Tokenizer_skipN(this, i + 1);
            continue;
        }
        // number
        if (isdigit(currentChar) || currentChar == '-' || currentChar == '+')
        {
            char* str = tools_str_apent(tools_str_new("", 0), currentChar, false);
            int i = 1;
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
            Tokenizer_addToken(this, TTYPE_NUMBER, str); // strtod(str, NULL)
            Tokenizer_skipN(this, i + 1);
            continue; 
        }
        // letter
        if (isalpha(currentChar) || currentChar == '_')
        {
            char* str = tools_str_apent(tools_str_new("", 0), currentChar, false);
            int i = 1;
            char c;
            while (true)
            {
                c = Tokenizer_getchar(this, i);
                if (!isalnum(c)) break;
                str = tools_str_apent(str, c, false);
                i++;
            }
            Tokenizer_addToken(this, TTYPE_NAME, str);
            Tokenizer_skipN(this, i + 1);
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
}
