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
};

void Tokenizer_reset(Tokenizer *this)
{
    this->uyghur = NULL;
    this->line = 1;
    this->column = 1;
    this->position = 0;
    this->length = 0;
    this->head = NULL;
    this->tail = NULL;
    this->path = NULL;
    this->code = NULL;
    this->keywordsMap = NULL;
}

Tokenizer *Tokenizer_new(Uyghur *uyghur)
{
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    Tokenizer_reset(tokenizer);
    //
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
        if (v != NULL)
        {
            type = is_equal(v, value) ? TTYPE_WORD : v;
        }
    }
    Token *token = Token_new(this->path, this->line, this->column, type, value);
    if (this->head == NULL)
    {
        this->head = token;
        this->tail = token;
    }
    else
    {
        Block_append(this->tail, token);
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
                if (!isalnum(c) && c != '_') break;
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
    }
    Tokenizer_reset(this);
    free(this);
}
