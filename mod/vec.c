/******************************************************************************
 *    vec.c
 *
 *    Copyright (C) 2011 Tom N Harris <telliamed@whoopdedo.org>
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to
 *    deal in the Software without restriction, including without limitation the
 *    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *    sell copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *    IN THE SOFTWARE.
 *
 *****************************************************************************/
#include <math.h>

#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include "mod/modlib.h"

static lvector* auxnew (lua_State* L, lua_Number x, lua_Number y, lua_Number z) {
  lvector* vec = (lvector*)lua_newuserdata(L, sizeof(lvector));
  lua_getfield(L, LUA_REGISTRYINDEX, LVECTOR_NAME);
  lua_setmetatable(L, -2);
  vec->v.x = x;
  vec->v.y = y;
  vec->v.z = z;
  return vec;
}

static int lvector_new (lua_State* L) {
  int arg = lua_istable(L, 1) ? 1 : 0;
  lua_settop(L, arg+3);
  lua_Number x = luaL_optnumber(L, arg+1, 0.0);
  lua_Number y = luaL_optnumber(L, arg+2, 0.0);
  lua_Number z = luaL_optnumber(L, arg+3, 0.0);
  auxnew(L, x, y, z);
  return 1;
}

static int lvector_tostring (lua_State* L) {
  lvector* vec = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addlstring(&b, "vector(", 7);
  lua_pushnumber(L, vec->v.x);
  luaL_addvalue(&b);
  luaL_addlstring(&b, ", ", 2);
  lua_pushnumber(L, vec->v.y);
  luaL_addvalue(&b);
  luaL_addlstring(&b, ", ", 2);
  lua_pushnumber(L, vec->v.z);
  luaL_addvalue(&b);
  luaL_addchar(&b, ')');
  luaL_pushresult(&b);
  return 1;
}

static int auxequal (lua_Number a, lua_Number b, lua_Number e) {
  return e > fabs(a-b);
}

static int lvector_eq (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  lua_getfield(L, 1, "epsilon");
  lua_Number e = lua_tonumber(L, -1);
  int c = auxequal(a->v.x,b->v.x,e) && auxequal(a->v.y,b->v.y,e) && auxequal(a->v.z,b->v.z,e);
  lua_pushboolean(L, c);
  return 1;
}

static int lvector_add (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  auxnew(L, a->v.x+b->v.x, a->v.y+b->v.y, a->v.z+b->v.z);
  return 1;
}

static int lvector_sub (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  auxnew(L, a->v.x-b->v.x, a->v.y-b->v.y, a->v.z-b->v.z);
  return 1;
}

static int lvector_unm (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  auxnew(L, -a->v.x, -a->v.y, -a->v.z);
  return 1;
}

static int lvector_mul (lua_State* L) {
  lvector* a;
  lua_Number b;
  if (lua_isuserdata(L, 1)) {
    a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
    b = luaL_checknumber(L, 2);
  }
  else {
    b = luaL_checknumber(L, 1);
    a = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  }
  auxnew(L, a->v.x*b, a->v.y*b, a->v.z*b);
  return 1;
}

static lua_Number auxmag (lvector* v) {
  lua_Number m = v->v.x*v->v.x + v->v.y*v->v.y + v->v.z*v->v.z;
  return sqrt(m);
}

static int lvector_mag (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_pushnumber(L, auxmag(a));
  return 1;
}

static int lvector_norm (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_Number m = auxmag(a);
  lua_getfield(L, 1, "epsilon");
  lua_Number e = lua_tonumber(L, -1);
  if (m >= e) {
    m = 1.0/m;
    auxnew(L, a->v.x*m, a->v.y*m, a->v.z*m);
  }
  else
    auxnew(L, 0, 0, 0);
  return 1;
}

static int lvector_dot (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  lua_Number c = a->v.x*b->v.x + a->v.y*b->v.y * a->v.z*b->v.z;
  lua_pushnumber(L, c);
  return 1;
}

