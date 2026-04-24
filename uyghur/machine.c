// machine

#include "others/header.h"

#include <stdio.h>
#include <stdlib.h>

#define Machine_STACK_SIZE 256

Object* __mapCreateFunc() { return Hashmap_new(IS_RETAIN_VALUES); }
Object* __arrCreateFunc() { return Array_new(IS_RETAIN_VALUES); }
Object* __valCreateFunc() { return Value_new(); }
void _machine_free_object(Object*);

Machine *Machine_new(Uyghur *uyghur) {
    Machine *vm = malloc(sizeof(Machine));
    vm->uyghur = uyghur;
    vm->sweeping = false;
    vm->freezing = true;
    vm->globals = NULL;
    vm->calls = NULL;
    vm->holders = NULL;
    vm->rootModule = NULL;
    vm->currHoldable = NULL;
    //
    int valueSize = sizeof(Value);
    valueSize = MAX(valueSize, sizeof(Runnable));
    valueSize = MAX(valueSize, sizeof(Listable));
    valueSize = MAX(valueSize, sizeof(Dictable));
    vm->valueSize = valueSize * 8;
    // 
    vm->gallector = Gallector_new(MACHINE_OBJECTS_EXTRA_COUNT, _machine_free_object);
    vm->cacheMap = Gallector_cache(vm->gallector, MACHINE_CACHE_SIZE_MAP, __mapCreateFunc);
    vm->cacheArr = Gallector_cache(vm->gallector, MACHINE_CACHE_SIZE_ARR, __arrCreateFunc);
    vm->cacheVal = Gallector_cache(vm->gallector, MACHINE_CACHE_SIZE_VAL, __valCreateFunc);
    //
    return vm;
}

///////////////////////////////////////////////////////////////////////////

Holdable *Machine_getProxyOrKindByType(Machine *this, char tp) {
    if (tp == UG_TYPE_BOL) return this->kindLgc;
    if (tp == UG_TYPE_NUM) return this->kindNum;
    if (tp == UG_TYPE_STR) return this->kindStr;
    if (tp == UG_TYPE_LST) return this->kindList;
    if (tp == UG_TYPE_DCT) return this->kindDict;
    if (tp == UG_TYPE_STF) return this->proxStuf;
    if (tp == UG_TYPE_TSK) return this->proxTask;
    return NULL;
}

Holdable *_Machine_writeKind(Machine *this, char *name) {
    Holdable *holdable = Holdable_newKind(name);
    Machine_retainObj(holdable);
    helper_set_lettered_key(this->globals, name, holdable);
    log_debug("kind: %s %p", name, holdable);
    return holdable;
}

Holdable *_Machine_writeProxy(Machine *this, char *name) {
    Holdable *holdable = Holdable_newProxy(name);
    Machine_retainObj(holdable);
    helper_set_lettered_key(this->globals, name, holdable);
    log_debug("proxy: %s %p", name, holdable);
    return holdable;
}

void Machine_initKinds(Machine *this) {
    this->kindLgc = _Machine_writeKind(this, LETTER_BOL);
    this->kindNum = _Machine_writeKind(this, LETTER_NUM);
    this->kindStr = _Machine_writeKind(this, LETTER_STR);
    this->kindList = _Machine_writeKind(this, LETTER_LST);
    this->kindDict = _Machine_writeKind(this, LETTER_DCT);
}

void Machine_initProxies(Machine *this) {
    this->proxStuf = _Machine_writeProxy(this, LETTER_STUF);
    this->proxTask = _Machine_writeProxy(this, LETTER_TASK);
}

Holdable *Machine_readKind(Machine *this, char *name) {
    if (name == NULL) return NULL;
    if (is_eq_string(name, LETTER_BOL)) return this->kindLgc;
    if (is_eq_string(name, LETTER_NUM)) return this->kindNum;
    if (is_eq_string(name, LETTER_STR)) return this->kindStr;
    if (is_eq_string(name, LETTER_LST)) return this->kindList;
    if (is_eq_string(name, LETTER_DCT)) return this->kindDict;
    return NULL;
}

Holdable *Machine_readProxy(Machine *this, char *name) {
    if (name == NULL) return NULL;
    if (is_eq_string(name, LETTER_STUF)) return this->proxStuf;
    if (is_eq_string(name, LETTER_TASK)) return this->proxTask;
    return NULL;
}

Value *Machine_newCacheableValue(char tp, bool freeze) {
    // 
    Machine* this = __uyghur->machine;
    Value *value = Gache_get(this->cacheVal, freeze || this->freezing);
    Value_reset(value);
    //
    value->type = tp;
    value->proxy = Machine_getProxyOrKindByType(this, tp);
    //
    if (is_type_listable(tp)) {
        value->arr = Gache_get(this->cacheArr, this->freezing);
    }
    if (is_type_dictable(tp) || is_type_holdable(tp) || is_type_objective(tp)) {
        value->map = Gache_get(this->cacheMap, this->freezing);
    }
    return value;
}

