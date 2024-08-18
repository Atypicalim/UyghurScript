// box

#include "../uyghur.c"

void native_box_iterate(Bridge *bridge)
{
    Value *box = Bridge_receiveValue(bridge, UG_TYPE_BOX);
    Value *wkr = Bridge_receiveValue(bridge, UG_TYPE_WKR);
    void (*_wkr)() = wkr->obj;
    // 
    Executer *exe = bridge->uyghur->executer;
    Hashmap *map = ((Holdable*)box)->map;
    Hashkey *ptr;
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
        ptr = map->bucket[i];
        while (ptr != NULL) {
            String *_key = ptr->key;
            Value *key = Value_newString(_key, NULL);
            Value *val = ptr->value;
            Executer_clearStack(exe);
            Executer_pushStack(exe, val);
            Executer_pushStack(exe, key);
            Executer_applyWorker(exe, Token_empty(), wkr, NULL);
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
    BRIDGE_BIND_NATIVE(box_iterate);
    //
    Bridge_register(bridge, ALIAS_BOX);
}
