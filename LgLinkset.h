/******************************************************************************
 *    LgLinkset.h
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
#ifndef LGLINKSET_H
#define LGLINKSET_H

#include <lg/types.h>
#include <lg/links.h>

#define LUAX_INLINE
#include "luax.h"

namespace Lgs
{

class LinkSet
{
	ILinkQuery* query;
	long link;
	int source, dest;
	short flavor;

	void Refresh(void);

	static const luax::Registry Methods[];
	static const luax::Registry Meta[];

	static int Pairs(luax::Handle L);
	static int Iter(luax::Handle L);

	static LinkSet* Check(luax::State& S, int arg);

	LinkSet() : query(NULL) { };
	LinkSet(const LinkSet&) { };
public:
	~LinkSet() { if (query) query->Release(); };
	LinkSet(linkset& l)
		: link(0), source(0), dest(0), flavor(0)
	{
		query = l.query;
		if (query)
			query->AddRef();
	};
	LinkSet(ILinkQuery* q)
		: link(0), source(0), dest(0), flavor(0)
	{
		query = q;
		if (query)
			query->AddRef();
	};
	static void Init(luax::State& S);

	void* operator new(size_t, luax::State&);
	void operator delete(void*);

	static const char s_ClassName[];

	static int Release(luax::Handle L);
	static int Done(luax::Handle L);
	static int Next(luax::Handle L);
	static int Id(luax::Handle L);
	static int Link(luax::Handle L);
	static int Data(luax::Handle L);
};

}

#endif
