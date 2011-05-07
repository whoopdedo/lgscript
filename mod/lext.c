#include <stdlib.h>
#include <math.h>

#include "lua.h"
#include "lauxlib.h"

#ifndef lua_pushglobaltable
#define lua_pushglobaltable(L)	lua_pushvalue(L, LUA_GLOBALSINDEX)
#endif

#define max(a,b)	((a)>=(b) ? (a) : (b))

#define aux_getn(L,n)	(luaL_checktype(L, n, LUA_TTABLE), luaL_getn(L, n))

static int t_insert (lua_State *L) {
  int e = aux_getn(L, 1) + 1;
  int pos;
  int num = lua_gettop(L);
  switch (num) {
    case 0: case 1: {
      return luaL_error(L, "wrong number of arguments to " LUA_QL("insert"));
    }
    case 2: {
      pos = e;
      num = 1;
      break;
    }
    default: {
      int i;
      num -= 2;
      pos = luaL_checkint(L, 2);
      if (pos > e) e = pos;
      for (i = e+num-1; i > pos; i--) {
        lua_rawgeti(L, 1, i-num);
        lua_rawseti(L, 1, i);
      }
      pos = pos + num - 1;
      break;
    }
  }
  luaL_setn(L, 1, e);  /* new size */
  while (num-- > 0)
    lua_rawseti(L, 1, pos--);  /* t[pos] = v */
  lua_settop(L, 2);
  return 1;
}

static int t_remove (lua_State *L) {
  int e = aux_getn(L, 1);
  int pos = luaL_optint(L, 2, e);
  int last = luaL_optint(L, 3, pos);
  int i;
  if (last < pos) return 0;
  if (!(1 <= last && pos <= e))
   return 0;
  if (pos < 1) pos = 1;
  if (last > e) last = e;
  int n = last - pos + 1;
  luaL_setn(L, 1, e - n);
  for (i=0; i<n; i++)
    lua_rawgeti(L, 1, pos+i);
  for (last++; last<=e; pos++,last++) {
    lua_rawgeti(L, 1, last);
    lua_rawseti(L, 1, pos);
  }
  for( ; pos<=e; pos++) {
    lua_pushnil(L);
    lua_rawseti(L, 1, pos);
  }
  return n;
}

static int posrelat (int pos, size_t len) {
  if (pos < 0) pos += len + 1;
  return (pos >= 0) ? pos : 0;
}

static int t_slice (lua_State *L) {
  int n = aux_getn(L, 1);
  int start = posrelat(luaL_checkint(L, 2), n);
  int end = posrelat(luaL_optint(L, 3, -1), n);
  int skip = luaL_optint(L, 4, 1);
  luaL_argcheck(L, skip != 0, 4, "step argument must not be zero");
  if (start < 1) start = 1;
  if (end > n) end = n;

  int i, ni;
  if (skip > 0)
    ni = (end >= start) ? ((end - start) / skip) + 1 : 0;
  else  /* skip < 0 */
    ni = (start >= end) ? ((start - end) / -skip) + 1 : 0;
  lua_createtable(L, ni, 0);
  for (i = 1; i <= ni; ++i) {
    lua_rawgeti(L, 1, start);
    lua_rawseti(L, -2, i);
    start += skip;
  }
  
  return 1;
}

static int t_find (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
#if 1
  lua_settop(L, 3);
  if (!lua_find(L, 1)) {
    lua_pushnil(L);
  }
#else
  if (!lua_isnoneornil(L, 3))
    lua_pushvalue(L, 3);
  else
    lua_pushnil(L);
  while (lua_next(L, 1)) {
    if (lua_equal(L, -1, 2)) {
      lua_pop(L, 1);
      return 1;
    }
    lua_pop(L, 1);
  }
  lua_pushnil(L);
#endif
  return 1;
}

static int t_extend (lua_State *L) {
  int nl = aux_getn(L, 1);
  int nr = aux_getn(L, 2);
  int n = 1;
  while (n <= nr) {
    lua_rawgeti(L, 2, n++);
    lua_rawseti(L, 1, ++nl);
  }
  if (nr > 0)
    lua_pushinteger(L, nr);
  else
    lua_pushnil(L);
  while (lua_next(L, 2) != 0) {
    lua_pushvalue(L, -2);
    lua_insert(L, -3);
    lua_rawset(L, 1);
  }
  return 0;
}

static int t_merge (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  int nr = aux_getn(L, 2);
  int n = 1;
  for (; n <= nr; n++) {
    lua_rawgeti(L, 2, n);
    lua_rawseti(L, 1, n);
  }
  if (nr > 0)
    lua_pushinteger(L, nr);
  else
    lua_pushnil(L);
  while (lua_next(L, 2) != 0) {
    lua_pushvalue(L, -2);
    lua_insert(L, -2);
    lua_rawset(L, 1);
  }
  return 0;
}


