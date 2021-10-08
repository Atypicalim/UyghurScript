// implementation

#include "uyghur.c"

int main(int argc, char const *argv[])
{
    //
    printf("hello uyghur script!\n");
    //
    tools_check(argc > 1, LANG_ERR_NO_INPUT_NAME);
    Uyghur *uyghur = Uyghur_new();
    Uyghur_run(uyghur, argv[1]);
    Uyghur_free(uyghur);
    //
    // token test
    // Token *token1 = Token_new("test.txt", 1, 1, "", "111");
    // Token_print(token1);
    // Token *token2 = Token_new("test.txt", 1, 1, "", "222");
    // Token_print(token2);
    // Token *token3 = Token_new("test.txt", 1, 1, "", "333");
    // Token_print(token3);
    // Block_append(token2, token3);
    // Block_prepend(token2, token1);
    // Token_free(token2);
    // Token_print(Block_last(Block_last(token3)));
    //
    // hash map test
    // Hashmap *map = Hashmap_new();
    // Hashmap_set(map, "aaa", "aaa-aaa");
    // Hashmap_set(map, "bbb", "bbb-bbb");
    // Hashmap_print(map);
    // Hashmap_free(map);
    //
    // block test
    // Block *block1 = Block_new("11");
    // Block *block2 = Block_new("22");
    // // Block_prepend(block1, block2);
    // Block_print(block1);
    // Block_print(block2);
    // Block_print(block1->last);
    //
    // stack test
    // Stack *stack = Stack_new();
    // Stack_push(stack, block1);
    // Stack_push(stack, block2);
    // Block *n = Stack_pop(stack);
    // Block *n2 = Stack_pop(stack);
    // Block_print(n);
    // Block_print(n2);
    //
    // queue test
    // Queue *queue = Queue_new();
    // Queue_push(queue, block1);
    // Queue_push(queue, block2);
    // Block *n = Queue_pop(queue);
    // Block *n2 = Queue_pop(queue);
    // Block_print(n);
    // Block_print(n2);
    //
    // leaf test
    // Leaf *leaf = Leaf_new("test");
    // Leaf_pushToken(leaf, token1);
    // Leaf_pushToken(leaf, token2);
    // Token *newToken = Leaf_popToken(leaf);
    // Leaf_pushToken(leaf, token3);
    // Leaf_print(leaf);
    // Leaf_free(leaf);
    // Token_print(newToken);
    // Leaf *child = Leaf_new("child");
    // Leaf_pushToken(child, token3);
    // Leaf_pushLeaf(leaf, child);
    // Leaf_print(leaf);
    // Leaf_free(leaf);
    //
    return 0;
}
