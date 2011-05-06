/******************************************************************************
 *    LgMultiParm.h
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
#ifndef LGMULTIPARM_H
#define LGMULTIPARM_H

#include <lg/objstd.h>
#include <lg/types.h>

#include "luax.h"

namespace Lgs
{

class ScriptMultiParm
{
public:
	~ScriptMultiParm()
		{ }
	ScriptMultiParm(luax::State& L)
		: m_lua(L)
		{ }

	operator const cMultiParm& ()
		{ return pop(-1); }
	ScriptMultiParm& operator= (const cMultiParm& mp)
		{ push(mp); return *this; }

	void push(const cMultiParm& mp);
	// misnomer -- doesn't actually pop the stack
	const cMultiParm& pop(int ix = -1);

private:
	luax::State& m_lua;
	cMultiParm m_data;
};

} // namespace Lgs

#endif // LGMULTIPARM_H
