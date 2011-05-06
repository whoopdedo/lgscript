/******************************************************************************
 *    LgMultiParm.cpp
 *
 *    This file is part of LgScript
 *    Copyright (C) 2009 Tom N Harris <telliamed@whoopdedo.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *****************************************************************************/
#define LUAX_INLINE
#include "luax.h"
#include "LgMultiParm.h"

#include <lua/vec.h>

using namespace Lgs;
using namespace luax;

void ScriptMultiParm::push(const cMultiParm& mp)
{
	switch (mp.type)
	{
	case kMT_Undef:
		m_lua.push(Nil()); break;
	case kMT_Int:
		m_lua.push(mp.i); break;
	case kMT_Float:
		m_lua.push(mp.f); break;
	case kMT_String:
		m_lua.push(mp.psz); break;
	case kMT_Vector:
		m_lua.getGlobal("vector");
		m_lua.push(mp.pVector->x)
		     .push(mp.pVector->y)
		     .push(mp.pVector->z);
		m_lua.call(3, 1);
		break;
	}
}

const cMultiParm& ScriptMultiParm::pop(int ix)
{
	switch (m_lua.getType(ix))
	{
	default:
		m_data.Unset();
		break;
	case TInteger:
		m_data = m_lua.toInteger(ix);
		break;
	case TNumber:
		m_data = m_lua.toNumber(ix);
		break;
	case TBoolean:
		m_data = m_lua.toBoolean(ix);
		break;
	case TString:
		m_data = m_lua.asString(ix);
		break;
	case TUserdata:
	{
		bool bIsVector = false;
		if (!m_lua.getMetatable(ix).isNil())
		{
			m_lua.getField(LVECTOR_NAME, LUA_REGISTRYINDEX);
			bIsVector = m_lua.isRawEqual();
			m_lua.pop();
		}
		m_lua.pop();
		if (bIsVector)
		{
			const lvector* v = m_lua.toUserdata(Userdata<lvector>(),ix);
			cScrVec sv(v->v.x, v->v.y, v->v.z);
			m_data = sv;
		}
		else
			m_data.Unset();
		break;
	}
	}
	return m_data;
}
