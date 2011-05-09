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
  int n = 0;
  if (!lua_isnoneornil(L, 1))
    n = aux_getn(L, 1);
  int i;
  auxnewlist(L, n);
  for (i = 1; i <= n; i++) {
    lua_rawgeti(L, 1, i);
    lua_rawseti(L, -2, i);
  }
  return 1;
}

static void addfield (lua_State *L, luaL_Buffer *b, int i) {
  lua_rawgeti(L, 1, i);
  switch (lua_type(L, -1)) {
    case LUA_TNUMBER:
    case LUA_TSTRING:
      luaL_addvalue(b);
      break;
    case LUA_TBOOLEAN:
      luaL_addstring(b, lua_toboolean(L, -1) ? "true" : "false");
      lua_pop(L, 1);
      break;
    case LUA_TNIL:
      luaL_error(L, "invalid value (nil) at index %d in table for "
                    LUA_QL("tostring"), i);
      break;
    default:
      lua_pushfstring(L, "%s: %p", luaL_typename(L, -1), lua_topointer(L, -1));
      luaL_addvalue(b);
      lua_pop(L, 1);
      break;
  }
}

static int list_tostring (lua_State *L) {
  int n = aux_getn(L, 1);
  int i;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addlstring(&b, "list{", 5);
  for (i = 1; i < n; i++) {
    addfield(L, &b, i);
    luaL_addlstring(&b, ",", 1);
  }
  if (i == n)  /* add last value (if interval was not empty) */
    addfield(L, &b, i);
  luaL_addlstring(&b, "}", 1);
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

static int list_extend (lua_State *L) {
  int nl = aux_getn(L, 1);
  int nr = aux_getn(L, 2);
  int n = 1;
  while (n <= nr) {
    lua_rawgeti(L, 2, n++);
    lua_rawseti(L, 1, ++nl);
  }
  return 0;
}

static int list_insert (lua_State *L) {
  int e = aux_getn(L, 1) + 1;  /* first empty element */
  int num = lua_gettop(L);
  if (num >= 3) {
    int pos = luaL_checkint(L, 2);
    if (0 >= pos || pos > e)
      return luaL_argerror(L, 2, "index out of range");
  }
  return tinsert(L);
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
  int e = aux_getn(L, 1);  /* first empty element */
  int pos = luaL_checkint(L, 2);
  if (0 >= pos || pos > (e+1))
    return luaL_error(L, "index out of range");
  lua_settop(L, 3);
  if (lua_isnil(L, 3)) {
    luaL_setn(L, 1, e - 1);  /* t.n = t.n-1 */
    for (; pos<e; pos++) {
      lua_rawgeti(L, 1, pos+1);
      lua_rawseti(L, 1, pos);  /* t[pos] = t[pos+n] */
    }
    lua_pushnil(L);
    lua_rawseti(L, 1, e);  /* t[e] = nil */
  }
  else
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
