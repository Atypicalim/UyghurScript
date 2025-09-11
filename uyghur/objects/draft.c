// draft

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_DRAFT
#define H_UG_DRAFT

Draft *Draft_new(char type)
{
    Draft *draft = malloc(sizeof(Draft));
    Object_init(draft, PCT_OBJ_DRAFT);
    draft->type = type;
    draft->text = String_new();
    return draft;
}

void Draft_print(Draft *this)
{
    printf("[(DRAFT_START) => address:%d type:%c]\n", this, this->type);
    printf("%s", String_get(this->text));
    printf("[(DRAFT_END) => address:%d]\n", this);
}

CString Draft_getString(Draft *this)
{
    return String_get(this->text);
}

void Draft_pushString(Draft *this, CString text)
{
    String_append(this->text, text);
}

void Draft_pushWord(Draft *this, CString text)
{
    String_append(this->text, text);
    String_append(this->text, " ");
}

void Draft_pushLine(Draft *this, CString text)
{
    String_append(this->text, text);
    String_append(this->text, "\n");
}

void Draft_pushDraft(Draft *this, Draft *that)
{
    String_append(this->text, Draft_getString(that));
}

void Draft_free(Draft *this)
{
    String_free(this->text);
    Object_free(this);
}

#endif
