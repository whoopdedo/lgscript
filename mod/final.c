/*****************************************************************************
 * Finalizer module for Lua
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
#include "lua.h"
#include "lauxlib.h"

#include "mod/modlib.h"

#if LUA_VERSION_NUM<502
#define lua_rawlen lua_objlen
#define lua_setuservalue lua_setfenv
#define lua_getuservalue lua_getfenv
#define LUA_OK 0
#endif

#define FINALIZER_METATABLE	LFINALIZER_NAME ".env"

static void do_final (lua_State * L, int level, int protect)
{
	int finalizer, when;
	int top, stack = lua_gettop(L);
	int success = lua_isnil(L, stack-1) ? LMOD_FINSUCCESS : LMOD_FINERROR;
	/* for i = #stack, level, -1 do */
	for (top = lua_rawlen(L, -1); top > level; top--) {
	/* local finalizer = stack[i] */
	/* stack[i] = nil */
		lua_rawgeti(L, stack, top);
		lua_pushnil(L);
		lua_rawseti(L, stack, top);
	/* local when = finalizer[1] */
		lua_rawgeti(L, -1, 1);
		when = lua_tointeger(L, -1);
		lua_pop(L, 1);
	/* if not err and (when == nil or when == true) then */
	/*   caller(select(2, unpack(finalizer))) */
	/* elseif err and (when == nil or when == false) then */
	/*   finalizer[#finalizer+1] = err */
	/*   caller(select(2, unpack(finalizer))) */
	/* end */
		if (when == LMOD_FINALWAYS || when == success) {
			int nargs, n;
			finalizer = lua_gettop(L);
			lua_getfield(L, finalizer, "n");
			nargs = lua_tointeger(L, -1);
			lua_pop(L, 1);
			luaL_checkstack(L, nargs, NULL);
			for (n = 2; n <= nargs; n++) {
				lua_rawgeti(L, finalizer, n);
			}
			nargs -= 2;
			if (success == LMOD_FINERROR) {
				lua_pushvalue(L, stack-1);
				++nargs;
			}
			if (protect)
				lua_pcall(L, nargs, 0, 0);
			else
				lua_call(L, nargs, 0);
			lua_settop(L, finalizer);
		}
	/* end */
		lua_pop(L, 1);
	}
	lua_pop(L, 2);
}

static int gc_final (lua_State * L)
{
	lua_pushnil(L);
	lua_getuservalue(L, -2);
	do_final(L, 0, 1);
	return 0;
}

static int do_fcall (lua_State * L)
{
	int status;
	int errfunc = 0;
	int nargs = lua_gettop(L) - 1;
	if (!lua_isnil(L, lua_upvalueindex(1))) {
		lua_pushvalue(L, lua_upvalueindex(1));
		lua_insert(L, 1);
		errfunc = 1;
	}
	/* local result = {pcall(func,...)} */
#if LUA_VERSION_NUM>=502
	int ctx;
	status = lua_getctx(L, &ctx);
	if (status == LUA_OK)
		status = lua_pcallk(L, nargs, LUA_MULTRET, errfunc, 0, do_fcall);
#else
	status = lua_pcall(L, nargs, LUA_MULTRET, errfunc);
#endif
	/* local success = result[1] */
	/* local err */
	/* if not success then err = result[2] end */
	luaL_checkstack(L, 2, NULL);
	if (status != LUA_OK)
		lua_pushvalue(L, 1);
	else
		lua_pushnil(L);
	/* do_final(env._final, 0, err, true) */
	lua_pushvalue(L, lua_upvalueindex(2));
	do_final(L, 0, 1);
	/* return unpack(result) */
	lua_pushinteger(L, status);
	lua_insert(L, 1);
	return lua_gettop(L);
}

static int getfinalstack (lua_State * L)
{
	lua_Debug ar;
	int lvl;
	for (lvl = 1; lua_getstack(L, lvl, &ar); lvl++) {
		if (lua_getinfo(L, "f", &ar)) {
			if (lua_tocfunction(L, -1) == do_fcall) {
				lua_getupvalue(L, -1, 2);
				lua_replace(L, -2);
				return 1;
			}
			lua_pop(L, 1);
		}
	}
	return 0;
}

LUA_API int lmod_finally (lua_State *L, int nargs, int when) {
	int level;
	int gcfunc;
	int idx, arg = lua_gettop(L) - nargs;
	gcfunc = 0;
	/* [arg]=func 1arg 2arg .. narg */
	if (!lua_isfunction(L, arg)) {
		lua_getfield(L, arg, "close");
		if (lua_isfunction(L, -1)) {
			gcfunc = 1;
		}
		else {
			lua_pop(L, 1);
			if (lua_getmetatable(L, arg)) {
				lua_getfield(L, -1, "__gc");
				if (lua_isfunction(L, -1)) {
					lua_replace(L, -2);
					gcfunc = 1;
				}
				else
					return luaL_error(L, "finalizer is not a function");
			}
			else
				return luaL_error(L, "finalizer is not a function");
		}
	}
	/* [arg]=func 1arg 2arg .. narg gcfunc? */
	if (!getfinalstack(L)) {
		return luaL_error(L, "no fcall on stack");
	}
	level = lua_rawlen(L, -1);
	/* [arg]=func 1arg 2arg .. narg gcfunc? finstack */
	idx = nargs + gcfunc + 2;
	lua_createtable(L, idx, 1);
	lua_pushinteger(L, idx);
	lua_setfield(L, -2, "n");
	lua_pushinteger(L, when);
	lua_rawseti(L, -2, 1);
	/* [arg]=func 1arg 2arg .. narg gcfunc? finstack fintable{when} */
	lua_pushvalue(L, arg);
	if (gcfunc) {
		lua_pushvalue(L, -4);
		lua_rawseti(L, -3, 2);
		lua_rawseti(L, -2, 3);
	}
	else
		lua_rawseti(L, -2, 2);
	/* [arg]=func 1arg 2arg .. narg gcfunc? finstack fintable{when} */
	for (; nargs > 0; nargs--) {
		lua_pushvalue(L, arg+nargs);
		lua_rawseti(L, -2, idx--);
	}
	lua_rawseti(L, -2, ++level);
	lua_settop(L, arg-1);
	return level;
}

