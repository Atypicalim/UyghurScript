// machine

#include "others/header.h"

#include <stdio.h>
#include <stdlib.h>

#define Machine_STACK_SIZE 256

Machine *Machine_new(Uyghur *uyghur) {
    Machine *vm = malloc(sizeof(Machine));
    vm->uyghur = uyghur;
    vm->stack = NULL;
    vm->first = NULL;
    vm->sweeping = false;
    vm->freezing = true;
    vm->calls = NULL;
    vm->globals = NULL;
    vm->rootModule = NULL;
    vm->currHoldable = NULL;
    vm->numObjects = 0;
    vm->maxObjects = 50;
    return vm;
}

void _machine_mark_object(Object *object) {
    if (object->gcFreeze) return;
    // log_debug("mark_object:%c %p", object->objType, object);
    object->gcMark = 1;
}

void _machine_free_object(Machine *this, Object* object) {
    if (object->gcFreeze) return;
    // add hashmap and hashkey to vm
    if (object->objType == PCT_OBJ_VALUE) {
        Value *value = object;
        if (value->type == UG_TYPE_STR) {
            // log_info("free str %p", object);
            String_free(value->string);
        } else if (value->type == UG_TYPE_OBJ) {
            // log_info("free obj %p", object);
            Queue_free(value->extra);
        }
        free(object);
    } else if (object->objType == PCT_OBJ_ARRAY) {
        // log_info("free arr %p", object);
        Array_free(object);
    } else if (object->objType == PCT_OBJ_HASHMAP) {
        // log_info("free map %p", object);
        Hashmap_free(object);
    } else {
        free(object);
    }
}

void Machine_pushHolder(Machine *this, Object* object) {
    tools_assert(this->stack->size < MAX_STACK_SIZE, NULL, LANG_ERR_EXECUTER_STACK_OVERFLOW);
    Stack_push(this->stack, object);
    this->currHoldable = (Holdable *)this->stack->tail->data;
    this->rootModule = (Holdable *)this->stack->head->data;
}

Object* Machine_popHolder(Machine *this) {
    tools_assert(this->stack->size > 0, "Stack underflow!");
    Holdable *holdable = Stack_pop(this->stack);
    this->currHoldable = (Holdable *)this->stack->tail->data;
    return holdable;
}

Holdable *Machine_getCurrentModule(Machine *this)
{
    Stack_RESTE(this->stack);
    Holdable *holdable = NULL;
    while ((holdable = Stack_NEXT(this->stack)) != NULL)
    {
        if (Holdable_isModule(holdable)) break;
    }
    return holdable;
}

Value *Machine_getCurrentSelf(Machine *this)
{
    Stack_RESTE(this->stack);
    Value *value = NULL;
    while ((value = Stack_NEXT(this->stack)) != NULL)
    {
        if (!Holdable_isScope(value)) break;
        Value *self = Dictable_getLocation(value, SCOPE_ALIAS_SLF);
        if (self != NULL) {
            value = self;
            break;
        }
    }
    return value;
}

void _machine_mark_listable(Listable *);
void _machine_mark_dictable(Dictable *);
void _machine_mark_objective(Objective *);
void _machine_mark_hashkey(Hashkey *, void *);
void _machine_mark_value(Value *);

void _machine_mark_value(Value *value) {
    if (value == NULL) return;
    if (value->gcMark) return;
    if (value->type == UG_TYPE_NIL || value->type == UG_TYPE_BOL || value->type == UG_TYPE_NUM) {
        _machine_mark_object(value);
    } else if (value->type == UG_TYPE_STR) {
        _machine_mark_object(value);
        // _machine_mark_object(value->string);
    } else if (is_type_listable(value->type)) {
        _machine_mark_listable(value);
    } else if (is_type_dictable(value->type)) {
        _machine_mark_dictable(value);
    } else if (is_type_holdable(value->type)) {
        _machine_mark_dictable(value);
    } else if (is_type_objective(value->type)) {
        _machine_mark_objective(value);
    } else if (is_type_runnable(value->type)) {
        _machine_mark_object(value);
    } else {
        log_warn("value type not registered for marking");
        Value_print(value);
    }
}

void _machine_mark_arrkey(int key, Value *val, void *other) {
    _machine_mark_value(val);
}

void _machine_mark_listable(Listable *listable) {
    if (listable->gcMark) return;
    // Machine *this = __uyghur->machine;
    // log_info("mark_listable %p %c %i %i", listable, listable->type, listable == this->globals, listable == this->rootModule);
    _machine_mark_object(listable);
    _machine_mark_object(listable->arr);
    Array_foreachItem(listable->arr, _machine_mark_arrkey, NULL);
}

void _machine_mark_hashkey(Hashkey *hashkey, void *other) {
    String *key = hashkey->key;
    Value *val = hashkey->value;
    // _machine_mark_object(key);
    _machine_mark_value(val);
}

void _machine_mark_dictable(Dictable *dictable) {
    if (dictable->gcMark) return;
    // Machine *this = __uyghur->machine;
    // log_info("mark_dictable %p %c %i %i", dictable, dictable->type, dictable == this->globals, dictable == this->rootModule);
    _machine_mark_object(dictable);
    _machine_mark_object(dictable->map);
    Hashmap_foreachItem(dictable->map, _machine_mark_hashkey, NULL);
}