static int t_keys (lua_State *L) {
  int n = aux_getn(L, 1);
  int i = 1;
  lua_createtable(L, n, 0);
  for (; i <=n; i++) {
    lua_pushinteger(L, i);
    lua_rawseti(L, -2, i);
  }
  if (n > 0)
    lua_pushinteger(L, n);
  else
    lua_pushnil(L);
  while (lua_next(L, 1) != 0) {
    lua_pop(L, 1);
    lua_pushvalue(L, -1);
    lua_rawseti(L, -3, i++);
  }
  return 1;
}

static int t_values (lua_State *L) {
  int n = aux_getn(L, 1);
  int i = 1;
  lua_createtable(L, n, 0);
  for (; i <= n; i++) {
    lua_rawgeti(L, 1, i);
    lua_rawseti(L, -2, i);
  }
  if (n > 0)
    lua_pushinteger(L, n);
  else
    lua_pushnil(L);
  while (lua_next(L, 1) != 0) {
    lua_rawseti(L, -3, i++);
  }
  return 1;
}

static void get2 (lua_State *L, int i, int j) {
  lua_rawgeti(L, 1, i);
  lua_rawgeti(L, 1, j);
}

static void set2 (lua_State *L, int i, int j) {
  lua_rawseti(L, 1, i);
  lua_rawseti(L, 1, j);
}

static int t_reverse (lua_State *L) {
  int u = aux_getn(L, 1);
  int l = 1;
  while (l < u) {
    get2(L, l, u);
    set2(L, l, u);
    ++l; --u;
  }
  return 0;
}

static int g_seq (lua_State *L) {
  int n = lua_gettop(L);
  luaL_checkany(L, 1);
  lua_Number l,u,s = 1;
  int i, ni;
  if (n >= 3) {
    s = luaL_checknumber(L, 3);
    luaL_argcheck(L, s != 0, 3, "step argument must not be zero");
  }
  if (n >= 2) {
    l = luaL_checknumber(L, 1);
    u = luaL_checknumber(L, 2);
    if (s > 0)
      ni = (u >= l) ? (int)((u - l) / s) + 1 : 0;
    else
      ni = (l >= u) ? (int)((l - u) / -s) + 1 : 0;
  }
  else {
    u = luaL_checknumber(L, 1);
	l = 1.0;
	if (u < 0) { u = 0; ni = 0; }
	else ni = (int)u;
  }
  lua_settop(L, 0);
  if (!lua_checkstack(L, ni))
    return luaL_error(L, "too many results in sequence");
  for (i = 1; i <= ni; ++i) {
    lua_pushnumber(L, l);
	l += s;
  }
  return ni;
}

static int g_select (lua_State *L) {
  int n = lua_gettop(L);
  if (lua_type(L, 1) == LUA_TSTRING && *lua_tostring(L, 1) == '#') {
    lua_pushinteger(L, n-1);
    return 1;
  }
  else {
    int i = luaL_checkint(L, 1);
	if (i <= 0) {
      i = max(n + i, 0);
    }
    else if (i > n) i = n;
    return n - i;
  }
}

static int g_shift (lua_State *L) {
  int n = lua_gettop(L);
  if (lua_type(L, 1) == LUA_TSTRING && *lua_tostring(L, 1) == '#') {
    lua_pushinteger(L, n-1);
    return 1;
  }
  else {
    int i = luaL_checkint(L, 1);
    if (i < 0) {
      i = max(n + i, 0);
    }
    else if (i >= n) i = n - 1;
    luaL_argcheck(L, 0 <= i, 1, "index out of range");
    lua_settop(L, i + 1);
    return i;
  }
}

#define _round(x)  (((x) >= 0.0)?floor((x)+0.5):ceil((x)-0.5))
static int math_round (lua_State *L) {
  lua_Number r = luaL_checknumber(L, 1);
  lua_Number f = 1.0;
  int n = luaL_optint(L, 2, 0);
  int i = abs(n);
  while (--i >= 0)
    f = f*10.0;
  if (n < 0)
    r = _round(r/f) * f;
  else
    r = _round(r*f) / f;
  lua_pushnumber(L, r);
  return 1;
}


static const luaL_Reg extlib[] = {
  {"seq", g_seq},
  {"select", g_select},
  {"shift", g_shift},
  {NULL, NULL}
};

static const luaL_Reg extmath[] = {
  {"round", math_round},
  {NULL, NULL}
};

static const luaL_Reg exttable[] = {
  {"insert", t_insert},
  {"remove", t_remove},
  {"find", t_find},
  {"keys", t_keys},
  {"values", t_values},
  {"reverse", t_reverse},
  {"slice", t_slice},
  {"extend", t_extend},
  {"merge", t_merge},
  {NULL, NULL}
};

LUALIB_API int luaopen_ext (lua_State *L) {
  lua_pushglobaltable(L);
  luaL_register(L, NULL, extlib);
  lua_pop(L, 1);
  lua_getglobal(L, "math");
  luaL_register(L, NULL, extmath);
  lua_pop(L, 1);
  lua_getglobal(L, "table");
  luaL_register(L, NULL, exttable);
  lua_pop(L, 1);
  return 0;
}
