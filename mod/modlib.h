/******************************************************************************
 *    modlib.h
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

#define LVECTOR_NAME "vector"
#define LVECTOR_EPSILON 1e-10

#define LLIST_NAME "list"

LUALIB_API int luaopen_vector (lua_State* L);
LUALIB_API int luaopen_list (lua_State* L);
LUALIB_API int luaopen_ext (lua_State* L);

#endif