void _machine_mark_objective(Objective *objective) {
    if (objective->gcMark) return;
    // Machine *this = __uyghur->machine;
    // log_info("mark_objective %p %c %i %i", objective, objective->type, objective == this->globals, objective == this->rootModule);
    _machine_mark_dictable(objective);
    if(Objective_isObj(objective)) {
        Queue *parents = objective->extra;
        if (parents != NULL) {
            Queue_RESTE(parents);
            Objective *parent = Queue_NEXT(parents);
            while (parent != NULL) {
                _machine_mark_objective(parent);
                parent = Queue_NEXT(parents);
            }
        }
    }
}

void _machine_mark_cstack(void *ptr, void *other) {
    
    // Machine *this = __uyghur->machine;
    // Holdable *ctnr = ptr;
    // log_debug("mark_stack %p %p", ctnr, this->stack->tail->data);
    _machine_mark_dictable(ptr);
}

void _machine_mark_vstack(void *ptr, void *other) {
    _machine_mark_value(ptr);
}

void Machine_mark(Machine *this)
{
    _machine_mark_dictable(this->globals);
    Stack_foreachItem(this->stack, _machine_mark_cstack, NULL);
    Stack_foreachItem(this->calls, _machine_mark_vstack, NULL);
}


int sweepFate = 0;
int SWEEP_DEPTH = 5;

void Machine_sweep(Machine *this)
{
    Object* previous = NULL;
    Object* object = this->first;
    while (object) {
        // log_info("--->seeep:%i %c %p", object->gcMark, object->objType, object);
        // Object_print(object);
        if (object->gcMark) {
            object->gcMark = 0;
            previous = object;
            object = object->gcNext;
            continue;
        }
        Object* unreached = object;
        object = unreached->gcNext;
        if (previous != NULL) {
            previous->gcNext = object;
        } else {
            this->first = object;
        }
        _machine_free_object(this, unreached);
        this->numObjects--;
    }
}

void Machine_runGC(Machine *this) {
    if(!this->sweeping) return;
    log_debug("========gc_bgn:");
    double bgnTime = time_get_clock();
    int bgnCount = this->numObjects;
    Machine_mark(this);
    double midTime = time_get_clock();
    double mrkTime = midTime - bgnTime;
    log_debug("========gc_mid: %f %d", mrkTime, bgnCount);
    Machine_sweep(this);
    double endTime = time_get_clock();
    double swpTime = endTime - bgnTime;
    int endCount = this->numObjects;
    int delCount = bgnCount - endCount;
    this->maxObjects = endCount * 2 + 10000; // TODO:gc issue
    log_debug("========gc_end! %f %d - %d = %d", swpTime, bgnCount, delCount, endCount);
}

void Machine_tryGC(Machine *this) {
    if (this->sweeping && this->numObjects >= this->maxObjects) {
        Machine_runGC(this);
    }
}

void Machine_dump(Machine *this) {
}

void Machine_free(Machine *this) { 
    Machine_runGC(this);
    Object_free(this);
}

void Machine_tryLinkForGC(Object *object) {
    Machine *this = __uyghur->machine;
    if (!this->sweeping) return;
    object->gcFreeze = this->freezing;
    if (object->gcFreeze) return;
    object->gcNext = this->first;
    this->first = object;
    this->numObjects++;
}

Object* Machine_createObjByCurrentFreezeFlag(char type, size_t size) {
    Object *object = pct_mallloc(size);
    memset(object, 0, size);
    Object_init(object, type);
    Machine_tryLinkForGC(object);
    return object;
}

Object* Machine_createObjByCustomFreezeFlag(char type, size_t size, bool freeze) {
    Machine *this = __uyghur->machine;
    bool freezing = this->freezing;
    this->freezing = freeze;
    Object* object = Machine_createObjByCurrentFreezeFlag(type, size);
    this->freezing = freezing;
    return object;
}

Object* Machine_createObjNotFreeze(char type, size_t size) {
    return Machine_createObjByCustomFreezeFlag(type, size, false);
}

Object* Machine_createObjAndFreeze(char type, size_t size) {
    return Machine_createObjByCustomFreezeFlag(type, size, true);
}

void Machine_retainObj(Object* object) {
    Machine* this = __uyghur->machine;
    if (!this->sweeping) {
        Object_retain(object);
    }
}

void Machine_releaseObj(Object* object) {
    Machine* this = __uyghur->machine;
    if (!this->sweeping) {
        Object_release(object);
    }
}

void Machine_freeObj(Object* object) {
    Machine* this = __uyghur->machine;
    if (!this->sweeping) {
        Object_free(object);
    }
}

void Machine_testGC() {
    log_warn("\n\n\n---------------------TEST:");
    Machine* this = __uyghur->machine;
    Machine_pushHolder(this, Holdable_newScope(NULL));
    for (size_t i = 0; i < 100; i++) {
        log_warn("---------------------test%i", i);
        for (size_t j = 0; j < 1000000; j++) {
            Holdable_newScope(NULL);
        }
        Machine_runGC(this);
        time_sleep_seconds(1);
    }
    log_warn("\n\n\n---------------------END!!!");
    time_sleep_seconds(10);
    
}
