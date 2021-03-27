#ifndef XINJI_TYPE_H
#define XINJI_TYPE_H

#include "lua.h"

#define XINJI_TYPENAME "xinji_declaration"

void define_declaration_type(lua_State* L);

void declaration_create(lua_State* L);
void declaration_set_name(lua_State* L, int idx, const char* name, int skipDeclare);
void declaration_add_kind(lua_State* L, int idx, const char* kind);

#endif /* XINJI_TYPE_H */