static int lvector_cross (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  auxnew(L, a->v.y*b->v.z - a->v.z*b->v.y,
            a->v.z*b->v.x - a->v.x*b->v.z,
            a->v.x*b->v.y - a->v.y*b->v.x);
  return 1;
}

static int lvector_len (lua_State* L) {
  (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_pushinteger(L, 3);
  return 1;
}

static int lvector_newindex (lua_State* L) {
  lvector* vec = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_Number n = luaL_checknumber(L, 3);
  int ix = 0;
  if (lua_type(L, 2) == LUA_TSTRING) {
    size_t sz;
    const char* k = lua_tolstring(L, 2, &sz);
    if (sz==1) switch(k[0]) {
      case 'X': case 'x': case 'R': case 'r': case '1': ix = 1; break;
      case 'Y': case 'y': case 'G': case 'g': case '2': ix = 2; break;
      case 'Z': case 'z': case 'B': case 'b': case '3': ix = 3; break;
    }
  }
  if (ix == 0) {
    ix = luaL_checkint(L, 2);
    if (ix < 1 || ix > 3)
      luaL_argerror(L, 2, "index out of range");
  }
  vec->a[ix-1] = n;
  return 1;
}

static int lvector_index (lua_State* L) {
  lvector* vec = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_pushvalue(L, 2);
  lua_rawget(L, lua_upvalueindex(1));
  if (!lua_isnil(L, -1)) {
    return 1;
  }
  lua_pop(L, 1);
  int ix = 0;
  if (lua_type(L, 2) == LUA_TSTRING) {
    size_t sz;
    const char* k = lua_tolstring(L, 2, &sz);
    if (sz==1) switch(k[0]) {
      case 'X': case 'x': case 'R': case 'r': case '1': ix = 1; break;
      case 'Y': case 'y': case 'G': case 'g': case '2': ix = 2; break;
      case 'Z': case 'z': case 'B': case 'b': case '3': ix = 3; break;
    }
  }
  if (ix == 0) {
    ix = luaL_checkint(L, 2);
    if (ix < 1 || ix > 3)
      luaL_argerror(L, 2, "index out of range");
  }
  lua_pushnumber(L, vec->a[ix-1]);
  return 1;
}

static const luaL_Reg lvector_meta[] = {
  {"__newindex", lvector_newindex},
  {"__len", lvector_len},
  {"__tostring", lvector_tostring},
  {"__eq", lvector_eq},
  {"__add", lvector_add},
  {"__sub", lvector_sub},
  {"__unm", lvector_unm},
  {"__mul", lvector_mul},
  {NULL, NULL}
};
#define NUM_META     ((sizeof(lvector_meta)/sizeof(luaL_Reg))-1)

static const luaL_Reg lvector_methods[] = {
  {"mag", lvector_mag},
  {"norm", lvector_norm},
  {"dot", lvector_dot},
  {"cross", lvector_cross},
  {"new", lvector_new},
  {"__call", lvector_new},
  {NULL, NULL}
};
#define NUM_METHODS     ((sizeof(lvector_methods)/sizeof(luaL_Reg))-1)

LUALIB_API int luaopen_vector (lua_State* L) {
  lua_createtable(L, 0, NUM_METHODS);
  lua_pushvalue(L, -1);
  lua_setmetatable(L, -2);
  luaL_register(L, NULL, lvector_methods);
  lua_pushnumber(L, LVECTOR_EPSILON);
  lua_setfield(L, -2, "epsilon");
  luaL_newmetatable(L, LVECTOR_NAME);
  luaL_register(L, NULL, lvector_meta);
  lua_pushvalue(L, -2);
  lua_pushcclosure(L, lvector_index, 1);
  lua_setfield(L, -2, "__index");
  lua_pushvalue(L, -2);
  lua_setglobal(L, LVECTOR_NAME);
  lua_pop(L, 1);
  return 1;
}

