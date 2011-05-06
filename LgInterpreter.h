/******************************************************************************
 *    ScriptInterpreter.h
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
#ifndef LGSCRIPTINTERPRETER_H
#define LGSCRIPTINTERPRETER_H

#include "Script.h"

#include "luax.h"


namespace Lgs
{

class ScriptInterpreter
{
public:
	~ScriptInterpreter();
	ScriptInterpreter(bool bEditor);

	IScript* LoadScript(const char* pszName, int iObjId);

private:
	luax::State* m_pLua;
	bool m_bEditor;

	void Init(void);

	// Load script chunk. Name is passed on the stack.
	int Chunk(void);
	// Create environment for script. Stack is [chunk script]
	int Environment(void);

	static const char s_ScriptRegistry[];
	static const char s_ScriptCache[];

	static int Traceback(luax::Handle L);
	static int NoTraceback(luax::Handle L);
	static int Print(luax::Handle L);

protected:
	luax::State& Lua(void)
		{ return *m_pLua; }
	//luax::State Thread(void);
	void UnloadScript(const char* pszName, int iObjId);
	// Push a registry table.
	static int Registry(luax::State& S, const char* pszKey);
	int Cache(void) { return Registry(*m_pLua, s_ScriptCache); }
	int Instance(void) { return Registry(*m_pLua, s_ScriptRegistry); }
	int Instance(luax::State& S) { return Registry(S, s_ScriptRegistry); }

	void PushTraceback(luax::State& S, IScript* pScript);

	friend class LgScript;
};

} // namespace Lgs

#endif
