#include "assert.h"
#include "stdlib.h"
#include "string.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "xinji.h"

struct xinji_state
{
    lua_State* L;
};

xinji_state* xinji_open()
{
    lua_State* L = luaL_newstate();

    if (L == NULL)
        return NULL;

    luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 0);
    luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 0);
    luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 0);

    xinji_state* state = malloc(sizeof(xinji_state));
    memset(state, 0, sizeof(xinji_state));

    state->L = L;

    return state;
}

void xinji_close(xinji_state* state)
{
    if (state == NULL)
        return;

    lua_close(state->L);
    free(state);
}

void xinji_parse_file(xinji_state* state, const char* location)
{
    assert(state != NULL);

    int status = luaL_loadfilex(state->L, location, "t")
        || lua_pcall(state->L, 0, 0, 0);

    assert(status == LUA_OK);
}
