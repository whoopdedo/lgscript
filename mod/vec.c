/******************************************************************************
 * Vector library for Lua
 * Copyright (C) 2011 Tom N Harris. All rights reserved.
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *  4. Neither the names of the authors nor the names of any of the software
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 */
#include <math.h>

#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include "mod/modlib.h"

LUA_API lvector* lmod_tovector (lua_State *L, int arg) {
  lvector* vec = (lvector*)lua_touserdata(L, arg);
  if (vec != NULL) {
    if (lua_getmetatable(L, arg)) {
      lua_getfield(L, LUA_REGISTRYINDEX, LVECTOR_NAME);
      if (!lua_rawequal(L, -1, -2)) {
        vec = NULL;
      }
      lua_pop(L, 2);
    }
  }
  return vec;
}

LUA_API lvector* lmod_newvector (lua_State *L, lua_Number x, lua_Number y, lua_Number z) {
  lvector* vec = (lvector*)lua_newuserdata(L, sizeof(lvector));
  lua_getfield(L, LUA_REGISTRYINDEX, LVECTOR_NAME);
  lua_setmetatable(L, -2);
  vec->v.x = x;
  vec->v.y = y;
  vec->v.z = z;
  return vec;
}

static int lvector_new (lua_State* L) {
  lua_Number x,y,z;
  int arg = lua_istable(L, 1) ? 1 : 0;
  lua_settop(L, arg+3);
  x = luaL_optnumber(L, arg+1, 0.0);
  y = luaL_optnumber(L, arg+2, 0.0);
  z = luaL_optnumber(L, arg+3, 0.0);
  lmod_newvector(L, x, y, z);
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
  int c;
  lua_Number e;
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  lua_getfield(L, 1, "epsilon");
  e = lua_tonumber(L, -1);
  c = auxequal(a->v.x,b->v.x,e) && auxequal(a->v.y,b->v.y,e) && auxequal(a->v.z,b->v.z,e);
  lua_pushboolean(L, c);
  return 1;
}

static int lvector_add (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  lmod_newvector(L, a->v.x+b->v.x, a->v.y+b->v.y, a->v.z+b->v.z);
  return 1;
}

static int lvector_sub (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lvector* b = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  lmod_newvector(L, a->v.x-b->v.x, a->v.y-b->v.y, a->v.z-b->v.z);
  return 1;
}

static int lvector_unm (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lmod_newvector(L, -a->v.x, -a->v.y, -a->v.z);
  return 1;
}

static int lvector_mul (lua_State* L) {
  lvector* a;
  lua_Number b;
  if (lua_isnumber(L, 1)) {
    b = luaL_checknumber(L, 1);
    a = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
  }
  else {
    a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
    if (lua_isnumber(L, 2))
      b = luaL_checknumber(L, 2);
    else {
      lvector* c = (lvector*)luaL_checkudata(L, 2, LVECTOR_NAME);
      lmod_newvector(L, a->v.x*c->v.x, a->v.y*c->v.y, a->v.z*c->v.z);
      return 1;
    }
  }
  lmod_newvector(L, a->v.x*b, a->v.y*b, a->v.z*b);
  return 1;
}

static lua_Number auxmag2 (lvector* v) {
  return v->v.x*v->v.x + v->v.y*v->v.y + v->v.z*v->v.z;
}

static int lvector_mag (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_pushnumber(L, sqrt(auxmag2(a)));
  return 1;
}

static int lvector_mag2 (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_pushnumber(L, auxmag2(a));
  return 1;
}

static int lvector_norm (lua_State* L) {
  lvector* a = (lvector*)luaL_checkudata(L, 1, LVECTOR_NAME);
  lua_Number i,j,k,l,m,e;
  i = fabs(a->v.x); j = fabs(a->v.y); k = fabs(a->v.z);
  if (j > i) {
    lua_Number t = i;
    i = j; j = t;
  }
  if (k > i) {
    lua_Number t = i;
    i = k; k = t;
  }
  l = i * i;
  lua_getfield(L, 1, "epsilon");
  e = lua_tonumber(L, -1);
  if (l >= e) {
    m = sqrt((((j*j) + (k*k)) / l) + 1.0) * i;
    l = a->v.x / m; i = (fabs(l) < e) ? 0.0 : l;
    l = a->v.y / m; j = (fabs(l) < e) ? 0.0 : l;
    l = a->v.z / m; k = (fabs(l) < e) ? 0.0 : l;
    lmod_newvector(L, i, j, k);
  }
  else
    lmod_newvector(L, 0, 0, 0);
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
  lmod_newvector(L, a->v.y*b->v.z - a->v.z*b->v.y,
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
  vec->a[ix-1] = luaL_checknumber(L, 3);
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
  {"mag2", lvector_mag2},
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

