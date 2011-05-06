/******************************************************************************
 *    LgStructData.cpp
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
#include "LgStructData.h"
#include "ScriptModule.h"

using namespace Lgs;
using namespace luax;

StructData::StructData(luax::State& L, const char* D)
 : m_lua(L), m_sd(g_pScriptManager)
{
	m_desc = m_sd->Lookup(D);
}

int StructData::push(const char* data)
{
	if (!m_desc)
		return m_lua.error("invalid structure name");
	if (!data)
	{
		m_lua.push(Nil());
		return 1;
	}
	if (m_sd->IsSimple(m_desc))
	{
		pushField(&m_desc->fields[0], data);
		return 1;
	}
	m_lua.createTable(0, m_desc->num_fields);
	for (int n = 0; n < m_desc->num_fields; n++)
	{
		m_lua.push(m_desc->fields[n].name);
		pushField(&m_desc->fields[n], data);
		m_lua.rawSet();
	}
	return 1;
}

int StructData::push(const char* data, const char* name)
{
	if (!m_desc)
		return m_lua.error("invalid structure name");
	if (!data)
	{
		m_lua.push(Nil());
		return 1;
	}
	if (name == NULL && m_sd->IsSimple(m_desc))
	{
		pushField(&m_desc->fields[0], data);
		return 1;
	}
	const sFieldDesc* field = m_sd->GetFieldNamed(m_desc, name);
	if (!field)
		return m_lua.error("invalid field name");
	pushField(field, data);
	return 1;
}

void StructData::pushField(const sFieldDesc* field, const char* data)
{
	const char* elem = data + field->offset;
	switch (field->type)
	{
	case kFieldTypeInt:
	{
		int i = 0;
		switch (field->size)
		{
		case 1: i = *elem; break;
		case 2: i = *reinterpret_cast<const short*>(elem); break;
		default: i = *reinterpret_cast<const long*>(elem); break;
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
	case kFieldTypeShortInt:
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
		if (l > INT_MAX)
			m_lua.push(double(l));
		else
			m_lua.push(int(l));
		break;
	}
	case kFieldTypeString:
	{
		// I don't want to copy NULLs
		const char* end = reinterpret_cast<const char*>(memchr(elem, '\0', field->size));
		m_lua.push(elem, end-elem);
		break;
	}
	/*
	case kFieldTypePtr:
		m_lua.push(elem, field->size);
		break;
	*/
	case kFieldTypeVector:
	{
		const mxs_vector* v = reinterpret_cast<const mxs_vector*>(elem);
		m_lua.getGlobal("vector")
			.push(v->x).push(v->y).push(v->z)
			.call(3, 1);
		break;
	}
	case kFieldTypeFloat:
	case kFieldTypeDouble:
		if (field->size > sizeof(float))
			m_lua.push(*reinterpret_cast<const double*>(elem));
		else
			m_lua.push(*reinterpret_cast<const float*>(elem));
		break;
	default:
		m_lua.push(Nil());
		break;
	}
}
