/******************************************************************************
 * Extension module for Lua
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
#ifndef lvector_h
#define lvector_h

#include "lua.h"

struct lvector {
	union {
	lua_Number a[3];
	struct { lua_Number x,y,z; } v;
	};
};
typedef struct lvector lvector;

#define LVECTOR_NAME		"vector"
#define LVECTOR_EPSILON		1e-10

LUA_API lvector* lmod_newvector (lua_State *L, lua_Number x, lua_Number y, lua_Number z);

#define LLIST_NAME		"list"

#define LFINALIZER_NAME		"finalizer"
#define LMOD_FINALWAYS		0
#define LMOD_FINSUCCESS		1
#define LMOD_FINERROR		2

LUA_API int lmod_finally (lua_State *L, int nargs, int when);  /* [-(nargs+1), +0, e] */
LUA_API void lmod_finalize (lua_State *L, int base, int protect);  /* [-1, +0, e] */
#if LUA_VERSION_NUM<502
LUA_API int lmod_fcall (lua_State *L, int nargs, int nresults, int errfunc);  /* [-(nargs+1), +nresults, -] */
#else
LUA_API int lmod_fcall (lua_State *L, int nargs, int nresults, int errfunc, lua_CFunction cfunc);
#endif
LUA_API int lmod_getfinalframe (lua_State *L);  /* [-0, +0, e] */


LUALIB_API int luaopen_vector (lua_State* L);
LUALIB_API int luaopen_list (lua_State* L);
LUALIB_API int luaopen_ext (lua_State* L);
LUALIB_API int luaopen_finalizer (lua_State *L);

#endif
