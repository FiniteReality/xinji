#include "assert.h"
#include "stdlib.h"
#include "string.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "xinji.h"
#include "xinji_lib.h"

#define REPORT_ERROR(state, msg) \
    do { \
        if (state->reporter != NULL) state->reporter(state, msg); \
    } while (0)

#define REPORT_LUA_ERROR(state) \
    do { \
        if (state->reporter != NULL && !lua_isnil(state->L, -1)) { \
            const char* msg = lua_tostring(state->L, -1); \
            if (msg == NULL) msg = "(error object is not a string)"; \
            state->reporter(state, msg); \
            lua_pop(state->L, 1); \
        } \
    } while (0)

struct xinji_state
{
    lua_State* L;
    xinji_error_report_fn reporter;
};

xinji_state* xinji_open()
{
    lua_State* L = luaL_newstate();

    if (L == NULL)
        return NULL;

    luaopen_base(L);
    luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
    luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
    luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);

    luaL_requiref(L, XINJI_LIBNAME, luaopen_xinji, 1);

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

xinji_error_report_fn xinji_set_error_reporter(xinji_state* state,
    xinji_error_report_fn fn)
{
    assert(state != NULL);

    xinji_error_report_fn old = state->reporter;
    state->reporter = fn;
    return old;
}

int xinji_parse_file(xinji_state* state, const char* location)
{
    assert(state != NULL);

    int status = luaL_loadfilex(state->L, location, "t");
    if (status != LUA_OK)
    {
        REPORT_LUA_ERROR(state);
        return 0;
    }

    status = lua_pcall(state->L, 0, 0, 0);
    if (status != LUA_OK)
    {
        REPORT_LUA_ERROR(state);
        return 0;
    }

    lua_getglobal(state->L, "xinji");
    lua_getfield(state->L, -1, "print");
    lua_getfield(state->L, LUA_REGISTRYINDEX, "xinji_declarations");
    status = lua_pcall(state->L, 1, 0, 0);
    if (status != LUA_OK)
    {
        REPORT_LUA_ERROR(state);
        return 0;
    }

    return 1;
}
