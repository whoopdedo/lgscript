/******************************************************************************
 *  LgMultiParm.cpp
 *
 *  This file is part of LgScript
 *  Copyright (C) 2011 Tom N Harris <telliamed@whoopdedo.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************/
#define LUAX_INLINE
#include "luax.h"
#include "LgMultiParm.h"

#include "mod/modlib.h"

using namespace Lgs;
using namespace luax;

void ScriptMultiParm::push(const cMultiParm& mp)
{
	switch (mp.type)
	{
	default:
	case kMT_Undef:
		m_lua.push(Nil()); break;
	case kMT_Int:
		m_lua.push(mp.i); break;
	case kMT_Float:
		m_lua.push(mp.f); break;
	case kMT_String:
		m_lua.push(mp.psz); break;
	case kMT_Vector: {
			lmod_newvector(m_lua.L(), mp.pVector->x, mp.pVector->y, mp.pVector->z);
			break;
		}
	}
}

const cMultiParm& ScriptMultiParm::pop(int ix)
{
	switch (m_lua.getType(ix))
	{
	default:
		m_data.Unset();
		break;
	case TNumber: {
			Integer inum;
			Number num = m_lua.toNumber(ix);
			lua_number2integer(inum, num);
			if (static_cast<Number>(inum) == num)
				m_data = inum;
			else
				m_data = num;
			break;
		}
	case TBoolean:
		m_data = m_lua.toBoolean(ix);
		break;
	case TString:
		m_data = m_lua.asString(ix, NULL);
		break;
	case TUserdata:
	{
		const lvector* v = lmod_tovector(m_lua.L(), ix);
		if (v)
		{
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
