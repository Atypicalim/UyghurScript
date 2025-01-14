// objective

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_OBJECTIVE
#define H_UG_OBJECTIVE

Objective *Objective_new(char tp, Token *token, void *extra)
{
    tools_assert(is_type_objective(tp), "invalid objective type for new");
    return _Dictable_new(tp, token, extra);
}

Objective *Objective_newCtr(Token *name)
{
    return Objective_new(UG_TYPE_CTR, name, NULL);
}

Objective *Objective_newAtr(Token *name)
{
    return Objective_new(UG_TYPE_ATR, name, NULL);
}

Objective *Objective_newObj(Token *token, Queue *parents)
{
    return Objective_new(UG_TYPE_OBJ, token, parents);
}

// 

bool Objective_isCtr(Objective *this)
{
    return this != NULL && this->type == UG_TYPE_CTR;
}

bool Objective_isAtr(Objective *this)
{
    return this != NULL && this->type == UG_TYPE_ATR;
}

bool Objective_isObj(Objective *this)
{
    return this != NULL && this->type == UG_TYPE_OBJ;
}

// 

bool Objective_isInstanceOf(Objective *this, Value *other) {
    if (!Objective_isObj(this)) return false;
    if (!Objective_isCtr(other) && !Objective_isAtr(other)) return false;
    Objective *_other = other;
    Queue *parents = this->extra;
    Queue_RESTE(parents);
    Objective *parent = Queue_NEXT(parents);
    while (parent != NULL) {
        if (parent == _other) {
            return true;
        }
        parent = Queue_NEXT(parents);
    }
    return false;
}

// 

char *Objective_toString(Objective *this)
{
    return helper_value_to_string(this, "objective", (char *)this->extra);
}

void Objective_print(Objective *this)
{
    printf("%s\n", Objective_toString(this));
}

#endif
