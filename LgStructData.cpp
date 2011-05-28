/******************************************************************************
 *  LgStructData.cpp
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
#include "LgStructData.h"
#include "ScriptModule.h"
#include "mod/modlib.h"
#include "utils.h"

#include <cmath>
#include <cstring>

using namespace Lgs;
using namespace luax;

inline size_t strnlen(const char* str, size_t maxn)
{
	return std::min(::strlen(str), maxn);
}

SInterface<IStructDescTools> StructData::SD;

StructData::StructData(State& L, const char* D)
 : m_lua(L)
{
	SD.set(g_pScriptManager);
	m_desc = SD->Lookup(D);
}

int StructData::push(const void* data)
{
	if (!m_desc)
		return m_lua.error("invalid structure name");
	if (!data || m_desc->num_fields == 0)
	{
		m_lua.push(Nil());
		return 1;
	}
	if (m_desc->num_fields == 1)
	{
		pushField(&m_desc->fields[0], data);
		return 1;
	}
	int table = m_lua.createTableI(m_desc->num_fields, 1);
	for (int n = m_desc->num_fields; n > 0; n--)
	{
		m_lua.push(m_desc->fields[n-1].name, strnlen(m_desc->fields[n-1].name, sizeof(m_desc->fields->name)));
		pushField(&m_desc->fields[n-1], data);
		m_lua.copy().rawSetN(n, table).rawSet(table);
	}
	return 1;
}

int StructData::push(const void* data, const char* name)
{
	if (!m_desc)
		return m_lua.error("invalid structure name");
	if (!data || m_desc->num_fields == 0)
	{
		m_lua.push(Nil());
		return 1;
	}
	if (name == NULL)
	{
		pushField(&m_desc->fields[0], data);
		return 1;
	}
	const sFieldDesc* field = SD->GetFieldNamed(m_desc, name);
	if (field)
		pushField(field, data);
	else
		m_lua.push(Nil());
	return 1;
}

void StructData::pushField(const sFieldDesc* field, const void* data)
{
	const char* elem = static_cast<const char*>(data) + field->offset;
	switch (field->type)
	{
	case kFieldTypeInt:
	case kFieldTypeShortInt:
	{
		int i = 0;
		switch (field->size)
		{
		case 1: i = (field->flags & kFieldFlagUnsigned) ?
				*reinterpret_cast<unsigned const char*>(elem) : *elem;
			break;
		case 2: i = (field->flags & kFieldFlagUnsigned) ?
				*reinterpret_cast<const unsigned short*>(elem) :
				*reinterpret_cast<const short*>(elem);
			break;
		default:
			if (field->flags & kFieldFlagUnsigned)
			{
				unsigned long l = *reinterpret_cast<const unsigned long*>(elem);
				m_lua.push(l);
				return;
			}
			i = *reinterpret_cast<const long*>(elem);
			break;
		}
		m_lua.push(i);
		break;
	}
	case kFieldTypeBoolean:
	{
		bool b = false;
		switch (field->size)
		{
		case 1: b = *elem; break;
		case 2: b = *reinterpret_cast<const short*>(elem); break;
		default: b = *reinterpret_cast<const long*>(elem); break;
		}
		m_lua.push(b);
		break;
	}
	case kFieldTypeBitVector:
	case kFieldTypeEnum:
	{
		unsigned long l = 0;
		switch (field->size)
		{
		case 1: l = *reinterpret_cast<const unsigned char*>(elem); break;
		case 2: l = *reinterpret_cast<const unsigned short*>(elem); break;
		default: l = *reinterpret_cast<const unsigned long*>(elem); break;
		}
		m_lua.push(l);
		break;
	}
	//case kFieldTypeStringPtr:
	//	elem = *reinterpret_cast<const char* const*>(elem);
	case kFieldTypeString:
	{
		// I don't want to copy NULLs
		const char* end = reinterpret_cast<const char*>(memchr(elem, '\0', field->size));
		m_lua.push(elem, end-elem);
		break;
	}
	case kFieldTypePoint:
	{
		const float* v = reinterpret_cast<const float*>(elem);
		lmod_newvector(m_lua.L(), v[0], v[1], 0);
		break;
	}
	case kFieldTypeVector:
	{
		const mxs_vector* v = reinterpret_cast<const mxs_vector*>(elem);
		lmod_newvector(m_lua.L(), v->x, v->y, v->z);
		break;
	}
	case kFieldTypeDoubleVec:
	{
		const double* v = reinterpret_cast<const double*>(elem);
		lmod_newvector(m_lua.L(), v[0], v[1], v[2]);
		break;
	}
	case kFieldTypeFloat:
	case kFieldTypeDouble:
		if (field->size > sizeof(float))
			m_lua.push(*reinterpret_cast<const double*>(elem));
		else
			m_lua.push(*reinterpret_cast<const float*>(elem));
		break;
	case kFieldTypeFixed:
	{
		long l = *reinterpret_cast<const long*>(elem);
		m_lua.push(float(l) / 65536.0f);
		break;
	}
	case kFieldTypeFixVec:
	{
		const long *v = reinterpret_cast<const long*>(elem);
		lmod_newvector(m_lua.L(), (float(v[0])/65536.0f), (float(v[1])/65536.0f), (float(v[2])/65536.0f));
		break;
	}
	case kFieldTypeAng:
	{
		unsigned short a = *reinterpret_cast<const unsigned short*>(elem);
		m_lua.push(MXS_ANG2RAD(a));
		break;
	}
	case kFieldTypeAngVec:
	{
		const mxs_angvec* v = reinterpret_cast<const mxs_angvec*>(elem);
		lmod_newvector(m_lua.L(), MXS_ANG2RAD(v->x), MXS_ANG2RAD(v->y), MXS_ANG2RAD(v->z));
		break;
	}
	case kFieldTypeRGBA:
	{
		m_lua.push(Integer(*reinterpret_cast<const ulong*>(elem)));
		break;
	}
	default:
		m_lua.push(Nil());
		break;
	}
}

int StructData::pop(int arg, void* data)
{
	if (!m_desc)
		return m_lua.error("invalid structure name");
	if (!data)
		return m_lua.error("invalid structure data");
	if (m_desc->num_fields == 0)
		return 0;
	if (m_desc->num_fields == 1)
	{
		popField(&m_desc->fields[0], data, arg);
		return 0;
	}

	m_lua.checkType(arg, TTable);
	m_lua.push(Nil());
	while (m_lua.next(arg))
	{
		if (m_lua.getType(-2) == TNumber)
		{
			int n = m_lua.toInteger(-2);
			if (n > 0 && n <= m_desc->num_fields)
				popField(&m_desc->fields[n-1], data, -1);
		}
		m_lua.pop(1);
	}
	m_lua.push(Nil());
	while (m_lua.next(arg))
	{
		if (m_lua.getType(-2) == TString)
		{
			const sFieldDesc* field = SD->GetFieldNamed(m_desc, m_lua.asString(-2));
			if (field)
				popField(field, data, -1);
		}
		m_lua.pop(1);
	}
	return 0;
}

int StructData::pop(int arg, void* data, const char* name)
{
	if (!m_desc)
		return m_lua.error("invalid structure name");
	if (!data)
		return m_lua.error("invalid structure data");
	if (m_desc->num_fields == 0)
		return 0;
	if (name == NULL)
	{
		popField(&m_desc->fields[0], data, arg);
		return 0;
	}
	const sFieldDesc* field = SD->GetFieldNamed(m_desc, name);
	if (field)
		popField(field, data, arg);
	return 0;
}

void StructData::popField(const sFieldDesc* field, void* data, int arg)
{
	char* elem = static_cast<char*>(data) + field->offset;
	switch (field->type)
	{
	case kFieldTypeInt:
	case kFieldTypeShortInt:
	{
		if (field->flags & kFieldFlagUnsigned)
		{
			long l = m_lua.toNumber(arg);
			switch (field->size)
			{
			case 1: *reinterpret_cast<unsigned char*>(elem) = l; break;
			case 2: *reinterpret_cast<unsigned short*>(elem) = l; break;
			default: *reinterpret_cast<unsigned long*>(elem) = l; break;
			}
		}
		else
		{
			int i = m_lua.toInteger(arg);
			switch (field->size)
			{
			case 1: *elem = i; break;
			case 2: *reinterpret_cast<short*>(elem) = i; break;
			default: *reinterpret_cast<long*>(elem) = i; break;
			}
		}
		break;
	}
	case kFieldTypeBoolean:
	{
		bool b = m_lua.toBoolean(arg);
		switch (field->size)
		{
		case 1: *elem = b; break;
		case 2: *reinterpret_cast<short*>(elem) = b; break;
		default: *reinterpret_cast<long*>(elem) = b; break;
		}
		break;
	}
	case kFieldTypeBitVector:
	case kFieldTypeEnum:
	{
		unsigned long l = m_lua.toNumber(arg);
		switch (field->size)
		{
		case 1: *reinterpret_cast<unsigned char*>(elem) = l; break;
		case 2: *reinterpret_cast<unsigned short*>(elem) = l; break;
		default: *reinterpret_cast<unsigned long*>(elem) = l; break;
		}
		break;
	}
	//case kFieldTypeStringPtr:
	//	elem = *reinterpret_cast<char**>(elem);
	case kFieldTypeString:
	{
		const char* s = m_lua.asString(arg);
		if (!s)
			s = "";
		strncpy(elem, s, field->size);
		break;
	}
	case kFieldTypePoint:
	{
		float* v = reinterpret_cast<float*>(elem);
		const lvector* varg = lmod_tovector(m_lua.L(), arg);
		if (varg)
		{
			v[0] = varg->v.x;
			v[1] = varg->v.y;
		}
		else
		{
			v[0] = 0;
			v[1] = 0;
		}
		break;
	}
	case kFieldTypeVector:
	{
		mxs_vector* v = reinterpret_cast<mxs_vector*>(elem);
		const lvector* varg = lmod_tovector(m_lua.L(), arg);
		if (varg)
		{
			v->x = varg->v.x;
			v->y = varg->v.y;
			v->z = varg->v.z;
		}
		else
		{
			v->x = 0;
			v->y = 0;
			v->z = 0;
		}
		break;
	}
	case kFieldTypeDoubleVec:
	{
		double* v = reinterpret_cast<double*>(elem);
		const lvector* varg = lmod_tovector(m_lua.L(), arg);
		if (varg)
		{
			v[0] = varg->v.x;
			v[1] = varg->v.y;
			v[2] = varg->v.z;
		}
		else
		{
			v[0] = 0;
			v[1] = 0;
			v[2] = 0;
		}
		break;
	}
	case kFieldTypeFloat:
	case kFieldTypeDouble:
	{
		Number n = m_lua.toNumber(arg);
		if (field->size > sizeof(float))
			*reinterpret_cast<double*>(elem) = n;
		else
			*reinterpret_cast<float*>(elem) = n;
		break;
	}
	case kFieldTypeFixed:
	{
		*reinterpret_cast<long*>(elem) = m_lua.toNumber(arg) * 65536.5f;
		break;
	}
	case kFieldTypeFixVec:
	{
		long *v = reinterpret_cast<long*>(elem);
		const lvector* varg = lmod_tovector(m_lua.L(), arg);
		if (varg)
		{
			v[0] = varg->v.x * 65536.5f;
			v[1] = varg->v.y * 65536.5f;
			v[2] = varg->v.z * 65536.5f;
		}
		else
		{
			v[0] = 0;
			v[1] = 0;
			v[2] = 0;
		}
		break;
	}
	case kFieldTypeAng:
	{
		*reinterpret_cast<unsigned short*>(elem) = MXS_RAD2ANG(m_lua.toNumber(arg));
		break;
	}
	case kFieldTypeAngVec:
	{
		mxs_angvec* v = reinterpret_cast<mxs_angvec*>(elem);
		const lvector* varg = lmod_tovector(m_lua.L(), arg);
		if (varg)
		{
			v->x = MXS_RAD2ANG(varg->v.x);
			v->y = MXS_RAD2ANG(varg->v.y);
			v->z = MXS_RAD2ANG(varg->v.z);
		}
		else
		{
			v[0] = 0;
			v[1] = 0;
			v[2] = 0;
		}
		break;
	}
	case kFieldTypeRGBA:
	{
		*reinterpret_cast<ulong*>(elem) = m_lua.toInteger(arg);
		break;
	}
	default:
		break;
	}
}
