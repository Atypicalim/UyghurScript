// objective

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_OBJECTIVE
#define H_UG_OBJECTIVE

Objective *Objective_new(char tp, void *extra)
{
    tools_assert(is_type_objective(tp), "invalid objective type for new");
    return _Dictable_new(tp, extra);
}

Objective *Objective_newCtr(Token *name)
{
    return Objective_new(UG_TYPE_CTR, name);
}

Objective *Objective_newAtr(Token *name)
{
    return Objective_new(UG_TYPE_ATR, name);
}

Objective *Objective_newObj(Queue *parents)
{
    return Objective_new(UG_TYPE_OBJ, parents);
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
    Token *token = NULL;
    if (Objective_isCtr(this) || Objective_isAtr(this)) {
        token = this->extra;
    } else if (Objective_isObj(this)) {
        Queue *parents = this->extra;
        Queue_RESTE(parents);
        Objective *parent = Queue_NEXT(parents);
        if (parent != NULL) token = parent->extra;
    }
    char *name = get_value_name(this->type, "objective");
    char *desc = token != NULL ? token->value : "?";
    return tools_format("<%s %p %s>", name, this, desc);
}

void Objective_print(Objective *this)
{
    printf("%s\n", Objective_toString(this));
    // printf("[V:%s -> %p %p]\n", get_value_name(this->type, "Objective"), this, this->map);
    // _hashmap_print_with_callback(this->map, "|", _container_key_print_callback);
    // printf("[Objective]\n");
}

#endif