void Machine_freeCacheableValue(Value *value) {
    Machine* this = __uyghur->machine;
    bool cached = Gache_return(this->cacheVal, value);
    if (cached) {
        Value_reset(value);
    } else {
        free(value);
    }
}

void Machine_freeCacheableArray(Array *arr) {
    Machine* this = __uyghur->machine;
    bool cached = Gache_return(this->cacheArr, arr);
    if (cached) {
        Array_clear(arr);
    } else {
        Array_free(arr);
    }
}

void Machine_freeCacheableHashmap(Hashmap *map) {
    Machine* this = __uyghur->machine;
    bool cached = Gache_return(this->cacheMap, map);
    if (cached) {
        Hashmap_clear(map);
    } else {
        Hashmap_free(map);
    }
}

///////////////////////////////////////////////////////////////////////////

void _machine_mark_object(Object *object) {
    if (object->gcFreeze) return;
    // log_debug("mark_object:%c %p", object->objType, object);
    object->gcMark = 1;
}

void _machine_free_object(Object* object) {
    // add hashmap and hashkey to vm
    if (object->objType == PCT_OBJ_VALUE) {
        Value *value = object;
        if (value->type == UG_TYPE_STR) {
            // log_info("free str %p", object);
            pct_free(value->string);
        } else if (is_type_listable(value->type)) {
            #if !IS_GC_LINK_LISTABLE_ARR
            Array_free(value->arr);
            #endif
        } else if (is_type_dictable(value->type)) {
            #if !IS_GC_LINK_DICTABLE_MAP
            Hashmap_free(value->map);
            #endif
        } else if (is_type_holdable(value->type)) {
            #if !IS_GC_LINK_DICTABLE_MAP
            Hashmap_free(value->map);
            #endif
        } else if (is_type_objective(value->type)) {
            #if !IS_GC_LINK_DICTABLE_MAP
            Hashmap_free(value->map);
            #endif
            if (value->type == UG_TYPE_OBJ) {
                // log_info("free obj %p", object);
                Queue_free(value->extra);
            }
        } else if (is_type_runnable(value->type)) {
            //
        } else if (is_type_loadable(value->type)) {
            LOADABLE_RELEASE_FUNC func = value->linka;
            if (func != NULL) {
                func(value);
            }
        } else if (is_type_waitable(value->type)) {
            WAITABLE_RELEASE_FUNC func = value->linka;
            if (func != NULL) {
                func(value);
            }
        }
        Machine_freeCacheableValue(value);
    } else if (object->objType == PCT_OBJ_ARRAY) {
        // log_info("free arr %p", object);
        Machine_freeCacheableArray(object);
    } else if (object->objType == PCT_OBJ_HASHMAP) {
        // log_info("free map %p", object);
        Machine_freeCacheableHashmap(object);
    } else {
        free(object);
    }
}

void Machine_pushHolder(Machine *this, Holdable* holdable) {
    tools_assert(this->holders->size < MAX_STACK_SIZE, NULL, LANG_ERR_EXECUTER_STACK_OVERFLOW);
    Stack_push(this->holders, holdable);
    this->currHoldable = (Holdable *)this->holders->tail->data;
    if (this->rootModule == NULL) {
        tools_assert(Holdable_isModule(holdable), NULL, LANG_ERR_EXECUTER_INVALID_STATE);
        this->rootModule = holdable;
    }
}

Holdable* Machine_popHolder(Machine *this) {
    tools_assert(this->holders->size > 0, "Stack underflow!");
    Holdable *holdable = Stack_pop(this->holders);
    Block *block = this->holders->tail;
    this->currHoldable = block != NULL ? (Holdable *)block->data : NULL;
    return holdable;
}

Holdable *Machine_getCurrentModule(Machine *this)
{
    Stack_RESTE(this->holders);
    Holdable *holdable = NULL;
    while ((holdable = Stack_NEXT(this->holders)) != NULL)
    {
        if (Holdable_isModule(holdable)) break;
    }
    return holdable;
}

