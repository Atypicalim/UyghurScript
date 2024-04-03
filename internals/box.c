// box

#include "../uyghur/uyghur.c"

void ug_box_loop(Bridge *bridge)
{
    Value *box = Bridge_receiveValue(bridge, UG_TYPE_CNT);
    Value *fun = Bridge_receiveValue(bridge, UG_TYPE_FUN);
    Container *_box = box->object;
    void (*_fun)() = fun->object;
    printf("loop: %s %s \n", Value_toString(box), Value_toString(fun));
    // 
    Executer *exe = bridge->uyghur->executer;
    Hashmap *map = _box->map;
    Hashkey *ptr;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = map[i].position;
        while (ptr != NULL) {
            String *_key = ptr->key;
            Value *key = Value_newString(_key, NULL);
            Value *val = ptr->value;
            Executer_clearStack(exe);
            Executer_pushStack(exe, val);
            Executer_pushStack(exe, key);
            Executer_runFunc(exe, fun);
            // 
            ptr = ptr->next;
        }
    }
    //
    Bridge_returnEmpty(bridge);
}

void lib_box_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    Bridge_bindNative(bridge, "dewriylesh", ug_box_loop);
    //
    Bridge_register(bridge, "sanduq");
}
