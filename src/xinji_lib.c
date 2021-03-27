#include "lauxlib.h"

#include "xinji_lib.h"
#include "xinji_type.h"

void create_declaration(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    int skipDeclare = (lua_gettop(L) > 1) && lua_toboolean(L, 2);

    declaration_create(L);

    declaration_set_name(L, -1, name, skipDeclare);
}

#define DECLARATION_KIND(n) \
    int define_ ## n (lua_State* L) \
    { \
        create_declaration(L); \
        declaration_add_kind(L, -1, #n); \
        return 1; \
    }
#include "primitives.inc"
#undef DECLARATION_KIND

#define DECLARATION_KIND(n) \
    { #n, define_ ##n },
static const luaL_Reg xinji_funcs[] =
{
#include "primitives.inc"
    { NULL, NULL }
};
#undef DECLARATION_KIND

int luaopen_xinji(lua_State* L)
{
    luaL_newlib(L, xinji_funcs);
    define_declaration_type(L);
    return 1;
}
