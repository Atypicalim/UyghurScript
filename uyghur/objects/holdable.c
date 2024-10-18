// holdable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_HOLDABLE
#define H_UG_HOLDABLE

Holdable *Holdable_new(char tp, void *extra)
{
    tools_assert(is_type_holdable(tp), "invalid holdable type for new");
    return _Dictable_new(tp, NULL, extra);
}

Holdable *Holdable_newModule(char *path)
{
    Holdable *module = Holdable_new(UG_TYPE_MDL, path);
    module->linka = __uyghur->executer->globalScope;
    return module;
}

Holdable *Holdable_newScope(Holdable *parent)
{
    Holdable *scope = Holdable_new(UG_TYPE_SCP, NULL);
    scope->linka = parent;
    return scope;
}

Holdable *Holdable_newKind(CString name)
{
    return Holdable_new(UG_TYPE_KND, name);
}

Holdable *Holdable_newProxy(CString name)
{
    return Holdable_new(UG_TYPE_PXY, name);
}

//

bool Holdable_isModule(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_MDL;
}

bool Holdable_isScope(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_SCP;
}

bool Holdable_isKind(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_KND;
}

bool Holdable_isProxy(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_PXY;
}

// 

bool Holdable_isKindOf(Holdable *this, Value *other) {
    if (!Holdable_isKind(this)) return false;
    Machine *machine = __uyghur->machine;
    if (this == machine->kindLgc && Value_isBoolean(other)) return true;
    if (this == machine->kindNum && Value_isNumber(other)) return true;
    if (this == machine->kindStr && Value_isString(other)) return true;
    if (this == machine->kindList && Value_isListable(other)) return true;
    if (this == machine->kindDict && Value_isDictable(other)) return true;
    return false;
}

// 

char *Holdable_toString(Holdable *this)
{
    char *desc = "?";
    if (Holdable_isKind(this) || Holdable_isProxy(this)) {
        desc = helper_translate_something(this->extra);
    } else if (Holdable_isModule(this)) {
        desc = this->extra;
    }
    char *name = get_value_name(this->type, "holdable");
    return tools_format("<%s %p %s>", name, this, desc);
}

void Holdable_print(Holdable *this)
{
    printf("%s\n", Holdable_toString(this));
    // printf("[V:%s -> %p %p]\n", get_value_name(this->type, "Holdable"), this, this->map);
    // _hashmap_print_with_callback(this->map, "|", _container_key_print_callback);
    // printf("[Holdable]\n");
}


#endif
