// objective

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_OBJECTIVE
#define H_UG_OBJECTIVE

Objective *Objective_new(char tp, void *extra)
{
    tools_assert(is_type_objective(tp), "invalid objective type for new");
    Objective *objective = _value_newValueBySize(false, tp, sizeof(Objective));
    objective->map = Hashmap_new(true);
    // log_debug("new-%s: %p %p", get_value_name(tp, "objective"), objective, objective->map);
    Machine_tryLinkForGC(objective->map);
    objective->type = tp;
    objective->extra = extra;
    return objective;
}

Objective *Objective_newCtr()
{
    return Objective_new(UG_TYPE_CTR, NULL);
}

Objective *Objective_newAtr()
{
    return Objective_new(UG_TYPE_ATR, NULL);
}

Objective *Objective_newObj(void *extra)
{
    return Objective_new(UG_TYPE_OBJ, extra);
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
    if (!Objective_isCtr(other) && !Objective_isAtr(other)) return false;
    Queue *parents = this->extra;
    Queue_RESTE(parents);
    Objective *parent = Queue_NEXT(parents);
    while (parent != NULL) {
        if (parent == other) {
            return true;
        }
        parent = Queue_NEXT(parents);
    }
    return false;
}

// 

char *Objective_toString(Objective *this)
{
    char *name = get_value_name(this->type, "objective");
    return tools_format("<%s p:%p>", name, this);
}


#endif
