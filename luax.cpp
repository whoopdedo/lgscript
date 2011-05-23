/******************************************************************************
 *    luax.cpp
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
#define LUAX_INLINE
#include "luax.h"
#include <new>
#include <cstring>

namespace luax
{

const int MultiRet = LUA_MULTRET;

Exception::~Exception() throw()
	{ }
const char* Exception::what() const throw()
	{ return err.c_str(); }
ErrCode Exception::which() const throw()
	{ return ErrErr; }
const std::string& Exception::msg() const throw()
	{ return err; }

ErrCode RuntimeError::which() const throw()
	{ return ErrRun; }
ErrCode SyntaxError::which() const throw()
	{ return ErrSyntax; }
ErrCode OutOfMemory::which() const throw()
	{ return ErrMem; }
ErrCode StackOverflow::which() const throw()
	{ return ErrStack; }
ErrCode FileError::which() const throw()
	{ return ErrFile; }
ErrCode InvalidArgument::which() const throw()
	{ return ErrType; }
InvalidArgument::InvalidArgument(const char* _s, const char* _t)
	: Exception(_s)
{
	if (_t) {
		err += '('; err += _t; err += ')';
	}
}

int Dump::writer(Handle L, const char* p, size_t sz, Dump* _this)
{
	State S(L);
	return _this->write(S, p, sz);
}
const char* Undump::reader(Handle L, Undump* _this, size_t *sz)
{
	State S(L);
	return _this->read(S, sz);
}
StringDump::~StringDump()
	{ }
int StringDump::write(State&, const char* p, size_t sz)
{
	m_S.append(p, sz);
	return 0;
};
const char* StringDump::read(State&, size_t *sz)
{
	if (m_used) return NULL;
	*sz = m_S.size();
	return m_S.c_str();
};


void* State::realloc(void*, void* ptr, size_t osize, size_t nsize)
{
	if (!nsize)
	{
		::operator delete(ptr, std::nothrow);
		return NULL;
	}
	if (!osize)
	{
		void* nptr = ::operator new(nsize, std::nothrow);
		return nptr;
	}
	if (size_t(osize - nsize) <= sizeof(int)*4)
		return ptr;  // nsize < osize and difference is small
	void* nptr = ::operator new(nsize, std::nothrow);
	if (!nptr)
		return NULL;
	memcpy(nptr, ptr, std::min(osize,nsize));
	::operator delete(ptr, std::nothrow);
	return nptr;
}

int State::panic(Handle L)
{
	State S(L);
	throw Exception(S.errMsg());
	return 0;
}

ErrCode State::raise(ErrCode err)
{
	switch (err)
	{
	case NoErr:
	case Yielded:
		break;
	case ErrRun: throw RuntimeError(errMsg());
	case ErrSyntax: throw SyntaxError(errMsg());
	case ErrMem: throw OutOfMemory(errMsg());
	case ErrFile: throw FileError(errMsg());
	default: throw Exception(errMsg());
	};
	return err;
}

}
