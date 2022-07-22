// vm

#include <stdio.h>
#include <stdlib.h>

#define VM_STACK_SIZE 256

typedef enum {
    VM_OBJ_UNKNOWN
} VmType;

typedef struct sObject {
    VmType type;
    unsigned char marked;
    struct sObject* next;
    struct sObject* data;
} VmObject;

typedef struct {
    VmObject* stack[VM_STACK_SIZE];
    int stackSize;
    VmObject* firstObject;
    int numObjects;
    int maxObjects;
} Vm;

Vm* Vm_new() {
    Vm* vm = malloc(sizeof(Vm));
    vm->stackSize = 0;
    vm->firstObject = NULL;
    vm->numObjects = 0;
    vm->maxObjects = 16;
    return vm;
}

void Vm_freeObject(VmObject* object) {
    if (object->type == VM_OBJ_UNKNOWN) {
        // TODO
    }
    free(object);
}

void Vm_push(Vm* vm, VmObject* object) {
    tools_assert(vm->stackSize < VM_STACK_SIZE, "Stack overflow!");
    vm->stack[vm->stackSize++] = object;
}

VmObject* Vm_pop(Vm* vm) {
    tools_assert(vm->stackSize > 0, "Stack underflow!");
    return vm->stack[--vm->stackSize];
}

void Vm_mark(Vm* vm)
{
    for (int i = 0; i < vm->stackSize; i++) {
        if (!vm->stack[i]->marked) vm->stack[i]->marked = 1;
    }
}

void Vm_sweep(Vm* vm)
{
    VmObject** object = &vm->firstObject;
    while (*object) {
        if ((*object)->marked) {
            (*object)->marked = 0;
            object = &(*object)->next;
        } else {
            VmObject* unreached = *object;
            *object = unreached->next;
            Vm_freeObject(unreached);
            vm->numObjects--;
        }
    }
}

void Vm_gc(Vm* vm) {
    int numObjects = vm->numObjects;
    Vm_mark(vm);
    Vm_sweep(vm);
    if (vm->numObjects >= vm->maxObjects) vm->maxObjects = vm->maxObjects * 2;
    printf("<vm:collectd %d, left:%d>\n", numObjects - vm->numObjects, vm->numObjects);
}

VmObject* Vm_newObject(Vm* vm, void *data, VmType type) {
    if (vm->numObjects == vm->maxObjects) Vm_gc(vm);
    VmObject* object = malloc(sizeof(VmObject));
    object->data = data;
    object->type = type;
    object->next = vm->firstObject;
    object->marked = 0;
    vm->firstObject = object;
    vm->numObjects++;
    return object;
}

void Vm_pushObject(Vm* vm, void *data, VmType type) {
    VmObject* object = Vm_newObject(vm, data, type);
    Vm_push(vm, object);
}

void Vm_clear(Vm *vm) {
    vm->stackSize = 0;
    Vm_gc(vm);
}

void Vm_free(Vm *vm) {
    Vm_clear(vm);
    free(vm);
}

// Vm *vm = Vm_new();
// for (size_t i = 0; i < 100; i++)
// {
//     Value *v = Value_newNumber(i, NULL);
//     Vm_pushObject(vm, v, VM_OBJ_UNKNOWN);
//     Vm_pop(vm);
// }