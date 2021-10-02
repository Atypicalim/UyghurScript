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
    Token *head;
} Tokenizer;

Tokenizer *Tokenizer_new()
{
    Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
    tokenizer->line = 0;
    tokenizer->column = 0;
    return tokenizer;
}

void Tokenizer_parseCharacter(Tokenizer *this, char character)
{
    printf("%d %d %c\n", this->line, this->column, character);
}

Token *Tokenizer_parseFile(Tokenizer *this, FILE *file)
{
    int c;
    while ((c = getc(file)) != EOF)
    {
        if (c == '\n')
        {
            this->line++;
            this->column = 0;
        }
        else
        {
            this->column++;
            Tokenizer_parseCharacter(this, c);
        }
    }
    return this->head;
}

void Tokenizer_free(Tokenizer *this)
{
    //
}
