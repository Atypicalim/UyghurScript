// time bind generated

#include "../../../uyghur/uyghur.c"
#include "../../../uyghur/{library}/{module}.c" // [M[ MODULE_FORMAT ]M]

 // [M[ MODULE_FUNSTIONS ]M]

void lib_{module}_register(Bridge *bridge) // [M[ MODULE_FORMAT ]M]
{
    Bridge_startBox(bridge);

    {register} //  // [M[ MODULE_REGISTERS ]M]

    Bridge_register(bridge, {target}); // [M[ MODULE_TARGET ]M]
}
