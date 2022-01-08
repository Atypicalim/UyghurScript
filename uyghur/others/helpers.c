// helpers

#ifndef H_HELPERS
#define H_HELPERS

#include "header.h"

Queue *S2Q(int num, char *s, ...) {
    Queue *queue = Queue_new();
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
    char *space = str_concat(_space, " | ");
    // TODO
    printf("%s[LEAF => type:%s]\n", space, this->type);
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
    char *space = str_concat(_space, " | ");
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

bool is_values(char *target, int num, char *s, ...)
{
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(target, s)) return true;
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return false;
}

bool is_calculation(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

#endif
