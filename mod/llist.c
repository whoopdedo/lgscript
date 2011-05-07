#include "lua.h"
#include "lauxlib.h"
#include "mod/modlib.h"

/*
** {======================================================
** Lists
*/

#define aux_getn(L,n)	(luaL_checktype(L, n, LUA_TTABLE), luaL_getn(L, n))

static void auxnewlist (lua_State *L, int n) {
  lua_createtable(L, n, 0);
  luaL_getmetatable(L, LLIST_NAME);
  lua_setmetatable(L, -2);
}

static int list_new (lua_State *L) {
  int n = lua_gettop(L);
  int i;
  auxnewlist(L, n);
  for (i = 1; i <= n; i++) {
    lua_pushvalue(L, i);
    lua_rawseti(L, -2, i);
  }
  return 1;
}

static void addfield (lua_State *L, luaL_Buffer *b, int i) {
  lua_rawgeti(L, 1, i);
  if (!lua_isstring(L, -1))
    luaL_error(L, "invalid value (%s) at index %d in table for "
                  LUA_QL("concat"), luaL_typename(L, -1), i);
    luaL_addvalue(b);
}

static int list_tostring (lua_State *L) {
  int n = aux_getn(L, 1);
  int i;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addlstring(&b, "list(", 5);
  for (i = 1; i < n; i++) {
    addfield(L, &b, i);
    luaL_addlstring(&b, ", ", 2);
  }
  if (i == n)  /* add last value (if interval was not empty) */
    addfield(L, &b, i);
  luaL_addlstring(&b, ")", 1);
  luaL_pushresult(&b);
  return 1;
}

static int list_concat (lua_State *L) {
  int ll = aux_getn(L, 1);
  int lr = aux_getn(L, 2);
  int i, n = ll + lr;
  auxnewlist(L, n);
  for (n = 1, i = 1; i <= ll; i++) {
    lua_rawgeti(L, 1, i);
    lua_rawseti(L, -2, n++);
  }
  for (i = 1; i <= lr; i++) {
    lua_rawgeti(L, 2, i);
    lua_rawseti(L, -2, n++);
  }
  return 1;
}

static int list_equal (lua_State *L) {
  int ll = aux_getn(L, 1);
  int lr = aux_getn(L, 2);
  int eq;
  if (ll != lr) {
    lua_pushboolean(L, 0);
    return 1;
  }
  for (ll = 1; ll <= lr; ll++) {
    lua_rawgeti(L, 1, ll);
    lua_rawgeti(L, 2, ll);
    eq = lua_equal(L, -2, -1);
    lua_pop(L, 2);
    if (!eq) {
      lua_pushboolean(L, 0);
      return 1;
    }
  }
  lua_pushboolean(L, 1);
  return 1;
}

static int list_setn (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  int d = luaL_checkint(L, 2);
  if (d <= 0) {
    return luaL_error(L, "invalid index for list");
  }
  lua_settop(L, 3);
  lua_rawset(L, 1);
  return 0;
}


static const luaL_Reg list_funcs[] = {
  {"__tostring", list_tostring},
  {"__newindex", list_setn},
  {"__concat", list_concat},
  {"__eq", list_equal},
  {NULL, NULL}
};


static void createmetatable (lua_State *L) {
  luaL_newmetatable(L, LLIST_NAME);  /* create metatable for lists */
  lua_pushvalue(L, -2);  /* table methods */
  lua_setfield(L, -2, "__index");  /* mt.__index = metatable */
  luaL_register(L, NULL, list_funcs);
}

LUALIB_API int luaopen_list (lua_State *L) {
  createmetatable(L);
  lua_pop(L, 1);
  lua_pushcclosure(L, list_new, 0);
  lua_setfield(L, LUA_GLOBALSINDEX, LLIST_NAME);
  return 0;
}
