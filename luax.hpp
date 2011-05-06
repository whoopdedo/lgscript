/******************************************************************************
 *    luax.hpp
 *
 *    Copyright (C) 2009 Tom N Harris <telliamed@whoopdedo.org>
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
#ifndef LUAXX_H
#define LUAXX_H

#include <string>
#include <exception>
#include <cstdarg>

#include "luax.h"

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

namespace luax
{
	class State;
	class Buffer;

	typedef lua_Integer Integer;
	typedef lua_Number Number;
	typedef lua_CFunction CFunction;
	typedef lua_Alloc Alloc;
	typedef lua_Reader Reader;
	typedef lua_Writer Writer;
	typedef luaL_Reg Registry;

	enum Type
	{
		TNone = LUA_TNONE,
		TNil = LUA_TNIL,
		TBoolean = LUA_TBOOLEAN,
		TNumber = LUA_TNUMBER,
		TInteger = LUA_TINTEGER,
		TString = LUA_TSTRING,
		TTable = LUA_TTABLE,
		TFunction = LUA_TFUNCTION,
		TUserdata = LUA_TUSERDATA,
		TLightUserdata = LUA_TLIGHTUSERDATA,
		TThread = LUA_TTHREAD
	};

	enum ErrCode
	{
		NoErr = 0,
		Yielded = LUA_YIELD,
		ErrRun = LUA_ERRRUN,
		ErrSyntax = LUA_ERRSYNTAX,
		ErrMem = LUA_ERRMEM,
		ErrErr = LUA_ERRERR,
		ErrFile = LUA_ERRFILE,
		ErrStack,
		ErrType
	};

	enum Reference
	{
		RefNil = LUA_REFNIL,
		NoRef = LUA_NOREF
	};

	extern const int MultiRet;

	class Exception : public std::exception
	{
	protected:
		std::string err;
	public:
		virtual ~Exception() throw();
		explicit Exception(const char* _s)
			: err(_s) { };
		virtual const char* what() const throw();
		virtual ErrCode which() const throw();
		virtual const std::string& msg() const throw();
	};

	class RuntimeError : public Exception
	{
	public:
		explicit RuntimeError(const char* _s)
			: Exception(_s) { };
		virtual ErrCode which() const throw();
	};

	class SyntaxError : public Exception
	{
	public:
		explicit SyntaxError(const char* _s)
			: Exception(_s) { };
		virtual ErrCode which() const throw();
	};

	class OutOfMemory : public Exception
	{
	public:
		explicit OutOfMemory(const char* _s)
			: Exception(_s) { };
		virtual ErrCode which() const throw();
	};

	class StackOverflow : public Exception
	{
	public:
		explicit StackOverflow()
			: Exception("stack overflow") { };
		virtual ErrCode which() const throw();
	};

	class FileError : public Exception
	{
	public:
		explicit FileError(const char* _s)
			: Exception(_s) { };
		virtual ErrCode which() const throw();
	};

	class InvalidArgument: public Exception
	{
	public:
		explicit InvalidArgument(const char* _s, const char* _t = NULL);
		virtual ErrCode which() const throw();
	};

	class Dump
	{
	public:
		virtual int write(State& S, const char* p, size_t sz) = 0;

		static int writer(Handle L, const char* p, size_t sz, Dump* _this);
	};

	class Undump
	{
	public:
		virtual const char* read(State& S, size_t *sz) = 0;

		static const char* reader(Handle L, Undump* _this, size_t *sz);
	};

	class Nil { };

	class Upvalue
	{
	private:
		int m_up;
	public:
		Upvalue(int _u) : m_up(_u) { };
		operator int (void)
			{ return lua_upvalueindex(m_up); };
	};

	template<typename T>
	class Userdata
	{
	private:
		T* m_ptr;
	public:
		Userdata() : m_ptr(NULL) { };
		explicit Userdata(T* _ptr)
			: m_ptr(_ptr) { };
		operator T* (void)
			{ return m_ptr; };
		operator const T* (void) const
			{ return m_ptr; };
		Userdata& operator== (T* _ptr) const
			{ m_ptr = _ptr; return *this; };
		Userdata& operator== (const Userdata& _ref) const
			{ m_ptr = _ref.m_ptr; return *this; };
	};

	class State : public Lua
	{
	protected:
		State() : Lua() { };

		static int panic(Handle L);
		ErrCode raise(ErrCode err);

		friend class Frame;

	public:
		~State() { };
		State(Handle _ptr) : Lua(_ptr) { };
		State(State const& _ref) : Lua(_ref) { };

		int getTop(void)
			{ return lua_gettop(m_L); };
		State& setTop(int index)
			{ lua_settop(m_L, index); return *this; };
		State& checkStack(int sz)
		{
			if (! lua_checkstack(m_L, sz)) throw StackOverflow();
			return *this;
		};

		State& insert(int index = -2)
			{ lua_insert(m_L, index); return *this; };
		State& pop(int num = 1)
			{ lua_pop(m_L, num); return *this; };
		State& remove(int index)
			{ lua_remove(m_L, index); return *this; };
		State& replace(int index)
			{ lua_replace(m_L, index); return *this; };
		State& xMove(State& t, int num = 1)
			{ lua_xmove(m_L, t.m_L, num); return *this; };
		State& copy(int index = -1)
			{ lua_pushvalue(m_L, index); return *this; };

		State& push(bool b)
			{ lua_pushboolean(m_L, b); return *this; };
		State& push(CFunction fn, int num) // Closure
			{ lua_pushcclosure(m_L, fn, num); return *this; };
		State& push(CFunction fn)
			{ lua_pushcfunction(m_L, fn); return *this; };
		State& push(const Nil&)
			{ lua_pushnil(m_L); return *this; };
		State& push(Integer n)
			{ lua_pushinteger(m_L, n); return *this; };
		State& push(int n)
			{ return push(Integer(n)); };
		State& push(Number n)
			{ lua_pushnumber(m_L, n); return *this; };
		State& push(float n)
			{ return push(Number(n)); };
		State& push(const char* s)
			{ lua_pushstring(m_L, s); return *this; };
		State& push(const char* s, size_t len)
			{ lua_pushlstring(m_L, s, len); return *this; };
		template <size_t N>
		State& push(const char (&s)[N])
			{ lua_pushlstring(m_L, s, N); return *this; };
		State& push(const std::string& s)
			{ lua_pushlstring(m_L, s.c_str(), s.size()); return *this; };
		State& push(void* p)
			{ lua_pushlightuserdata(m_L, p); return *this; };
		template<typename T>
		State& push(const Userdata<T>& p)
			{ lua_pushlightuserdata(m_L, const_cast<T*>(static_cast<const T*>(p))); return *this; };
		bool pushThread(void)
			{ return lua_pushthread(m_L); };

		bool toBoolean(int index = -1)
			{ return lua_toboolean(m_L, index); };
		CFunction toCFunction(int index = -1)
			{ return lua_tocfunction(m_L, index); };
		Integer toInteger(int index = -1)
			{ return lua_tointeger(m_L, index); };
		Number toNumber(int index = -1)
			{ return lua_tonumber(m_L, index); };
		const void* toPointer(int index = -1)
			{ return lua_topointer(m_L, index); };
		const char* asString(int index = -1)
			{ return lua_tostring(m_L, index); };
		const char* asString(int index = -1, size_t* l)
			{ return lua_tolstring(m_L, index, l); };
		std::string toString(int index = -1)
		{
			copy(index);
			size_t l;
			const char* p = lua_tolstring(m_L, -1, &l);
			std::string s(p, l);
			pop(1);
			return s;
		};
		void* toUserdata(int index = -1)
			{ return lua_touserdata(m_L, index); };
		template<typename T>
		T* toUserdata(const Userdata<T>&, int index = -1)
			{ return reinterpret_cast<T*>(toUserdata(index)); };

		Type getType(int index = -1)
			{ return Type(lua_type(m_L, index)); };
		const char* typeName(Type t)
			{ return lua_typename(m_L, t); };
		bool isBoolean(int index = -1)
			{ return lua_isboolean(m_L, index); };
		bool isCFunction(int index = -1)
			{ return lua_iscfunction(m_L, index); };
		bool isFunction(int index = -1)
			{ return lua_isfunction(m_L, index); };
		bool isLightUserdata(int index = -1)
			{ return lua_islightuserdata(m_L, index); };
		bool isInteger(int index = -1)
			{ return lua_isinteger(m_L, index); };
		bool isNil(int index = -1)
			{ return lua_isnil(m_L, index); };
		bool isNone(int index = -1)
			{ return lua_isnone(m_L, index); };
		bool isNoneOrNil(int index = -1)
			{ return lua_isnoneornil(m_L, index); };
		bool isNumber(int index = -1)
			{ return lua_isnumber(m_L, index); };
		bool isString(int index = -1)
			{ return lua_isstring(m_L, index); };
		bool isTable(int index = -1)
			{ return lua_istable(m_L, index); };
		bool isThread(int index = -1)
			{ return lua_isthread(m_L, index); };
		bool isUserdata(int index = -1)
			{ return lua_isuserdata(m_L, index); };

		bool isEqual(int ix2 = -2, int ix1 = -1)
			{ return lua_equal(m_L, ix1, ix2); };
		bool isLessThan(int ix2 = -2, int ix1 = -1)
			{ return lua_lessthan(m_L, ix1, ix2); };
		bool isRawEqual(int ix2 = -2, int ix1 = -1)
			{ return lua_rawequal(m_L, ix1, ix2); };

		const char* format(const char* fmt, ...)
		{
			const char* s;
			va_list va;
			va_start(va, fmt);
			s = format(fmt, va);
			va_end(va);
			return s;
		};
		State& pushFormat(const char* fmt, ...)
		{
			va_list va;
			va_start(va, fmt);
			format(fmt, va);
			va_end(va);
			return *this;
		};
		const char* format(const char* fmt, va_list va)
			{ return lua_pushvfstring(m_L, fmt, va); };
		State& pushFormat(const char* fmt, va_list va)
			{ lua_pushvfstring(m_L, fmt, va); return *this; };

		State& concat(int num = 2)
			{ lua_concat(m_L, num); return *this; };
		int concatI(int num = 2)
			{ lua_concat(m_L, num); return getTop(); };
		State& createTable(int narr = 0, int nrec = 0)
			{ lua_createtable(m_L, narr, nrec); return *this; };
		int createTableI(int narr = 0, int nrec = 0)
			{ lua_createtable(m_L, narr, nrec); return getTop(); };
		int error(void)
			{ return lua_error(m_L); };
		int error(const char* msg)
			{ push(msg); return error(); };
		int gcCollect(void)
			{ return lua_gc(m_L, LUA_GCCOLLECT, 0); };
		int gcCount(void)
			{ return lua_gc(m_L, LUA_GCCOUNT, 0); };
		int gcRestart(void)
			{ return lua_gc(m_L, LUA_GCRESTART, 0); };
		int gcStop(void)
			{ return lua_gc(m_L, LUA_GCSTOP, 0); };
		int gcSetPause(int wait)
			{ return lua_gc(m_L, LUA_GCSETPAUSE, wait); };
		int gcStep(int size)
			{ return lua_gc(m_L, LUA_GCSTEP, size); };
		int gcSetStepMul(int size)
			{ return lua_gc(m_L, LUA_GCSETSTEPMUL, size); };
		State& getFEnv(int index = -1)
			{ lua_getfenv(m_L, index); return *this; };
		int getFEnvI(int index = -1)
			{ lua_getfenv(m_L, index); return getTop(); };
		State& getField(const char* key, int index = -1)
			{ lua_getfield(m_L, index, key); return *this; };
		State& getField(const std::string& key, int index = -1)
			{ lua_getfield(m_L, index, key.c_str()); return *this; };
		int getFieldI(const char* key, int index = -1)
			{ lua_getfield(m_L, index, key); return getTop(); };
		int getFieldI(const std::string& key, int index = -1)
			{ lua_getfield(m_L, index, key.c_str()); return getTop(); };
		State& getGlobal(const char* name)
			{ lua_getglobal(m_L, name); return *this; };
		State& getGlobal(const std::string& name)
			{ lua_getglobal(m_L, name.c_str()); return *this; };
		int getGlobalI(const char* name)
			{ lua_getglobal(m_L, name); return getTop(); };
		int getGlobalI(const std::string& name)
			{ lua_getglobal(m_L, name.c_str()); return getTop(); };
		State& getMetatable(int index = -1)
		{
			if (!lua_getmetatable(m_L, index))
				push(Nil());
			return *this;
		};
		int getMetatableI(int index = -1)
		{
			if (!lua_getmetatable(m_L, index))
				push(Nil());
			return getTop();
		};
		State& getTable(int index = -2)
			{ lua_gettable(m_L, index); return *this; };
		int getTableI(int index = -2)
			{ lua_gettable(m_L, index); return getTop(); };
		State& getTable(int index, int key)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(key);
			lua_gettable(m_L, index);
			return *this;
		};
		int getTableI(int index, int key)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(key);
			lua_gettable(m_L, index);
			return getTop();
		};
		State& getTableN(Integer n, int index = -1)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			push(n);
			lua_gettable(m_L, index);
			return *this;
		};
		int getTableNI(Integer n, int index = -1)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			push(n);
			lua_gettable(m_L, index);
			return getTop();
		};
		State& newTable(void)
			{ lua_newtable(m_L); return *this; };
		int newTableI(void)
			{ lua_newtable(m_L); return getTop(); };

		void* newUserdata(size_t size)
			{ return lua_newuserdata(m_L, size); };
		template<typename T>
		State& newUserdata(const T& p)
		{
			T* ud = reinterpret_cast<T*>(
				lua_newuserdata(m_L, sizeof(T)));
			*ud = p;
			return *this;
		};

		bool next(int index = -2)
			{ return lua_next(m_L, index); };
		size_t objLen(int index = -1)
			{ return lua_objlen(m_L, index); };
		State& rawGet(int index = -2)
			{ lua_rawget(m_L, index); return *this; };
		int rawGetI(int index = -2)
			{ lua_rawget(m_L, index); return getTop(); };
		State& rawGet(int index, int key)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(key);
			lua_rawget(m_L, index);
			return *this;
		};
		int rawGetI(int index, int key)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(key);
			lua_rawget(m_L, index);
			return getTop();
		};
		State& rawGetN(Integer n, int index = -1)
			{ lua_rawgeti(m_L, index, n); return *this; };
		int rawGetNI(Integer n, int index = -1)
			{ lua_rawgeti(m_L, index, n); return getTop(); };
		State& rawSet(int index = -3)
			{ lua_rawset(m_L, index); return *this; };
		State& rawSet(int index, int key)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(key);
			insert(-2);
			lua_rawset(m_L, index);
			return *this;
		};
		State& rawSet(int index, int key, int val)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				index -= 2;
			if (val < 0 && val > LUA_REGISTRYINDEX)
				--val;
			copy(key);
			copy(val);
			lua_rawset(m_L, index);
			return *this;
		};
		State& rawSetN(Integer n, int index = -2)
			{ lua_rawseti(m_L, index, n); return *this; };
		State& rawSetN(Integer n, int index, int val)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(val);
			lua_rawseti(m_L, index, n);
			return *this;
		};
		State& setGlobal(const char* name, CFunction f)
			{ push(f); setGlobal(name); return *this; };
		State& setGlobal(const std::string& name, CFunction f)
			{ push(f); setGlobal(name); return *this; };
		State& setFEnv(int index = -2)
		{
			if (!lua_setfenv(m_L, index))
				throw InvalidArgument("expected function, thread, or userdata");
			return *this;
		};
		State& setField(const char* key, int index = -2)
			{ lua_setfield(m_L, index, key); return *this; };
		State& setField(const std::string& key, int index = -2)
			{ lua_setfield(m_L, index, key.c_str()); return *this; };
		State& setGlobal(const char* key)
			{ lua_setglobal(m_L, key); return *this; };
		State& setGlobal(const std::string& key)
			{ lua_setglobal(m_L, key.c_str()); return *this; };
		State& setMetatable(int index = -2)
		{
			if (!lua_setmetatable(m_L, index))
				throw InvalidArgument("expected function, thread, or userdata");
			return *this;
		};
		State& setTable(int index = -3)
			{ lua_settable(m_L, index); return *this; };
		State& setTable(int index, int key)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				--index;
			copy(key);
			insert(-2);
			lua_settable(m_L, index);
			return *this;
		};
		State& setTable(int index, int key, int val)
		{
			if (index < 0 && index > LUA_REGISTRYINDEX)
				index -= 2;
			if (val < 0 && val > LUA_REGISTRYINDEX)
				--val;
			copy(key);
			copy(val);
			lua_settable(m_L, index);
			return *this;
		};

		State& call(int nargs = 0, int nresults = 0)
		{
			lua_call(m_L, nargs, nresults);
			return *this;
		};
		State& pCall(CFunction func, void* ud = NULL)
		{
			raise(ErrCode(lua_cpcall(m_L, func, ud)));
			return *this;
		};
		State& pCall(int nargs = 0, int nresults = 0, int errfunc = 0)
		{
			raise(ErrCode(lua_pcall(m_L, nargs, nresults, errfunc)));
			return *this;
		};
		bool resume(int nargs = 0)
		{
			return Yielded == raise(ErrCode(lua_resume(m_L, nargs)));
		};
		ErrCode status(void)
			{ return ErrCode(lua_status(m_L)); };
		int yield(int nresults = 0)
			{ return lua_yield(m_L, nresults); };

		// lauxlib.h
		const char* getTypeName(int index = -1)
			{ return luaL_typename(m_L, index); };
		int typeError(int narg, const char* tname)
			{ return luaL_typerror(m_L, narg, tname); };
		int argError(int narg, const char* msg)
			{ return luaL_argerror(m_L, narg, msg); };
		State& checkAny(int narg)
			{ luaL_checkany(m_L, narg); return *this; };
		State& checkType(int narg, Type t)
			{ luaL_checktype(m_L, narg, t); return *this; };
		Integer checkInteger(int narg)
			{ return luaL_checkinteger(m_L, narg); };
		Number checkNumber(int narg)
			{ return luaL_checknumber(m_L, narg); };
		Integer checkIntOrNumber(int narg)
			{ return luaL_checkintnum(m_L, narg); };
		const char* checkString(int narg, size_t* l)
			{ return luaL_checklstring(m_L, narg, l); };
		std::string checkString(int narg)
		{
			size_t l;
			const char* s = luaL_checklstring(m_L, narg, &l);
			return std::string(s, l);
		};
		void* checkUserdata(int narg, const char* tname)
			{ return luaL_checkudata(m_L, narg, tname); };
		template<typename T>
		T* checkUserdata(int narg, const Userdata<T>&)
			{ return reinterpret_cast<T*>(luaL_checkudata(m_L, narg, T::s_ClassName)); };
		int checkOption(int narg, const char* const lst[], const char* def = NULL)
			{ return luaL_checkoption(m_L, narg, def, lst); };
		unsigned long checkFlags(int narg, const char* const lst[], const char* def = NULL)
			{ return luaL_checkflags(m_L, narg, def, lst); };
		State& checkStack(int sz, const char* msg)
			{ luaL_checkstack(m_L, sz, msg); return *this; };
		Integer optInteger(int narg, Integer d = 0)
			{ return luaL_optinteger(m_L, narg, d); };
		Number optNumber(int narg, Number d = 0)
			{ return luaL_optnumber(m_L, narg, d); };
		Integer optIntOrNumber(int narg, Integer d = 0)
			{ return luaL_optintnum(m_L, narg, d); };
		const char* optString(int narg, size_t* l, const char* d = NULL)
			{ return luaL_optlstring(m_L, narg, d, l); };
		std::string optString(int narg, const char* d = NULL)
		{
			size_t l;
			const char* s = luaL_optlstring(m_L, narg, d, &l);
			return std::string(s, l);
		};
		std::string optString(int narg, const std::string& d)
		{
			size_t l;
			const char* s = luaL_optlstring(m_L, narg, d.c_str(), &l);
			return std::string(s, l);
		};

		State& getMetatable(const char* tname)
			{ luaL_getmetatable(m_L, tname); return *this; };
		int getMetatableI(const char* tname)
			{ luaL_getmetatable(m_L, tname); return getTop(); };
		State& getMetafield(const char* e, int index = -1)
		{
			if (!luaL_getmetafield(m_L, index, e))
				push(Nil());
			return *this;
		};
		State& callMeta(const char* e, int index = -1)
		{
			if (!luaL_callmeta(m_L, index, e))
				throw InvalidArgument("method not found", e);
			return *this;
		};
		State& newMetatable(const char* tname)
		{
			luaL_newmetatable(m_L, tname);
			return *this;
		};

		const char* gSub(const char* s, const char* p, const char* r)
			{ return luaL_gsub(m_L, s, p, r); };
		int gSubI(const char* s, const char* p, const char* r)
			{ luaL_gsub(m_L, s, p, r); return getTop(); };
		int ref(int t)
			{ return luaL_ref(m_L, t); };
		int ref(void)
			{ return luaL_ref(m_L, LUA_REGISTRYINDEX); };
		void unref(int t, int ref)
			{ luaL_unref(m_L, t, ref); };
		void unref(int ref)
			{ luaL_unref(m_L, LUA_REGISTRYINDEX, ref); };
		State& getRef(int ref)
			{ lua_rawgeti(m_L, LUA_REGISTRYINDEX, ref); return *this; };
		State& where(int l = 1)
			{ luaL_where(m_L, l); return *this; };

		State& doFile(const char* filename)
		{
			loadFile(filename);
			pCall(0, MultiRet, 0);
			return *this;
		};
		State& doFile(const std::string& filename)
		{
			loadFile(filename);
			pCall(0, MultiRet, 0);
			return *this;
		};
		State& doString(const char* s)
		{
			loadString(s);
			pCall(0, MultiRet, 0);
			return *this;
		};
		State& doString(const std::string& s)
		{
			loadString(s);
			pCall(0, MultiRet, 0);
			return *this;
		};
		State& loadFile(const char* filename)
		{
			raise(ErrCode(luaL_loadfile(m_L, filename)));
			return *this;
		};
		State& loadFile(const std::string& filename)
		{
			raise(ErrCode(luaL_loadfile(m_L, filename.c_str())));
			return *this;
		};
		State& loadString(const char* s)
		{
			raise(ErrCode(luaL_loadstring(m_L, s)));
			return *this;
		};
		State& loadString(const char* s, size_t sz, const char* name = NULL)
		{
			raise(ErrCode(luaL_loadbuffer(m_L, s, sz, name)));
			return *this;
		};
		State& loadString(const std::string& s, const char* name = NULL)
		{
			raise(ErrCode(luaL_loadbuffer(m_L, s.c_str(), s.size(), name)));
			return *this;
		};

		State& dump(Dump* d)
		{
			raise(ErrCode(lua_dump(m_L, reinterpret_cast<Writer>(Dump::writer), d)));
			return *this;
		};
		State& load(Undump* d, const char* name = NULL)
		{
			raise(ErrCode(lua_load(m_L, reinterpret_cast<Reader>(Undump::reader), d, name)));
			return *this;
		};
		State& load(Undump* d, const std::string& name)
		{
			raise(ErrCode(lua_load(m_L, reinterpret_cast<Reader>(Undump::reader), d, name.c_str())));
			return *this;
		};

		State newThread(void)
			{ return State(lua_newthread(m_L)); };
		State toThread(int index = -1)
		{
			Handle _ptr = lua_tothread(m_L, index);
			if (_ptr == NULL)
				throw Exception("not a thread");
			return State(_ptr);
		};
		State& openLibs(void)
			{ luaL_openlibs(m_L); return *this; };
		State& registerLib(const Registry* table, const char* name = NULL)
			{ luaL_register(m_L, name, table); return *this; };

		enum FinalType
		{
			Finally = LUA_FINALWAYS,
			FinalReturn = LUA_FINSUCCESS,
			FinalError = LUA_FINERROR
		};

		int getFinalFrame(void)
			{ return lua_getfinalframe(m_L); };
		int finally(int nargs = 0, FinalType when = Finally, int scope = -1)
			{ return lua_finally(m_L, nargs, scope, when); };
		State& finalize(bool failed = false, int base = 0)
			{ lua_finalize(m_L, base, int(failed)); return *this; }
	};

	class MainState : public State
	{
	private:
		MainState(State const&) { };
	public:
		~MainState()
		{
			if (m_L) lua_close(m_L);
		};
		MainState()
		{
			m_L = luaL_newstate();
			lua_atpanic(m_L, panic);
		};
	};

	class Frame
	{
	private:
		State m_S;
		int m_top;
		Frame() { };
		Frame(Frame const&) { };
	public:
		Frame(Handle S) : m_S(S)
			{ m_top = m_S.getTop(); };
		Frame(State& S)	: m_S(S)
			{ m_top = m_S.getTop(); };
		~Frame()
			{ m_S.setTop(m_top); };
		operator State& ()
			{ return m_S; };
		State* operator -> ()
			{ return &m_S; };
	};

	class Buffer
	{
	private:
		luaL_Buffer m_B;
	public:
		Buffer(State& S)
			{ luaL_buffinit(S.L(), &m_B); };
		Buffer& push(void)
			{ luaL_pushresult(&m_B); return *this; };
		Buffer& add(void)
			{ luaL_addvalue(&m_B); return *this; };
		Buffer& add(char c)
			{ luaL_addchar(&m_B, c); return *this; };
		Buffer& add(const char* s)
			{ luaL_addstring(&m_B, s); return *this; };
		Buffer& add(const std::string& s)
			{ luaL_addlstring(&m_B, s.c_str(), s.size()); return *this; };
		char* prep(void)
			{ return luaL_prepbuffer(&m_B); };
		Buffer& addSize(size_t n)
			{ luaL_addsize(&m_B, n); return *this; };
	};

	class StringDump : public Dump, public Undump
	{
	private:
		std::string m_S;
		bool m_used;
	public:
		virtual ~StringDump();
		StringDump() : m_used(false) { };
		StringDump(const std::string& _s) : m_S(_s), m_used(false) { };
		operator const std::string& (void) const { return m_S; };

		virtual int write(State&, const char* p, size_t sz);
		virtual const char* read(State&, size_t *sz);
		void reset(void) { m_used = false; };
	};

}

#endif
