/******************************************************************************
 * Bitwise arithmetic library for Lua
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
#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include "mod/modlib.h"

/* number of bits to consider in a number */
#define NBITS	32

#define ALLONES		(~(((~(unsigned long)0) << (NBITS - 1)) << 1))

/* mask to trim extra bits */
#define trim(x)		((x) & ALLONES)

typedef LUAI_UINT32 uint;

#define getuintarg(L,arg)	(uint)luaL_checknumber(L,arg)

#define lua_pushunsigned(L,u)	lua_pushnumber(L,(lua_Number)(u))


static uint andaux (lua_State *L) {
  int i, n = lua_gettop(L);
  uint r = ~(uint)0;
  for (i = 1; i <= n; i++)
    r &= getuintarg(L, i);
  return trim(r);
}

static int l_and (lua_State *L) {
  uint r = andaux(L);
  lua_pushunsigned(L, r);
  return 1;
}

static int l_test (lua_State *L) {
  uint r = andaux(L);
  lua_pushboolean(L, r != 0);
  return 1;
}

static int l_or (lua_State *L) {
  int i, n = lua_gettop(L);
  uint r = 0;
  for (i = 1; i <= n; i++)
    r |= getuintarg(L, i);
  lua_pushunsigned(L, trim(r));
  return 1;
}

static int l_xor (lua_State *L) {
  int i, n = lua_gettop(L);
  uint r = 0;
  for (i = 1; i <= n; i++)
    r ^= getuintarg(L, i);
  lua_pushunsigned(L, trim(r));
  return 1;
}

static int l_not (lua_State *L) {
  uint r = ~getuintarg(L, 1);
  lua_pushunsigned(L, trim(r));
  return 1;
}

static int bitrotate (lua_State *L, int i) {
  uint r = getuintarg(L, 1);
  i &= (NBITS - 1);
  r = trim(r);
  r = (r << i) | (r >> (NBITS - i));
  lua_pushunsigned(L, trim(r));
  return 1;
}

static int l_lrot (lua_State *L) {
  return bitrotate(L, luaL_checkint(L, 2));
}

static int l_rrot (lua_State *L) {
  return bitrotate(L, -luaL_checkint(L, 2));
}

static int bitshift (lua_State *L, uint r, int i) {
  if (i < 0) {  /* shift right */
    i = -i;
    r = trim(r);
    if (i >= NBITS) r = 0;
    else r >>= i;
  }
  else {  /* shift left */
    if (i >= NBITS) r = 0;
    else r <<= i;
    r = trim(r);
  }
  lua_pushunsigned(L, r);
  return 1;
}

static int l_lshift (lua_State *L) {
  return bitshift(L, getuintarg(L, 1), luaL_checkint(L, 2));
}

static int l_rshift (lua_State *L) {
  return bitshift(L, getuintarg(L, 1), -luaL_checkint(L, 2));
}

static int l_arshift (lua_State *L) {
  uint r = getuintarg(L, 1);
  int i = luaL_checkint(L, 2);
  if (i < 0 || !(r & ((uint)1 << (NBITS - 1))))
    return bitshift(L, r, -i);
  else {  /* arithmetic shift for 'negative' number */
    if (i >= NBITS) r = ALLONES;
    else
      r = trim((r >> i) | ~(~(uint)0 >> i));  /* add signal bit */
  }
  lua_pushunsigned(L, r);
  return 1;
}

static const luaL_Reg l_bitlib_reg[] = {
  {"arshift", l_arshift},
  {"band", l_and},
  {"bnot", l_not},
  {"bor", l_or},
  {"btest", l_test},
  {"bxor", l_xor},
  {"lrotate", l_lrot},
  {"lshift", l_lshift},
  {"rrotate", l_rrot},
  {"rshift", l_rshift},
  {NULL, NULL}
};

LUALIB_API int luaopen_bit32 (lua_State *L) {
  luaL_register(L, LBITLIB_NAME, l_bitlib_reg);
  return 1;
}

