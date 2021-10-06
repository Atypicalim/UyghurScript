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
} Tokenizer;

Tokenizer *Tokenizer_new()
{
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->line = 1;
    tokenizer->column = 1;
    tokenizer->position = 0;
    tokenizer->length = 0;
    tokenizer->head = NULL;
    tokenizer->path = NULL;
    tokenizer->code = NULL;
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

Token *Tokenizer_parseCode(Tokenizer *this, const char *path, const char *code)
{
    this->path = path;
    this->code = code;
    this->length = strlen(code);
    int currentChar;

    while (this->position < this->length)
    {
        currentChar = Tokenizer_getchar(this, 0);
        // printf("--------------%d %d %c %s\n", this->line, this->column, currentChar, isdigit(currentChar) ? "YES" : "NO");
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
            printf("\nSTRING: %s\n", str);
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
            printf("\nNUMBER: %s %f\n", str, strtod(str, NULL));
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
            printf("\nLETTER: %s\n", str);
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
