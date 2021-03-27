#include "lua.h"
#include "lauxlib.h"

#include "xinji_type.h"

void check_declaration(lua_State* L, int idx)
{
    if (lua_type(L, idx) != LUA_TTABLE)
        luaL_error(L, "expected table");

    if (!lua_getmetatable(L, idx))
        luaL_error(L, "missing metatable");

    luaL_getmetatable(L, XINJI_TYPENAME);

    if (!lua_rawequal(L, -1, -2))
        luaL_error(L, "expected %s", XINJI_TYPENAME);

        lua_pop(L, 2); // pop metatables
}

int declaration_mt_call(lua_State* L)
{
    check_declaration(L, 1);

    switch (lua_type(L, 2))
    {
        case LUA_TSTRING:
        {
            declaration_set_name(L, 1, lua_tostring(L, 2), 1);
            break;
        }
        case LUA_TTABLE:
        {
            lua_pushnil(L);
            while (lua_next(L, 2) != 0)
            {
                // key = -2
                // value = -1
                int keyType = lua_type(L, -2);
                if (keyType == LUA_TSTRING)
                {
                    lua_pushvalue(L, 1); // self
                    lua_pushvalue(L, -3); // key
                    lua_pushvalue(L, -3); // value
                    lua_rawset(L, -3); // self[key] = value
                    lua_pop(L, 1);
                }
                else if (keyType == LUA_TNUMBER)
                {
                    int length = luaL_len(L, 1); // #self
                    lua_pushvalue(L, 1); // self
                    lua_pushvalue(L, -2); // value
                    lua_rawseti(L, -2, length + 1); // self[#self+1] = value
                    lua_pop(L, 1);
                }
                else
                {
                    lua_pop(L, 2); // pop key/value
                    const char* msg = lua_pushfstring(L,
                        "unexpected key type %s", lua_typename(L, keyType));
                    return luaL_argerror(L, 2, msg);
                }

                lua_pop(L, 1); // pop value
            }
            break;
        }
        default:
            return luaL_argerror(L, 2, "unexpected type");
    }

    lua_pop(L, lua_gettop(L) - 1);
    return 1;
}

static const luaL_Reg metafuncs[] = {
    { "__call", declaration_mt_call },
    { NULL, NULL }
};

void define_declaration_type(lua_State* L)
{
    luaL_newmetatable(L, XINJI_TYPENAME);
    luaL_setfuncs(L, metafuncs, 0);
    lua_pop(L, 1);
}

void declaration_create(lua_State* L)
{
    // declaration
    lua_createtable(L, 0, 1);

    // kind
    lua_createtable(L, 0, 0);
    lua_setfield(L, -2, "kind");

    // metatable
    luaL_getmetatable(L, XINJI_TYPENAME);
    lua_setmetatable(L, -2);
}

void declaration_set_name(lua_State* L, int idx, const char* name, int skipDeclare)
{
    check_declaration(L, idx);

    // duplicate the value at "idx" so our stack offsets work correctly
    lua_pushvalue(L, idx);
    lua_pushliteral(L, "name");
    lua_pushstring(L, name);
    lua_rawset(L, -3);

    if (!skipDeclare) {
        luaL_getsubtable(L, LUA_REGISTRYINDEX, "xinji_declarations");
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, name);
        lua_pop(L, 1);

        declaration_create(L);
        declaration_set_name(L, -1, name, 1);
        declaration_add_kind(L, -1, "reference");
        lua_setglobal(L, name);
    }

    lua_pop(L, 1); // pop duplciate declaration table
}


void declaration_add_kind(lua_State* L, int idx, const char* kind)
{
    check_declaration(L, idx);

    // duplicate the value at "idx" so our stack offsets work correctly
    lua_pushvalue(L, idx);
    lua_pushliteral(L, "kind");
    lua_rawget(L, -2);

    int length = luaL_len(L, -1);
    lua_pushstring(L, kind);
    lua_rawseti(L, -2, length + 1);

    lua_pop(L, 2); // pop "kind" table and duplicate declaration table
}
