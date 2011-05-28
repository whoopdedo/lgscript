/******************************************************************************
 *  LgObject.h
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
#ifndef LGOBJECT_H
#define LGOBJECT_H

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/types.h>
#include <lg/scrmanagers.h>
#include <lg/scrservices.h>
#include <lg/objects.h>
#include <lg/interface.h>

#include "luax.h"

namespace Lgs
{

class LgObject
{
	int id;
	static SInterface<IObjectSystem> ObjSys;
	static SInterface <ITraitManager> TraitMan;
	static SService<IObjectSrv> ObjSrv;

	static const luax::Registry Methods[];
	static const luax::Registry Properties[];

	static int Index(luax::Handle L);

	static LgObject* Check(luax::State& S, int arg);

	static int ExistsMethod(luax::Handle L);
	static int HasDonorIntrinsicallyMethod(luax::Handle L);
	static int HasDonorMethod(luax::Handle L);
	static int SetNameMethod(luax::Handle L);
	static int ArchetypeProperty(luax::Handle L);
	static int FacingProperty(luax::Handle L);
	static int IdProperty(luax::Handle L);
	static int NameProperty(luax::Handle L);
	static int PositionProperty(luax::Handle L);

	LgObject(const LgObject& o) : id(o.id) { };
public:
	~LgObject() { };
	LgObject() : id(0) { };
	LgObject(int i) : id(i) { };
	LgObject(const char* name);

	static int pop(luax::State& S, int arg);

	static void Init(luax::State& S);
	static const char s_ClassName[];

private:
	void* operator new(size_t);
public:
	void* operator new(size_t, luax::State&);
	void operator delete(void*, luax::State&);
	void operator delete(void*);
};

} // namespace Lgs

#endif