LUA_API void lmod_finalize (lua_State *L, int base, int protect) {
	if (!getfinalstack(L)) {
		luaL_error(L, "no fcall on stack");
	}
	do_final(L, base, protect);
}

LUA_API int lmod_fcall (lua_State *L, int nargs, int nresults, int errfunc
#if LUA_VERSION_NUM>=502
	, lua_CFunction cfunc
#endif
) {
	int status;
	int func = lua_gettop(L) - nargs;
	if (errfunc)
		lua_pushvalue(L, errfunc);
	else
		lua_pushnil(L);
	/* local env = {_final={}} */
	lua_createtable(L, 0, 0);
	/* local proxy = newproxy(true) */
	/* getmetatable(proxy).__gc = function() do_final(env._final) end */
	lua_newuserdata(L, 0);
	luaL_getmetatable(L, FINALIZER_METATABLE);
	lua_setmetatable(L, -2);
	lua_pushvalue(L, -2);
	lua_setuservalue(L, -2);
	lua_setfield(L, -2, "proxy");
	/* setfenv(1, setmetatable(env,{__index=_G})) */
	lua_pushcclosure(L, do_fcall, 2);
	lua_insert(L, func);
#if LUA_VERSION_NUM<502
	lua_call(L, nargs+1, LUA_MULTRET);
#else
	lua_callk(L, nargs+1, LUA_MULTRET, 0, cfunc);
#endif
	status = lua_tointeger(L, func);
	lua_remove(L, func);
	if (status == LUA_OK && nresults != LUA_MULTRET)
		lua_settop(L, func+nresults-1);
	return status;
}

LUA_API int lmod_getfinalframe (lua_State *L) {
	int level;
	if (!getfinalstack(L)) {
		return luaL_error(L, "no fcall on stack");
	}
	level = lua_rawlen(L, -1);
	lua_pop(L, 1);
	return level;
}


static int l_fcall (lua_State *L)
{
	int status;
	int nargs = lua_gettop(L);
#if LUA_VERSION_NUM>=502
	int ctx;
	if (lua_getctx(L, &ctx) == LUA_YIELD)
		return nargs;
#endif
	luaL_checkany(L, 1);
#if LUA_VERSION_NUM<502
	status = lmod_fcall(L, nargs-1, LUA_MULTRET, 0);
#else
	status = lmod_fcall(L, nargs-1, LUA_MULTRET, 0, l_fcall);
#endif
	lua_pushboolean(L, (status == LUA_OK));
	lua_insert(L, 1);
	return lua_gettop(L);
}

static int l_finalize (lua_State *L)
{
	lua_settop(L, 2);
	lmod_finalize(L, luaL_optinteger(L, 1, 0), 0);
	return 0;
}

static int l_finally (lua_State *L)
{
	int level;
	int nargs = lua_gettop(L);
	if (nargs == 0)
		level = lmod_getfinalframe(L);
	else
		level = lmod_finally(L, nargs-1, LMOD_FINALWAYS);
	lua_pushinteger(L, level);
	return 1;
}

static int l_onerror (lua_State *L)
{
	int level;
	luaL_checkany(L, 1);
	level = lmod_finally(L, lua_gettop(L)-1, LMOD_FINERROR);
	lua_pushinteger(L, level);
	return 1;
}

static int l_onreturn (lua_State *L)
{
	int level;
	luaL_checkany(L, 1);
	level = lmod_finally(L, lua_gettop(L)-1, LMOD_FINSUCCESS);
	lua_pushinteger(L, level);
	return 1;
}

static const luaL_Reg l_finalizer_reg[] =
{
	{"fcall", l_fcall},
	{"finalize", l_finalize},
	{"finally", l_finally},
	{"onerror", l_onerror},
	{"onreturn", l_onreturn},
	{NULL, NULL}
};


LUALIB_API int luaopen_finalizer (lua_State *L)
{
	if (luaL_newmetatable(L, FINALIZER_METATABLE)) {
		lua_pushcfunction(L, gc_final);
		lua_setfield(L, -2, "__gc");
	}
	lua_pop(L, 1);
#if LUA_VERSION_NUM<502
	luaL_register(L, LFINALIZER_NAME, l_finalizer_reg);
#else
	luaL_newlib(L, l_finalizer_reg);
#endif
	return 1;
}
