// machine

#include "others/header.h"

#include <stdio.h>
#include <stdlib.h>

#define Machine_STACK_SIZE 256

Machine *Machine_new(Uyghur *uyghur) {
    Machine *vm = malloc(sizeof(Machine));
    vm->uyghur = uyghur;
    vm->first = NULL;
    vm->sweeping = false;
    vm->freezing = true;
    vm->hldStack = NULL;
    vm->envStack = NULL;
    vm->globals = NULL;
    vm->rootModule = NULL;
    vm->currHoldable = NULL;
    vm->currEnvironment = NULL;
    vm->numObjects = 0;
    vm->maxObjects = 50;
    //
    vm->cacheMap = NULL;
    vm->cacheArr = NULL;
    vm->cacheVal = NULL;
    //
    int valueSize = sizeof(Value);
    valueSize = MAX(valueSize, sizeof(Runnable));
    valueSize = MAX(valueSize, sizeof(Listable));
    valueSize = MAX(valueSize, sizeof(Dictable));
    vm->valueSize = valueSize * 8;
    //
    return vm;
}

///////////////////////////////////////////////////////////////////////////

Object *_machine_readFromCache(Object **cache, size_t size) {
    Object *object = *cache;
    if (object) {
        *cache = object->gcNext;
        object->gcCount = 0;
        object->gcNext = NULL;
    }
    return object;
}

bool _machine_writeToCache(Object **cache, size_t size, Object *object) {
    if (!IS_GC_CACHEABLE) {
        return false;
    }
    if (*cache) {
        int count = (*cache)->gcCount;
        if (count >= size) {
            return false;
        }
        object->gcCount = count + 1;
        object->gcNext = *cache;
    }
    *cache = object;
    return true;
}

Array *Machine_newCacheableArray(char c) {
    Machine* this = __uyghur->machine;
    Array *arr = _machine_readFromCache(&this->cacheArr, MACHINE_CACHE_SIZE_ARR);
    if (!arr) {
        arr = Array_new(IS_GC_COUNTING);
    }
    #if IS_GC_LINK_LISTABLE_ARR
    Machine_tryLinkForGC(arr);
    #endif
    return arr;
}

Hashmap *Machine_newCacheableHashmap(char c) {
    Machine* this = __uyghur->machine;
    Hashmap *map = _machine_readFromCache(&this->cacheMap, MACHINE_CACHE_SIZE_MAP);
    if (!map) {
        map = Hashmap_new(IS_GC_COUNTING);
    }
    #if IS_GC_LINK_DICTABLE_MAP
    Machine_tryLinkForGC(map);
    #endif
    return map;
}

Value *Machine_newNormalValue(bool freeze, char typ) {
    Machine* this = __uyghur->machine;
    size_t size = this->valueSize;
    Value *value = NULL;
    if (freeze) {
        value = Machine_createObjAndFreeze(PCT_OBJ_VALUE, size);
    } else {
        value = Machine_createObjByCurrentFreezeFlag(PCT_OBJ_VALUE, size);
    }
    value->type = typ;
    value->obj = NULL;
    value->fixed = false;
    value->token = NULL;
    value->linka = NULL;
    value->extra = NULL;
    // log_debug("new=%s: %p", get_value_name(typ, "value"), value);
    return value;
}

int valueCount = 0;

Value *Machine_newCacheableValue(char tp, bool isDebug) {
    // 
    Machine* this = __uyghur->machine;
    Value *value = _machine_readFromCache(&this->cacheVal, MACHINE_CACHE_SIZE_VAL);
    bool cached = value != NULL;
    if (value) {
        Machine_tryLinkForGC(value);
    } else {
        value = Machine_newNormalValue(false, tp);
        value->obj = NULL;
    }
    //
    if (is_type_listable(tp)) {
        Listable *listable = value;
        listable->arr = Machine_newCacheableArray(tp);
    }
    //
    if (is_type_dictable(tp) || is_type_holdable(tp) || is_type_objective(tp)) {
        Dictable *dictable = value;
        dictable->map = Machine_newCacheableHashmap(tp);
    }
    //
    value->type = tp;
    value->fixed = false;
    value->token = NULL;
    value->linka = NULL;
    value->extra = NULL;
    return value;
}

void Machine_freeCacheableValue(Value *value) {
    Machine* this = __uyghur->machine;
    value->obj = NULL;
    value->fixed = NULL;
    value->token = NULL;
    value->linka = NULL;
    value->extra = NULL;
    //
    bool cached = _machine_writeToCache(&this->cacheVal, MACHINE_CACHE_SIZE_VAL, value);
    if (cached) {
        value->type = UG_TYPE_NIL;
    } else {
        free(value);
    }
}

void Machine_freeCacheableArray(Array *arr) {
    Machine* this = __uyghur->machine;
    bool cached = _machine_writeToCache(&this->cacheArr, MACHINE_CACHE_SIZE_ARR, arr);
    if (cached) {
        Array_clear(arr);
    } else {
        Array_free(arr);
    }
}

