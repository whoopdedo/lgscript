/******************************************************************************
 *    luax.h
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
#ifndef LUAX_H
#define LUAX_H

#include <string>
#include <exception>

struct lua_State;

namespace luax
{

	typedef lua_State* Handle;

	class Lua
	{
	protected:
		Handle m_L;
		Lua() : m_L(NULL) { };
		Lua(Lua const& _ref) : m_L(_ref.m_L) { };
		Lua(Handle _ptr) : m_L(_ptr) { };
	public:
		~Lua() { };
		Handle L(void) const { return m_L; };
	};

	class State;
	class MainState;
}

#endif

#ifdef LUAX_INLINE
#include "luax.hpp"
#endif