Value *Machine_getCurrentSelf(Machine *this)
{
    Stack_RESTE(this->holders);
    Value *value = NULL;
    while ((value = Stack_NEXT(this->holders)) != NULL)
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
void _machine_mark_holdable(Holdable *);
void _machine_mark_objective(Objective *);
void _machine_mark_runnable(Runnable *);
void _machine_mark_loadable(Loadable *);
void _machine_mark_waitable(Waitable *);
void _machine_mark_hashkey(Hashkey *, void *);
void _machine_mark_value(Value *);

void _machine_mark_value(Value *value) {
    if (value == NULL) return;
    if (value->gcMark) return;
    if (value->type == UG_TYPE_NIL || value->type == UG_TYPE_BOL || value->type == UG_TYPE_NUM) {
        _machine_mark_object(value);
    } else if (value->type == UG_TYPE_STR) {
        _machine_mark_object(value);
    } else if (is_type_listable(value->type)) {
        _machine_mark_listable(value);
    } else if (is_type_dictable(value->type)) {
        _machine_mark_dictable(value);
    } else if (is_type_holdable(value->type)) {
        _machine_mark_holdable(value);
    } else if (is_type_objective(value->type)) {
        _machine_mark_objective(value);
    } else if (is_type_runnable(value->type)) {
        _machine_mark_runnable(value);
    } else if (is_type_loadable(value->type)) {
        _machine_mark_loadable(value);
    } else if (is_type_waitable(value->type)) {
        _machine_mark_waitable(value);
    } else {
        log_warn("value type not registered for marking");
        Value_print(value);
    }
}

void _machine_mark_arrkey(int key, void *val, void *other) {
    _machine_mark_value((Value *)val);
}

void _machine_mark_listable(Listable *listable) {
    if (listable->gcMark) return;
    // Machine *this = __uyghur->machine;
    // log_info("mark_listable %p %c %i %i", listable, listable->type, listable == this->globals, listable == this->rootModule);
    _machine_mark_object(listable);
    #if IS_GC_LINK_LISTABLE_ARR
    _machine_mark_object(listable->arr);
    #endif
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
    #if IS_GC_LINK_DICTABLE_MAP
    _machine_mark_object(dictable->map);
    #endif
    Hashmap_foreachItem(dictable->map, _machine_mark_hashkey, NULL);
}

void _machine_mark_holdable(Holdable *holdable) {
    if (holdable->gcMark) return;
    _machine_mark_dictable(holdable);
    if (holdable->linka) {
        _machine_mark_holdable(holdable->linka);
    }
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

void _machine_mark_runnable(Runnable *runnable) {
    // closure environment
    if (runnable->linka) {
        _machine_mark_holdable(runnable->linka);
    }
    _machine_mark_object(runnable);
}

void _machine_mark_loadable(Loadable *loadable) {
    _machine_mark_object(loadable);
}

void _machine_mark_waitable(Waitable *waitable) {
    _machine_mark_object(waitable);
    _machine_mark_value(waitable->obj);
}

void _machine_mark_hstack(void *ptr, void *other) {
    
    // Machine *this = __uyghur->machine;
    // Holdable *ctnr = ptr;
    _machine_mark_holdable(ptr);
}

void _machine_mark_vstack(void *ptr, void *other) {
    _machine_mark_value(ptr);
}

void _machine_mark_timer(void *value) {
    _machine_mark_value((Value *)value);
}

void Machine_mark(Machine *this)
{
    _machine_mark_dictable(this->globals);
    Stack_foreachItem(this->holders, _machine_mark_hstack, NULL);
    Stack_foreachItem(this->calls, _machine_mark_vstack, NULL);
    timer_each(&_machine_mark_timer);
}

void Machine_runGC(Machine *this) {
    if(!this->sweeping) return;

    // log_debug("========gc_bgn:");
    double bgnTime = time_clock();
    Machine_mark(this);
    double midTime = time_clock();
    double mrkTime = midTime - bgnTime;
    // log_debug("========gc_mid: %f %d", mrkTime, bgnCount);
    int delCount = Gallector_sweep(this->gallector);
    double endTime = time_clock();
    double swpTime = endTime - bgnTime;
    // log_debug("========gc_end! %f %d - %d = %d", swpTime, bgnCount, delCount, endCount);
}

void Machine_tryGC(Machine *this) {
    if (this->gallector->numObjects >= this->gallector->maxObjects) {
        Machine_runGC(this);
    }
}

void Machine_dump(Machine *this) {
}

void Machine_free(Machine *this) { 
    Machine_runGC(this);
    Object_free(this);
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
    Machine_pushHolder(this, Holdable_newScope("test", NULL));
    for (size_t i = 0; i < 100; i++) {
        log_warn("---------------------test%i", i);
        for (size_t j = 0; j < 1000000; j++) {
            Holdable_newScope("test", NULL);
        }
        Machine_runGC(this);
        system_sleep(1000);
    }
    log_warn("\n\n\n---------------------END!!!");
    system_sleep(10 * 1000);
    
}
