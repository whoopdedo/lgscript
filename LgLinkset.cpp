/******************************************************************************
 *  LgLinkset.cpp
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
#include "LgLinkset.h"
#include "LgStructData.h"
#include "ScriptModule.h"

#include <lg/interface.h>

using namespace Lgs;
using namespace luax;

void* LinkSet::operator new(size_t s, State& L)
{
	void* p = L.newUserdata(s);
	L.getMetatable(s_ClassName).setMetatable();
	L.copy().finally();
	return p;
}

void LinkSet::operator delete(void*, State&)
{
	// allow GC
}

void LinkSet::operator delete(void*)
{
	// allow GC
}

const char LinkSet::s_ClassName[] = "LgLinkset";

const Registry LinkSet::Methods[] = {
	{"__gc", Release},
	{"__pairs", Pairs},
	{"__call", Iter},
	{"Release", Release},
	{"Done", Done},
	{"Next", Next},
	{"Id", Id},
	{"Link", Link},
	{"Data", Data},
	{NULL, NULL}
};

void LinkSet::Init(State& S)
{
	S.newMetatable(s_ClassName).registerLib(Methods);
	S.copy().setField("__index");
}

inline LinkSet* LinkSet::Check(State& S, int arg)
{
	LinkSet* ls = S.checkUserdata(arg,Userdata<LinkSet>());
	//if (ls->query == NULL) S.error("linkset used after Release");
	return ls;
}

void LinkSet::Refresh(void)
{
	link = query->ID();
	sLink sl;
	if (0 == query->Link(&sl))
	{
		source = sl.source;
		dest = sl.dest;
		flavor = sl.flavor;
	}
	else
	{
		dest = source = 0;
		flavor = 0;
	}
}

int LinkSet::Pairs(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	S.push(Iter).copy(1);
	if (self->query == NULL || self->link == 0)
		S.push(Nil());
	else
		S.push(self->link);
	return 3;
}

int LinkSet::Iter(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	if (!self->query)
	{
		S.push(Nil());
		return 1;
	}
	long link = self->link;
	// Iter via __pairs will get 2 args, via __call has 3 args.
	if (S.getTop() > 1 && !S.isNoneOrNil(-1))
	{
		link = S.toInteger(-1);
		if (link != self->link)
			return S.error("iteration out-of-order");
		self->query->Next();
	}
	if (self->query->Done())
	{
		self->query->Release();
		self->query = NULL;
		S.push(Nil());
		return 1;
	}
	self->Refresh();
	S.push(self->link).push(self->dest).push(self->source).push(Integer(self->flavor));
	return 4;
}

int LinkSet::Release(Handle L)
{
	State S(L);
	LinkSet* self = S.checkUserdata(1,Userdata<LinkSet>());
	if (self->query)
	{
		self->query->Release();
		self->query = NULL;
	}
	return 0;
}

int LinkSet::Done(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	if (!self->query)
	{
		S.push(true);
		return 1;
	}
	bool done = self->query->Done();
	S.push(done);
	return 1;
}

int LinkSet::Next(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	if (self->query)
	{
		self->query->Next();
	}
	return 0;
}

int LinkSet::Id(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	if (!self->query)
	{
		S.push(Nil());
		return 1;
	}
	if (self->link == 0)
		self->Refresh();
	S.push(self->link);
	return 1;
}

int LinkSet::Link(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	if (!self->query)
	{
		S.push(Nil());
		return 1;
	}
	if (self->link == 0)
		self->Refresh();
	S.push(self->dest).push(self->source).push(Integer(self->flavor));
	return 3;
}

int LinkSet::Data(Handle L)
{
	State S(L);
	LinkSet* self = Check(S,1);
	if (!self->query)
	{
		S.push(Nil());
		return 1;
	}
	if (self->link == 0)
		self->Refresh();
	SInterface<ILinkManager> pLM(g_pScriptManager);
	SInterface<IRelation> pRel(pLM->GetRelation(self->flavor));
	if (!pRel)
	{
		// Should never happen
		return S.error("invalid relation type");
	}
	const sRelationDataDesc* pDataDesc = pRel->DescribeData();
	const char* pData = reinterpret_cast<const char*>(pRel->GetData(self->link));
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

