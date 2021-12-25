// baord

#include "raylib.h"
#include "../uyghur/uyghur.c"

// tool

void test()
{
    InitWindow(800, 450, "raylib [core] example - basic window");
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
}

// api

void ug_baord_test(Bridge *bridge)
{
    char *a = Bridge_popString(bridge);
    char *r = "";
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, r);
    Bridge_return(bridge);
}

void lib_board_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "doska");
    //
    Bridge_pushKey(bridge, "doskaSinaq");
    Bridge_pushFunction(bridge, ug_baord_test);
    //
    Bridge_register(bridge);
}
