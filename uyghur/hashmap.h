// hashmap

#ifndef _HASHMAP_H
#define _HASHMAP_H
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define CAPACITY 4097

typedef struct EntryNode{
    char *key;
    void *value;
    struct EntryNode *next;
} Entry;

typedef struct {
    Entry *position;
} Hashmap;

Hashmap *Hashmap_new();
void Hashmap_set(Hashmap *map, char *key, void *value);
void* Hashmap_get(Hashmap *map, char *key);
void Hashmap_del(Hashmap *map, char *key);
void Hashmap_print(Hashmap *map);
void Hashmap_free(Hashmap *map);

static unsigned long hashcode(char *key) {
    register unsigned long hash = 0;
    unsigned long ch;
    while (ch = (unsigned long)*key++) {
        hash = hash * 131 + ch;
    }
    return hash;
}

static int hash(char *key) {
    return hashcode(key) % CAPACITY;
}

Hashmap* Hashmap_new() {
    Hashmap *map = (Hashmap *)malloc(sizeof(Hashmap) * CAPACITY);
    for (int i = 0; i < CAPACITY; ++i ) {
         map[i].position = NULL;
    }
    return map;
}

void Hashmap_free(Hashmap *map) {
    for (int i = 0; i < CAPACITY; ++i) {
        Entry *ptr = map[i].position;
        while (ptr != NULL) {
            Entry *head = ptr;
            ptr = ptr->next;
            if (ptr) free(head);
        }
    }
    free(map);
}

void Hashmap_set(Hashmap *map, char *key, void *value) {
    assert(map != NULL);
    int pos = hash(key);
    Entry *ptr = map[pos].position;
    Entry *pnode = (Entry *)malloc(sizeof(Entry));
    pnode->key = key;
    pnode->value = value;
    pnode->next = NULL;
    if (ptr == NULL) {
        map[pos].position = pnode;
    } else {
        while (ptr != NULL) {
            if (!strcmp(key, ptr->key)) {
                ptr->value = value;
                return;
            }
            ptr = ptr->next;
        }
        pnode->next = map[pos].position;
        map[pos].position = pnode;
    }
}

void Hashmap_fill(Hashmap *map, char *content) {
    Hashmap_set(map, content, content);
}

void *Hashmap_get(Hashmap *map, char *key) {
    assert(map != NULL);
    int pos = hash(key);
    Entry *ptr = map[pos].position;
    while (ptr != NULL) {
        if (!strcmp(key, ptr->key)) {
            return ptr->value;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void Hashmap_del(Hashmap *map, char *key) {
    assert(map != NULL);
    int pos = hash(key);
    Entry *ptr = map[pos].position;
    Entry *pre = ptr;
    if (ptr->next == NULL) {
        free(ptr);
        map[pos].position = NULL;
        return;
    }
    while (ptr != NULL) {
        if (!strcmp(key, ptr->key)) {
            pre->next = ptr->next;
            free(ptr);
            return;
        }
        pre = ptr;
        ptr = ptr->next;
    }
}

void Hashmap_print(Hashmap *map) {
    int i;
    printf("[Hashmap_start] address:%p capacity:%d\n", (void*)&map, CAPACITY);
    for (i = 0; i < CAPACITY; i++) {
        if (map[i].position != NULL) {
            Entry *ptr = map[i].position;
            while (ptr != NULL) {
                printf(" %s -> %p\n", ptr->key, (void*)&ptr->value);
                ptr = ptr->next;
            }
        }
    }
    printf("[Hashmap_end] address:%p", (void*)&map);
}
