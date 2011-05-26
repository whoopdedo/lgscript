/******************************************************************************
 *  LgLink.h
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
#ifndef LGLINK_H
#define LGLINK_H

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/types.h>
#include <lg/links.h>
#include <lg/interface.h>

#include "luax.h"

namespace Lgs
{

class LgLink
{
	long link;
	SInterface<IRelation> rel;
	static SInterface<ILinkManager> LinkMan;

	void Refresh(void);
	bool Refresh(sLink&);

	static const luax::Registry Methods[];
	static const luax::Registry Properties[];

	static int Index(luax::Handle L);
	static int LinkGet(luax::Handle L);
	static int GetData(luax::Handle L);
	static int SetData(luax::Handle L);
	static int IdProperty(luax::Handle L);
	static int FlavorProperty(luax::Handle L);
	static int DestProperty(luax::Handle L);
	static int SourceProperty(luax::Handle L);

	static LgLink* Check(luax::State& S, int arg);

	LgLink(const LgLink& l) : link(l.link) { Refresh(); };
public:
	~LgLink() { };
	LgLink() : link(0) { };
	LgLink(long l) : link(l) { Refresh(); };

	static long pop(luax::State& S, int arg);

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
