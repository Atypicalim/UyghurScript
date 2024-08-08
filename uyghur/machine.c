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
    vm->freezing = true;
    vm->collecting = false;
    vm->calls = NULL;
    vm->globals = NULL;
    vm->rootModule = NULL;
    vm->currContainer = NULL;
    vm->numObjects = 0;
    vm->maxObjects = 50;
    return vm;
}

void _machine_mark_object(Object *object) {
    if (object->gcFreeze) return;
    object->gcMark = 1;
}

void _machine_free_object(Machine *this, Object* object) {
    if (object->gcFreeze) return;
    Container *container = object;
    // add hashmap and hashkey to vm
    // if (object->objType == PCT_OBJ_CNTNR) {
    //     free(container->map);
    // }
    free(object);
}

void Machine_pushContainer(Machine *this, Object* object) {
    tools_assert(this->stack->size < MAX_STACK_SIZE, NULL, LANG_ERR_EXECUTER_STACK_OVERFLOW);
    Stack_push(this->stack, object);
    this->currContainer = (Container *)this->stack->tail->data;
    this->rootModule = (Container *)this->stack->head->data;
}

Object* Machine_popContainer(Machine *this) {
    tools_assert(this->stack->size > 0, "Stack underflow!");
    Container *container = Stack_pop(this->stack);
    this->currContainer = (Container *)this->stack->tail->data;
    return container;
}

Container *Machine_getCurrentModule(Machine *this, Token *token)
{
    Cursor *cursor = Stack_reset(this->stack);
    Container *container = NULL;
    while ((container = Stack_next(this->stack, cursor)) != NULL)
    {
        if (Container_isModule(container)) break;
    }
    Cursor_free(cursor);
    return container;
}

Container *Machine_getCurrentSelf(Machine *this, Token *token)
{
    Cursor *cursor = Stack_reset(this->stack);
    Container *container = NULL;
    while ((container = Stack_next(this->stack, cursor)) != NULL)
    {
        if (!Container_isScope(container)) break;
        Value *self = Container_getLocation(container, SCOPE_ALIAS_SLF);
        if (self != NULL) {
            container = self->object;
            break;
        }
    }
    Cursor_free(cursor);
    return container;
}

void _machine_mark_container(Container *);
void _machine_mark_hashkey(Hashkey *, void *);
void _machine_mark_value(Value *);

void _machine_mark_value(Value *value) {
    if (value == NULL) return;
    if (value->gcMark) return;
    _machine_mark_object(value);
    if (value->type == UG_TYPE_NUM) {
        // token
    } else if (value->type == UG_TYPE_STR) {
        _machine_mark_object(value->string);
    } else if (value->type == UG_TYPE_CNT) {
        _machine_mark_container(value->object);
    } else if (value->type == UG_TYPE_WKR) {
        // leaf
    }  else if (value->type == UG_TYPE_CTR) {
        _machine_mark_container(value->object);
    } else if (value->type == UG_TYPE_OBJ) {
        _machine_mark_container(value->object);
    } else if (value->type == UG_TYPE_NTV) {
        // native
    } else {
        log_warn("value type not registered for marking");
    }
}

void _machine_mark_hashkey(Hashkey *hashkey, void *other) {
    String *key = hashkey->key;
    Value *val = hashkey->value;
    _machine_mark_object(key);
    _machine_mark_value(val);
}

void _machine_mark_container(Container *container) {
    if (container->gcMark) return;
    _machine_mark_object(container);
    Hashmap_foreachItem(container->map, _machine_mark_hashkey, NULL);
}

void _machine_mark_cstack(void *ptr, void *other) {
    _machine_mark_container(ptr);
}

void _machine_mark_vstack(void *ptr, void *other) {
    _machine_mark_value(ptr);
}

void Machine_mark(Machine *this)
{
    _machine_mark_container(this->globals);
    Stack_foreachItem(this->stack, _machine_mark_cstack, NULL);
    Stack_foreachItem(this->calls, _machine_mark_vstack, NULL);
}

void Machine_sweep(Machine *this)
{
    Object* previous = NULL;
    Object* object = this->first;
    while (object) {
        // log_info("--->seeep:%i %i %p %i", object->gcMark, object != NULL, object, object == this->globals);
        // Object_print(object);
        if (object->gcMark) {
            object->gcMark = 0;
            previous = object;
            object = object->gcNext;
        } else {
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
}

void Machine_runGC(Machine *this) {
#if GC_USE_COUNTING
    //
#elif GC_USE_SWEEPING
    int numObjects = this->numObjects;
    log_warn("======================runGC:");
    Machine_mark(this);
    Machine_sweep(this);
    this->maxObjects = this->numObjects * 2;
    int numCollected = numObjects - this->numObjects;
    log_warn("======================endGC! %d %d/%d", numCollected, this->numObjects, this->maxObjects);
#endif
}

void Machine_tryGC(Machine *this) {
    if (this->numObjects >= this->maxObjects && this->collecting) {
        Machine_runGC(this);
    }
}

void Machine_dump(Machine *this) {
#if GC_USE_COUNTING
    log_warn("GC: counting");
#elif GC_USE_SWEEPING
    log_warn("GC: sweeping %d", this->numObjects);
#endif  
}

void Machine_free(Machine *this) {
    // Machine_dump(this);
    // Stack_clear(this->stack); 
    // Machine_runGC(this);
    free(this);
}

void Machine_tryLinkForGC(Object *object) {
    Machine *this = __uyghur->machine;
    object->gcFreeze = this->freezing;
    if (object->gcFreeze) return;
    object->gcNext = this->first;
    this->first = object;
    this->numObjects++;
}

Object* Machine_createObjTryGC(char type, size_t size) {
    Object* object = malloc(size);
    Object_init(object, type);
    Machine_tryLinkForGC(object);
    return object;
}

Object* _Machine_createObjChangeGC(char type, size_t size, bool freeze) {
    Machine *this = __uyghur->machine;
    bool freezing = this->freezing;
    this->freezing = freeze;
    Object* object = Machine_createObjTryGC(type, size);
    this->freezing = freezing;
    return object;
}

Object* Machine_createObjWithGC(char type, size_t size) {
    return _Machine_createObjChangeGC(type, size, false);
}

Object* Machine_createObjWithoutGC(char type, size_t size) {
    return _Machine_createObjChangeGC(type, size, true);
}

void Machine_retainObj(Object* object) {
    // Object_retain(object);
}

void Machine_releaseObj(Object* object) {
    // Object_release(object);
}

void Machine_freeObj(Object* object) {
    // Object_free(object);
}

void Machine_testGC(Machine* this) {
    log_warn("\n\n\n---------------------TEST:");
    Machine_pushContainer(this, Container_new(UG_CTYPE_SCP));
    for (size_t i = 0; i < 100; i++) {
        log_warn("---------------------test%i", i);
        for (size_t j = 0; j < 100; j++) {
            void *ctnr = Container_newBox();
            Container *target = this->globals;
            Value *self = Value_newContainer(target, NULL);
            Container_setLocation(this->currContainer, SCOPE_ALIAS_SLF, self);
        }
        log_warn("--gc");
        Machine_runGC(this);
        time_sleep_seconds(1);
    }
    log_warn("\n\n\n---------------------END!!!");
    time_sleep_seconds(10);
    
}