void Machine_freeCacheableHashmap(Hashmap *map) {
    Machine* this = __uyghur->machine;
    bool cached = _machine_writeToCache(&this->cacheMap, MACHINE_CACHE_SIZE_MAP, map);
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

void _machine_free_object(Machine *this, Object* object) {
    if (object->gcFreeze) return;
    object->gcNext = NULL;
    object->gcCount = 0;
    // add hashmap and hashkey to vm
    if (object->objType == PCT_OBJ_VALUE) {
        Value *value = object;
        if (value->type == UG_TYPE_STR) {
            // log_info("free str %p", object);
            String_free(value->string);
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

Holdable *_Machine_writeKind(Machine *this, char *name) {
    Holdable *holdable = Holdable_newKind(name);
    Machine_retainObj(holdable);
    helper_set_aliased_key(this->globals, name, holdable);
    log_debug("proxy: %s %p", name, holdable);
    return holdable;
}

void Machine_initKinds(Machine *this) {
    this->kindLgc = _Machine_writeKind(this, TVALUE_BOL);
    this->kindNum = _Machine_writeKind(this, TVALUE_NUM);
    this->kindStr = _Machine_writeKind(this, TVALUE_STR);
    this->kindList = _Machine_writeKind(this, TVALUE_LST);
    this->kindDict = _Machine_writeKind(this, TVALUE_DCT);
}

Holdable *Machine_readKind(Machine *this, char *name) {
    if (name == NULL) return NULL;
    if (is_eq_string(name, TVALUE_BOL)) return this->kindLgc;
    if (is_eq_string(name, TVALUE_NUM)) return this->kindNum;
    if (is_eq_string(name, TVALUE_STR)) return this->kindStr;
    if (is_eq_string(name, TVALUE_LST)) return this->kindList;
    if (is_eq_string(name, TVALUE_DCT)) return this->kindDict;
    return NULL;
}

void Machine_pushHolder(Machine *this, Holdable* holdable) {
    tools_assert(this->hldStack->size < MAX_STACK_SIZE, NULL, LANG_ERR_EXECUTER_STACK_OVERFLOW);
    Stack_push(this->hldStack, holdable);
    this->currHoldable = (Holdable *)this->hldStack->tail->data;
    if (this->rootModule == NULL) {
        tools_assert(Holdable_isModule(holdable), NULL, LANG_ERR_EXECUTER_INVALID_STATE);
        this->rootModule = holdable;
    }
}

Holdable* Machine_popHolder(Machine *this) {
    tools_assert(this->hldStack->size > 0, "Stack underflow!");
    Holdable *holdable = Stack_pop(this->hldStack);
    Block *block = this->hldStack->tail;
    this->currHoldable = block != NULL ? (Holdable *)block->data : NULL;
    return holdable;
}

void Machine_pushEnvironment(Machine *this, Holdable* holdable) {
    tools_assert(this->envStack->size < MAX_STACK_SIZE, NULL, LANG_ERR_EXECUTER_STACK_OVERFLOW);
    Stack_push(this->envStack, holdable);
    this->currEnvironment = (Holdable *)this->envStack->tail->data;
}

Holdable* Machine_popEnvironment(Machine *this) {
    tools_assert(this->envStack->size > 0, "Stack underflow!");
    Holdable *holdable = Stack_pop(this->envStack);
    Block *block = this->envStack->tail;
    this->currEnvironment = block != NULL ? (Holdable *)block->data : NULL;
    return holdable;
}

Holdable *Machine_getCurrentModule(Machine *this)
{
    Stack_RESTE(this->hldStack);
    Holdable *holdable = NULL;
    while ((holdable = Stack_NEXT(this->hldStack)) != NULL)
    {
        if (Holdable_isModule(holdable)) break;
    }
    return holdable;
}

Value *Machine_getCurrentSelf(Machine *this)
{
    Stack_RESTE(this->hldStack);
    Value *value = NULL;
    while ((value = Stack_NEXT(this->hldStack)) != NULL)
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
        _machine_mark_holdable(value);
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

void _machine_mark_hstack(void *ptr, void *other) {
    
    // Machine *this = __uyghur->machine;
    // Holdable *ctnr = ptr;
    _machine_mark_holdable(ptr);
}

void _machine_mark_vstack(void *ptr, void *other) {
    _machine_mark_value(ptr);
}

void _machine_mark_timer(Value *value) {
    _machine_mark_value(value);
}

void Machine_mark(Machine *this)
{
    _machine_mark_dictable(this->globals);
    Stack_foreachItem(this->hldStack, _machine_mark_hstack, NULL);
    Stack_foreachItem(this->envStack, _machine_mark_hstack, NULL);
    Stack_foreachItem(this->calls, _machine_mark_vstack, NULL);
    timer_each(&_machine_mark_timer);
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
    // log_debug("========gc_bgn:");
    double bgnTime = time_clock();
    int bgnCount = this->numObjects;
    Machine_mark(this);
    double midTime = time_clock();
    double mrkTime = midTime - bgnTime;
    // log_debug("========gc_mid: %f %d", mrkTime, bgnCount);
    Machine_sweep(this);
    double endTime = time_clock();
    double swpTime = endTime - bgnTime;
    int endCount = this->numObjects;
    int delCount = bgnCount - endCount;
    this->maxObjects = endCount * 2 + MACHINE_OBJECTS_EXTRA_COUNT;
    // log_debug("========gc_end! %f %d - %d = %d", swpTime, bgnCount, delCount, endCount);
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
    Machine_pushHolder(this, Holdable_newScope("test", NULL));
    for (size_t i = 0; i < 100; i++) {
        log_warn("---------------------test%i", i);
        for (size_t j = 0; j < 1000000; j++) {
            Holdable_newScope("test", NULL);
        }
        Machine_runGC(this);
        sleep(1);
    }
    log_warn("\n\n\n---------------------END!!!");
    sleep(10);
    
}
