/******************************************************************************
 *    ScriptInterpreter.cpp
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
#include "luax.h"

#include "LgInterpreter.h"
#include "LgScript.h"
#include "LgMessage.h"
#include "LgLinkset.h"
#include "LgServices.h"
#include "ScriptModule.h"

using namespace Lgs;
using namespace luax;

const char ScriptInterpreter::s_ScriptRegistry[] = "LGSRegistry";
const char ScriptInterpreter::s_ScriptCache[] = "LGSCache";

int ScriptInterpreter::Print(Handle L)
{
	char buf[1024];
	State S(L);
	int num = S.getTop();
	S.getGlobal("tostring");
	for (int i=1; i<=num; i++)
	{
		S.copy().copy(i).call(1,1);
		size_t l;
		const char* s = S.asString(-1, &l);
		if (s == NULL)
			return S.error(LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		if (i > 1)
			ScriptModule::MPrintf("\t");
		while (l >= 1024)
		{
			strncpy(buf, s, 1023);
			buf[1023] = '\0';
			s += 1023;
			l -= 1023;
			ScriptModule::MPrintf("%s", buf);
		}
		ScriptModule::MPrintf("%s", s);
		S.pop();
	}
	ScriptModule::MPrintf("\n");
	return 0;
}

int ScriptInterpreter::Traceback(Handle L)
{
	State S(L);
	int err = 1;
	LgScript* pScript = NULL;
	if (!S.isNoneOrNil(Upvalue(1)))
		pScript = S.toUserdata(Userdata<LgScript>(),Upvalue(1));
	if (!S.isString(1))  /* 'message' not a string? */
	{
		err = S_pushLiteral(S,"Unknown error").getTop();
	}
	if (!S.getGlobal("debug").isTable())
	{
		S.setTop(1);
		return 1;
	}
	if (!S.getField("traceback").isFunction())
	{
		S.setTop(1);
		return 1;
	}
	S.copy(err)	// pass error message
	 .push(2)	// skip this function and traceback
	 .call(2,1);	// call debug.traceback
	if (pScript)
		ScriptModule::MPrintf("!!! [%s:%d] %s\n",
				pScript->Name(), pScript->ObjId(),
				S.asString());
	else
		ScriptModule::MPrintf("!!! %s\n", S.asString());
	if (err != 1)
		S.setTop(1);
	return 1;
}

int ScriptInterpreter::NoTraceback(Handle L)
{
	State S(L);
	int err = 1;
	LgScript* pScript = NULL;
	if (!S.isNoneOrNil(Upvalue(1)))
		pScript = S.toUserdata(Userdata<LgScript>(),Upvalue(1));
	if (!S.isString(1))  /* 'message' not a string? */
	{
		err = S_pushLiteral(S,"Unknown error").getTop();
	}
	if (pScript)
		ScriptModule::MPrintf("!!! [%s:%d] %s\n",
				pScript->Name(), pScript->ObjId(),
				S.asString());
	else
		ScriptModule::MPrintf("!!! %s\n", S.asString());
	if (err != 1)
		S.setTop(1);
	return 1;
}

ScriptInterpreter::~ScriptInterpreter()
{
	delete static_cast<MainState*>(m_pLua);
}

ScriptInterpreter::ScriptInterpreter(bool bEditor)
	: m_pLua(NULL), m_bEditor(bEditor)
{
	ScriptModule::MPrintf(
	"=== This is LgScript 1.0 Copyright (C) 2009 Tom N Harris\n"
	"=== Using " LUA_RELEASE " " LUA_COPYRIGHT "\n"
	"=== LgScript comes with ABSOLUTELY NO WARRANTY; see the GNU General\n"
	"=== Public License for more details.\n"
	"=== This is free software, and you are welcome to redistribute it\n"
	"=== under certain conditions; for details see the copyright file\n"
	"=== that is distributed with this script module.\n\n"
	);

	m_pLua = new MainState();
	m_pLua->gcStop();
	static_cast<MainState*>(m_pLua)->openLibs();
	Init();
	LgScript::Init(*m_pLua);
	ScriptMessage::Init(*m_pLua);
	LinkSet::Init(*m_pLua);
	ScriptServices::Init(*m_pLua);
	m_pLua->gcRestart();
}

void ScriptInterpreter::Init(void)
{
	// Create chunk cache
	m_pLua->createTable(0,0).setField(s_ScriptCache, LUA_REGISTRYINDEX);
	// Create script instance cache
	m_pLua->createTable(0,0).copy().setMetatable();
	S_pushLiteral(*m_pLua, "k").setField("__mode");
	m_pLua->setField(s_ScriptRegistry, LUA_REGISTRYINDEX);
	if (m_bEditor)
	{
		// Replace `print'
		m_pLua->push(Print).setField("print", LUA_GLOBALSINDEX);
	}
}

int ScriptInterpreter::Registry(State& S, const char* pszKey)
{
	return S.getFieldI(pszKey, LUA_REGISTRYINDEX);
}

