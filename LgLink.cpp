/******************************************************************************
 *  LgLink.cpp
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
#include "LgLink.h"
#include "LgStructData.h"
#include "ScriptModule.h"

using namespace Lgs;
using namespace luax;

void* LgLink::operator new(size_t s, State& L)
{
	void* p = L.newUserdata(s);
	L.getMetatable(s_ClassName).setMetatable();
	//L.copy().finally();
	return p;
}

void LgLink::operator delete(void*, State&)
{
	// allow GC
}

void LgLink::operator delete(void*)
{
	// allow GC
}

const char LgLink::s_ClassName[] = "LgLink";

const Registry LgLink::Methods[] = {
	{"Get", LinkGet},
	{"GetData", GetData},
	{"SetData", SetData},
	{NULL, NULL}
};

const Registry LgLink::Properties[] = {
	{"dest", DestProperty},
	{"flavor", FlavorProperty},
	{"id", IdProperty},
	{"source", SourceProperty},
	{NULL, NULL}
};

void LgLink::Init(State& S)
{
	S.newMetatable(s_ClassName).registerLib(Methods).copy();
	S.createTable().registerLib(Properties);
	S.push(Index,2).setField("__index");
}

inline LgLink* LgLink::Check(State& S, int arg)
{
	LgLink* self = S.checkUserdata(arg,Userdata<LgLink>());
	return self;
}

long LgLink::pop(State& S, int arg)
{
	if (S.isUserdata(arg))
	{
		LgLink* self = S.checkUserdata(arg,Userdata<LgLink>());
		return self ? self->link : 0;
	}
	return S.optInteger(arg);
}

SInterface<ILinkManager> LgLink::LinkMan;

void LgLink::Refresh(void)
{
	if (link && !rel)
	{
		LinkMan.set(g_pScriptManager);
		rel.reset(LinkMan->GetRelation(LINKID_TO_LINKKIND(link)));
	}
}

bool LgLink::Refresh(sLink& sl)
{
	Refresh();
	if (!rel)
		return false;
	return rel->Get(link, &sl);
}

int LgLink::Index(Handle L)
{
	State S(L);
	S.setTop(2);
	if (!S.copy(2).rawGet(Upvalue(2)).isNil())
	{
		S.insert(1).setTop(2).call(1,1);
		return 1;
	}
	S.pop().copy(2).rawGet(Upvalue(1));
	return 1;
}

int LgLink::IdProperty(Handle L)
{
	State S(L);
	LgLink* self = Check(S,1);
	S.push(self->link);
	return 1;
}

int LgLink::FlavorProperty(Handle L)
{
	State S(L);
	LgLink* self = Check(S,1);
	S.push(LINKID_TO_LINKKIND(self->link));
	return 1;
}

int LgLink::SourceProperty(Handle L)
{
	State S(L);
	LgLink* self = Check(S,1);
	sLink ret;
	if (self->Refresh(ret))
		S.push(Integer(ret.source));
	else
		S.push(Nil());
	return 1;
}

int LgLink::DestProperty(Handle L)
{
	State S(L);
	LgLink* self = Check(S,1);
	sLink ret;
	if (self->Refresh(ret))
		S.push(Integer(ret.dest));
	else
		S.push(Nil());
	return 1;
}

int LgLink::LinkGet(Handle L)
{
	State S(L);
	LgLink* self = Check(S,1);
	sLink ret;
	if (self->Refresh(ret))
	{
		S.push(Integer(ret.dest))
		 .push(Integer(ret.source))
		 .push(Integer(ret.flavor));
		return 3;
	}
	S.push(Nil());
	return 1;
}

int LgLink::GetData(Handle L)
{
	State S(L);
	LgLink* self = Check(S,1);
	self->Refresh();
	if (!self->rel)
	{
		S.push(Nil());
		return 1;
	}
	const sRelationDataDesc* pDataDesc = self->rel->DescribeData();
	const char* pData = static_cast<const char*>(self->rel->GetData(self->link));
	if (!pDataDesc || pDataDesc->uiTypeSize == 0 || !pData)
	{
		// No data.
		S.push(Nil());
		return 1;
	}
	const char* field = S.optString(2, NULL, NULL);
	if (field != NULL)
		return StructData(S, pDataDesc->szTypeName).push(pData, field);
	return StructData(S, pDataDesc->szTypeName).push(pData);
}

int LgLink::SetData(Handle L)
{
	State S(L);
	S.setTop(3);
	LgLink* self = Check(S,1);
	self->Refresh();
	if (!self->rel)
	{
		return S.argError(1, "invalid link");
	}
	const sRelationDataDesc* pDataDesc = self->rel->DescribeData();
	char* pData = static_cast<char*>(self->rel->GetData(self->link));
	if (!pDataDesc || pDataDesc->uiTypeSize == 0)
	{
		// No data.
		return 0;
	}
	if (!pData)
	{
		pData = static_cast<char*>(S.newUserdata(pDataDesc->uiTypeSize));
	}
	const char* field = S.optString(2, NULL, NULL);
	//if (field != NULL)
	//	StructData(S, pDataDesc->szTypeName).pop(3, pData, field);
	//StructData(S, pDataDesc->szTypeName).pop(3, pData);
	return 0;
}
