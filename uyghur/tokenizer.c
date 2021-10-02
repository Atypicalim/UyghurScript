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
        if (currentChar == '\n')
        {
            this->line++;
            this->column = 1;
            this->position++;
            continue;
        }
        else
        {
            this->column++;
            this->position++;
        }
        printf("--------------%d %d %c\n", this->line, this->line, currentChar);
        // letter
        if (isalpha(currentChar))
        {
            int i = 0;
            char c;
            while(true)
            {
                c = Tokenizer_getchar(this, i);
                if(!isalnum(c)) break;
                printf("->%c:%s\n", c, isalnum(c) ? "YES" : "NO");
                i++;
            }
            Tokenizer_skipN(this, i);
            continue;
        }
        // Tokenizer_parseCharacter(this, currentChar);
    }
    return this->head;
}

void Tokenizer_free(Tokenizer *this)
{
    //
}