IScript* ScriptInterpreter::LoadScript(const char* pszName, int iObjId)
{
	IScript* pScript = NULL;
	Frame S(*m_pLua);
	S->push(pszName);  // name
	S->copy().push(iObjId).concat(); // name nameobjid
	//S->getField(s_ScriptCache, LUA_REGISTRYINDEX);
	int iCache = Cache();  // name nameobjid LGS
	if (!S->rawGet(iCache, -2).isNil())  // name nameobjid LGS script
	{
S->getGlobal("print").push("*** CACHE > ").copy(-5).copy(-4).call(3,0);
		pScript = S->toUserdata(Userdata<IScript>());
		pScript->AddRef();
		return pScript;
	}
	S->pop();  // name nameobjid LGS
	S->copy(-3).push(0).concat(); // name nameobjid LGS name0
	if (S->rawGet(iCache, -1).isNil())  // name nameobjid LGS name0 chunk
	{
		S->pop().copy(-4);  // name nameobjid LGS name0 name
		if (0 != Chunk())  // name nameobjid LGS name0 chunk
		{
			ScriptModule::MPrintf("!!! Script not found [%s]\n", pszName);
			ScriptModule::MPrintf("!!! %s\n", S->asString());
			return NULL;
		}
// name nameobjid LGS name0 chunk print "*** CACHE " name0 chunk
S->getGlobal("print").push("*** CACHE < ").copy(-4).copy(-4).call(3,0);
		S->rawSet(iCache, -2, -1);  // cache[name0] = chunk
	}
	S->remove(-2);  // name nameobjid LGS chunk
	pScript = new(*m_pLua) LgScript(this, pszName, iObjId);
	S->insert(-2).copy(-2);  // name nameobjid LGS script chunk script
	if (0 != Environment())  // name nameobjid LGS script screnv
	{
		// no need to delete script, GC will clean it up
		return NULL;
	}
// name nameobjid LGS script screnv print "*** CACHE " nameobjid script
S->getGlobal("print").push("*** CACHE < ").copy(-6).copy(-5).call(3,0);
	S->rawSet(iCache, -4, -2);  // cache[nameobjid] = script
	int iScripts = Instance();   // name nameobjid LGS script screnv Registry
	S->push(Userdata<IScript>(pScript)); // name nameobjid LGS script screnv Registry pointer
// ... LGS script screnv Reg pointer print "*** INSTANCE " pointer screnv
S->getGlobal("print").push("*** INSTANCE < ").copy(-3).copy(-6).call(3,0);
	S->copy(-3).rawSet(iScripts); // registry[pointer] = screnv
	return pScript;
}

void ScriptInterpreter::UnloadScript(const char* pszName, int iObjId)
{
	Frame S(*m_pLua);
	S->push(pszName);  // name
	S->copy().push(iObjId).concat();  // name nameobjid
S->getGlobal("print").push("*** CACHE X ").copy(-3).call(2,0);
	int iCache = Cache();  // name nameobjid LGS
	if (S->rawGet(iCache, -2).isNil())  // name nameobjid LGS script
	{
		ScriptModule::MPrintf("!!! Script not found. Possible memory leak. [%s:%d]\n",
				pszName, iObjId);
		return;
	}
	IScript* pScript = S->toUserdata(Userdata<IScript>());
	S->pop();  // name nameobjid LGS
	S->copy(-2).push(Nil()).rawSet(iCache);  // cache[nameobjid] = nil
S->getGlobal("print").push("*** CACHE X ").push(Userdata<IScript>(pScript)).call(2,0);
	int iScripts = Instance();  // name nameobjid LGS Registry
	S->push(Userdata<IScript>(pScript)).push(Nil()).rawSet(iScripts); // scripts[pointer] = nil
}

// Load script chunk. Name is passed on the stack.
int ScriptInterpreter::Chunk(void)
{
	State S(*m_pLua);
	int iName = S.getTop();
	int iLoaders = S.getGlobal("package").getFieldI("loaders");
	if (!S.isTable(iLoaders))
		S.error(LUA_QL("package.loaders") " must be a table");
	S_pushLiteral(S, "");
	for (int i=1; !S.rawGetN(i, iLoaders).isNil(); i++)
	{
		S.copy(iName).call(1, 1);
		if (S.isFunction())
		{
			S.insert(iName).setTop(iName);
			return 0;
		}
		if (S.isString())
			S.concat();
		else
			S.pop();
	}
	S.pop().insert(iName).setTop(iName);
	return 1;
}

// Create environment for script. Stack is [chunk script]
int ScriptInterpreter::Environment(void)
{
	State S(*m_pLua);
	int iChunk = S.getTop() - 1;
	S.createTable(0, 2);
	S.createTable(0, 2);  // chunk script env mt
	S.copy(LUA_GLOBALSINDEX).setField("__index");
	S.setMetatable();  // metatable(env) = mt
	S.insert(-2).setField("script");  // move environment below script
	S.copy().setFEnv(iChunk);
	S.insert(iChunk);  // move environment below chunk
	try
	{
		S.push(Traceback).insert(); // move traceback below chunk
		S.pCall(0, 0, -2);
	}
	catch (Exception&)
	{
		S.setTop(iChunk-1);
		return 1;
	}
	S.setTop(iChunk);  // env
	return 0;
}

void ScriptInterpreter::PushTraceback(State& S, IScript* pScript)
{
	S.push(Userdata<IScript>(pScript))
	 .push(m_bEditor ? Traceback : NoTraceback, 1);
}
