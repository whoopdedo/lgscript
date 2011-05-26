/******************************************************************************
 *  LgObject.cpp
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
#include "LgObject.h"
#include "LgStructData.h"
#include "ScriptModule.h"
#include "mod/modlib.h"

#include <lg/scrservices.h>
#include <lg/scrmanagers.h>

using namespace Lgs;
using namespace luax;

void* LgObject::operator new(size_t s, State& L)
{
	void* p = L.newUserdata(s);
	L.getMetatable(s_ClassName).setMetatable();
	//L.copy().finally();
	return p;
}

void LgObject::operator delete(void*, State&)
{
	// allow GC
}

void LgObject::operator delete(void*)
{
	// allow GC
}

const char LgObject::s_ClassName[] = "LgObject";

const Registry LgObject::Methods[] = {
	{"Exists", ExistsMethod},
	{"HasMetaProperty", HasDonorIntrinsicallyMethod},
	{"InheritsFrom", HasDonorMethod},
	{"SetName", SetNameMethod},
	{NULL, NULL}
};

const Registry LgObject::Properties[] = {
	{"archetype", ArchetypeProperty},
	{"facing", FacingProperty},
	{"id", IdProperty},
	{"name", NameProperty},
	{"position", PositionProperty},
	{NULL, NULL}
};

void LgObject::Init(State& S)
{
	S.newMetatable(s_ClassName).registerLib(Methods).copy();
	S.createTable().registerLib(Properties);
	S.push(Index,2).setField("__index");
}

SInterface<IObjectSystem> LgObject::ObjSys;
SInterface<ITraitManager> LgObject::TraitMan;
SService<IObjectSrv> LgObject::ObjSrv;

LgObject::LgObject(const char* name)
{
	ObjSys.set(g_pScriptManager);
	id = ObjSys->GetObjectNamed(name);
}

inline LgObject* LgObject::Check(State& S, int arg)
{
	LgObject* self = S.checkUserdata(arg,Userdata<LgObject>());
	return self;
}

int LgObject::pop(State& S, int arg)
{
	if (S.isUserdata(arg))
	{
		LgObject* self = S.checkUserdata(arg,Userdata<LgObject>());
		return self ? self->id : 0;
	}
	else if (S.getType(arg) == TString)
	{
		ObjSys.set(g_pScriptManager);
		const char* name = S.asString(arg);
		return ObjSys->GetObjectNamed(name);
	}
	return S.optInteger(arg);
}

int LgObject::Index(luax::Handle L)
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

int LgObject::ExistsMethod(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	ObjSys.set(g_pScriptManager);
	S.push(bool(ObjSys->Exists(obj)));
	return 1;
}

int LgObject::HasDonorIntrinsicallyMethod(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	int donor = pop(S,2);
	if (!obj)
		return S.argError(1, "invalid object ID");
	if (!donor)
		return S.argError(2, "invalid object ID");
	TraitMan.set(g_pScriptManager);
	S.push(bool(TraitMan->ObjHasDonorIntrinsically(obj,donor)));
	return 1;
}

int LgObject::HasDonorMethod(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	int donor = pop(S,2);
	if (!obj)
		return S.argError(1, "invalid object ID");
	if (!donor)
		return S.argError(2, "invalid object ID");
	TraitMan.set(g_pScriptManager);
	S.push(bool(TraitMan->ObjHasDonor(obj,donor)));
	return 1;
}

int LgObject::SetNameMethod(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	const char* name = S.checkString(2,NULL);
	if (!obj)
		return S.argError(1, "invalid object ID");
	ObjSys.set(g_pScriptManager);
	ObjSys->NameObject(obj,name);
	return 0;
}

int LgObject::ArchetypeProperty(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	if (!obj)
		return S.argError(1, "invalid object ID");
	TraitMan.set(g_pScriptManager);
	new(S) LgObject(TraitMan->GetArchetype(obj));
	return 1;
}

int LgObject::IdProperty(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	S.push(obj);
	return 1;
}

int LgObject::NameProperty(luax::Handle L)
{
	State S(L);
	int obj = pop(S,1);
	if (!obj)
	{
		S.push(Nil());
		return 1;
	}
	ObjSys.set(g_pScriptManager);
	S.push(ObjSys->GetName(obj));
	return 1;
}

int LgObject::PositionProperty(luax::Handle L)
{
	State S(L);
	//LgObject* self = Check(S,1);
	int obj = pop(S,1);
	if (!obj)
	{
		S.push(Nil());
		return 1;
	}
	ObjSrv.set(g_pScriptManager);
	cScrVec ret;
	ObjSrv->Position(ret,obj);
	lmod_newvector(L, ret.x, ret.y, ret.z);
	return 1;
}

int LgObject::FacingProperty(luax::Handle L)
{
	State S(L);
	//LgObject* self = Check(S,1);
	int obj = pop(S,1);
	if (!obj)
	{
		S.push(Nil());
		return 1;
	}
	ObjSrv.set(g_pScriptManager);
	cScrVec ret;
	ObjSrv->Facing(ret,obj);
	lmod_newvector(L, ret.x, ret.y, ret.z);
	return 1;
}
