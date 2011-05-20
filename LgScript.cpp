/******************************************************************************
 *  LgScript.cpp
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
#include "luax.h"

#include "LgScript.h"
#include "LgInterpreter.h"
#include "LgMultiParm.h"
#include "LgMessage.h"
#include "ScriptModule.h"

#include "mod/ctype.h"

using namespace Lgs;
using namespace luax;

void* LgScript::operator new(size_t s, State& L)
{
	return L.newUserdata(s);
}

void LgScript::operator delete(void*, State&)
{
	// allow GC
}

void LgScript::operator delete(void*)
{
	// allow GC
}

const char LgScript::s_ClassName[] = "LgScript";

const Registry LgScript::Methods[] = {
	{"SendMessage", SendMessageMethod},
	{"PostMessage", PostMessageMethod},
	{"SetTimedMessage", SetTimedMessageMethod},
	{"KillTimedMessage", KillTimedMessageMethod},
	{"IsScriptDataSet", IsScriptDataSetMethod},
	{"GetScriptData", GetScriptDataMethod},
	{"SetScriptData", SetScriptDataMethod},
	{"ClearScriptData", ClearScriptDataMethod},
	{NULL, NULL}
};

void LgScript::Init(State& S)
{
	S.newMetatable(s_ClassName).registerLib(Methods).pop();
}

void LgScript::Environment(State& S)
{
	// Stack: env
	S.createTable(0, 3);  // env mt
	S.createTable().setField("upperindex");
	S.push(NewIndexEnvMethod).setField("__newindex");
	S.copy(LUA_GLOBALSINDEX).setField("__index");
	S.setMetatable();  // metatable(env) = mt
}

LgScript::~LgScript()
{
	try
	{
		if (m_iLastMsg >= 0)
			EndScript();
		m_pInterpreter->UnloadScript(Name(), ObjId());
	}
	catch (...)
	{
		ScriptModule::MPrintf("!!! Unhandled exception in destructor [%s:%d]\n",
			Name(), ObjId());
	}
}

LgScript::LgScript(ScriptInterpreter* pInterpreter, const char* pszName, int iObjId)
	    : cScript(pszName, iObjId),
	      m_pInterpreter(pInterpreter),
	      m_iLastMsg(-1), m_bFailed(false)
{
	// Script object will be at top of stack
	State S(pInterpreter->Lua());
	int iScript = S.getTop();
	int iMT = S.createTableI(0, 3);  // script mt
	S.createTable(0, 3)  // script mt index
	 .copy().setMetatable();  // metatable(index) = index
	S.push(pszName).setField("classname");  // index.classname = pszName
	S.push(iObjId).setField("objid");  // index.objid = iObjId
	S.copy(iScript)  // script mt index script
	 .getMetatable(s_ClassName)
	 .push(IndexMethod, 2)  // script mt index indexfunc<script,methods>
	 .setField("__index");  // script mt index
	S.setField("__index", iMT);  // script mt
	S.push(true)  // tag closure for quicker access
	 .push(SetScriptDataMethod, 1)
	 .setField("__newindex", iMT);
	S.push(s_ClassName).setField("class", iMT);
	S.setMetatable(iScript);  // script
}

STDMETHODIMP LgScript::ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	cScript::ReceiveMessage(pMsg, pReply, eTrace);
	long iRet = 0;
	if (!m_bFailed) try
	{
		if (m_iLastMsg < 0)
			BeginScript();
		if (strcasecmp(pMsg->message, "EndScript") != 0)
		{
			// Skip EndScript from the script manager
			// so we can guarantee it is always the last message.
			iRet = DispatchMessage(pMsg, static_cast<cMultiParm*>(pReply));
		}
	}
	catch (std::exception& e)
	{
		ScriptModule::MPrintf("!!! Unexpected exception in %s [%s:%d]: %s\n",
			pMsg->message, Name(), ObjId(), e.what());
		ScriptModule::MPrintf("!!! Bailing on script.\n");
		m_bFailed = true;
		iRet = -1;
	}
	catch (...)
	{
		ScriptModule::MPrintf("!!! Unhandled exception in %s [%s:%d]\n",
			pMsg->message, Name(), ObjId());
		ScriptModule::MPrintf("!!! Bailing on script.\n");
		m_bFailed = true;
		iRet = -1;
	}
	m_iLastMsg = pMsg->time;
	return iRet;
}

long LgScript::DispatchMessage(sScrMsg* pMsg, cMultiParm* pReply)
{
	try
	{
	Frame S(m_pInterpreter->Lua());
	S->checkStack(LUA_MINSTACK);
	int iCache = m_pInterpreter->Instance(S);
	if (S->push(Userdata<IScript>(this)).rawGet(iCache).isNil())
	{
		ScriptModule::MPrintf("!!! Can't find script instance %p [%s:%d]\n",
			this, Name(), ObjId());
		ScriptModule::MPrintf("!!! Bailing on script.\n");
		m_bFailed = true;
		return 1;
	}
	LookupMessage(S, pMsg->message);
	//S->push(pMsg->message).getTable();
	if (S->isFunction())
	{
		m_pInterpreter->PushTraceback(S, this);
		S->insert();  // screnv traceback function
		ScriptMessage(S).push(pMsg);  // screnv traceback function message
		// Message references cannot be kept outside of the message handler.
		sScrMsg** pScrMsg = S->toUserdata(Userdata<sScrMsg*>(), -1);
		try
		{
			S->fCall(1, 1, -3);
			if (pReply)
				*pReply = ScriptMultiParm(S);
		}
		catch (Exception&)
		{
			if (*pScrMsg)
				*pScrMsg = NULL;
			// Need to do this in case there is an open linkset.
			S->gcCollect();
		}
		if (*pScrMsg)
			*pScrMsg = NULL;
	}
	}
	catch (Exception&)
	{
		m_pInterpreter->Lua().gcCollect();
		throw;
	}
	return 0;
}

void LgScript::LookupMessage(State& S, const char* pszMessage)
{
	int iScript = S.getTop();
	S.getMetatable(iScript).getField("upperindex");
	Buffer upper(S);
	for (const char* psz = pszMessage; *psz; psz++)
		upper.add(toupper(*psz));
	upper.push();
	if (!S.rawGet().isNil())
	{
		S.getTable(iScript);
	}
	S.insert(iScript+1).setTop(iScript+1);
}

void LgScript::BeginScript(void)
{
	ScriptModule::MPrintf("=== BeginScript [%s:%d]\n",
			Name(), ObjId());
}

void LgScript::EndScript(void)
{
	sScrMsg* msg = new sScrMsg();
	msg->message = "EndScript";
	msg->time = m_iLastMsg;
	try
	{
	DispatchMessage(msg, NULL);
	}
	catch (Exception&)
	{ }
	msg->Release();
	ScriptModule::MPrintf("=== EndScript [%s:%d]\n",
			Name(), ObjId());
}

cMultiParm LgScript::SendMessage(int iDest, const char* pszMessage,
				const cMultiParm& mpData1,
				const cMultiParm& mpData2,
				const cMultiParm& mpData3)
{
	cMultiParm mpReply;
	g_pScriptManager->SendMessage2(mpReply, ObjId(), iDest, pszMessage, mpData1, mpData2, mpData3);
	return mpReply;
}

void LgScript::PostMessage(int iDest, const char* pszMessage,
				const cMultiParm& mpData1,
				const cMultiParm& mpData2,
				const cMultiParm& mpData3,
				unsigned long flags)
{
	g_pScriptManager->PostMessage2(ObjId(), iDest, pszMessage, mpData1, mpData2, mpData3, flags);
}

tScrTimer LgScript::SetTimedMessage(const char* pszName, unsigned long iTime, eScrTimedMsgKind eType,
				const cMultiParm& mpData)
{
	return g_pScriptManager->SetTimedMessage2(ObjId(), pszName, iTime, eType, mpData);
}

void LgScript::KillTimedMessage(tScrTimer hTimer)
{
	g_pScriptManager->KillTimedMessage(hTimer);
}

bool LgScript::IsScriptDataSet(const char* pszName)
{
	sScrDatumTag tag;
	tag.objId = ObjId();
	tag.pszClass = Name();
	tag.pszName = pszName;
	return g_pScriptManager->IsScriptDataSet(&tag);
}

cMultiParm LgScript::GetScriptData(const char* pszName)
{
	cMultiParm mpRet;
	sScrDatumTag tag;
	tag.objId = ObjId();
	tag.pszClass = Name();
	tag.pszName = pszName;
	g_pScriptManager->GetScriptData(&tag, &mpRet);
	return mpRet;
}

long LgScript::SetScriptData(const char* pszName, const cMultiParm& mpData)
{
	sScrDatumTag tag;
	tag.objId = ObjId();
	tag.pszClass = Name();
	tag.pszName = pszName;
	return g_pScriptManager->SetScriptData(&tag, &mpData);
}

cMultiParm LgScript::ClearScriptData(const char* pszName)
{
	cMultiParm mpRet;
	sScrDatumTag tag;
	tag.objId = ObjId();
	tag.pszClass = Name();
	tag.pszName = pszName;
	g_pScriptManager->ClearScriptData(&tag, &mpRet);
	return mpRet;
}

LgScript* LgScript::Check(State& S, int arg)
{
	void* pv = S.toUserdata(arg);
	if (pv == NULL)
		return NULL;
	if (!S.isNone(Upvalue(1)))
	{
		// closure guarantees success
		return reinterpret_cast<LgScript*>(pv);
	}
	bool bIsScript = false;
	if (!S.getMetatable(arg).isNil())
	{
		bIsScript = !S.getField("class").isNil() &&
				(0 == strcmp(S.asString(), s_ClassName));
		S.pop();
	}
	S.pop();
	return bIsScript ? reinterpret_cast<LgScript*>(pv) : NULL;
}

int LgScript::IndexMethod(Handle L)
{
	State S(L);
	// upvalue(1) -> script
	// upvalue(2) -> method table
	// stack: metatable key
	if (!S.copy(2).rawGet(Upvalue(2)).isNil())  // mt key val
		return 1;
	S.pop();
	LgScript* pScript = S.toUserdata(Userdata<LgScript>(),Upvalue(1));
	if (!S.isString(2))
	{
		S.push(Nil());
		return 1;
	}
	const char* pszKey = S.asString(2);
	ScriptMultiParm(S).push(pScript->GetScriptData(pszKey));
	return 1;
}

int LgScript::SendMessageMethod(Handle L)
{
	State S(L);
	// Stack: (script|objid) dest message ...
	LgScript* pScript = Check(S, 1);
	S.setTop(6);
	if (pScript)
	{
		// TODO: object userdata
		int iTo = S.checkInteger(2);
		const char* pszMsg = S.checkString(3,NULL);
		ScriptMultiParm mp1(S), mp2(S), mp3(S);
		ScriptMultiParm(S).push(
			pScript->SendMessage(iTo, pszMsg,
				mp1.pop(4), mp2.pop(5), mp3.pop(6)));
		return 1;
	}
	else
	{
		// TODO: object userdata
		int iFrom = S.checkInteger(1);
		int iTo = S.checkInteger(2);
		const char* pszMsg = S.checkString(3,NULL);
		ScriptMultiParm mp1(S), mp2(S), mp3(S);
		cMultiParm mpReply;
		g_pScriptManager->SendMessage2(mpReply, iFrom, iTo, pszMsg,
			mp1.pop(4), mp2.pop(5), mp3.pop(6));
		ScriptMultiParm(S).push(mpReply);
		return 1;
	}
	return 0;
}

int LgScript::PostMessageMethod(Handle L)
{
	static const char* const MsgFlags[] = {
		"1", "2", "sendtoproxy", "posttoowner", NULL };
	State S(L);
	// Stack: (script|objid) dest message ...
	LgScript* pScript = Check(S, 1);
	S.setTop(7);
	if (pScript)
	{
		// TODO: object userdata
		int iTo = S.checkInteger(2);
		const char* pszMsg = S.checkString(3,NULL);
		unsigned long flags = S.checkFlags(7, MsgFlags, "");
		ScriptMultiParm mp1(S), mp2(S), mp3(S);
		pScript->PostMessage(iTo, pszMsg,
				mp1.pop(4), mp2.pop(5), mp3.pop(6), flags);
		return 0;
	}
	else
	{
		// TODO: object userdata
		int iFrom = S.checkInteger(1);
		int iTo = S.checkInteger(2);
		const char* pszMsg = S.checkString(3,NULL);
		unsigned long flags = S.checkFlags(7, MsgFlags, "");
		ScriptMultiParm mp1(S), mp2(S), mp3(S);
		g_pScriptManager->PostMessage2(iFrom, iTo, pszMsg,
			mp1.pop(4), mp2.pop(5), mp3.pop(6), flags);
		return 0;
	}
	return 0;
}

static eScrTimedMsgKind getTimedMessageType(State& S, int arg)
{
	static const char* const TimerTypes[] = {
		"oneshot", "periodic", NULL };
	if (S.isNil(arg) || S.isBoolean(arg))
		return S.toBoolean(arg) ? kSTM_Periodic : kSTM_OneShot;
	eScrTimedMsgKind type;
	switch (S.checkOption(arg, TimerTypes, "oneshot"))
	{
	default: type = kSTM_OneShot; break;
	case 1: type = kSTM_Periodic; break;
	}
	return type;
}

int LgScript::SetTimedMessageMethod(Handle L)
{
	State S(L);
	// Stack: (script|objid) message time type? data?
	tScrTimer hTimer;
	LgScript* pScript = Check(S, 1);
	S.setTop(5);
	if (pScript)
	{
		const char* pszMsg = S.checkString(2,NULL);
		int iTime = S.checkInteger(3);
		ScriptMultiParm mp(S);
		hTimer = pScript->SetTimedMessage(pszMsg, iTime,
			getTimedMessageType(S, 4), mp.pop(5));
	}
	else
	{
		// TODO: object userdata
		int iTo = S.checkInteger(1);
		const char* pszMsg = S.checkString(2,NULL);
		int iTime = S.checkInteger(3);
		ScriptMultiParm mp(S);
		hTimer = g_pScriptManager->SetTimedMessage2(iTo, pszMsg, iTime,
			getTimedMessageType(S, 4), mp.pop(5));
	}
	S.push(reinterpret_cast<int>(hTimer));
	return 1;
}

int LgScript::KillTimedMessageMethod(Handle L)
{
	State S(L);
	// Stack: (script timer) or (timer)
	tScrTimer hTimer;
	if (Check(S, 1))
		hTimer = reinterpret_cast<tScrTimer>(S.checkInteger(2));
	else
		hTimer = reinterpret_cast<tScrTimer>(S.checkInteger(1));
	if (hTimer)
		g_pScriptManager->KillTimedMessage(hTimer);
	return 0;
}

int LgScript::IsScriptDataSetMethod(Handle L)
{
	State S(L);
	// Stack: script key
	LgScript* pScript = Check(S, 1);
	if (!pScript)
	{
		return S.argError(1, "not a script");
	}
	const char* pszKey = S.checkString(2,NULL);
	S.push(bool(pScript->IsScriptDataSet(pszKey)));
	return 1;
}

int LgScript::GetScriptDataMethod(Handle L)
{
	State S(L);
	// Stack: script key
	LgScript* pScript = Check(S, 1);
	if (!pScript)
	{
		return S.argError(1, "not a script");
	}
	const char* pszKey = S.checkString(2,NULL);
	ScriptMultiParm(S).push(pScript->GetScriptData(pszKey));
	return 1;
}

int LgScript::SetScriptDataMethod(Handle L)
{
	State S(L);
	// Stack: script key value
	LgScript* pScript = Check(S, 1);
	if (!pScript)
	{
		return S.argError(1, "not a script");
	}
	const char* pszKey = S.checkString(2,NULL);
	S.setTop(3);
	pScript->SetScriptData(pszKey, ScriptMultiParm(S).pop(3));
	return 0;
}

int LgScript::ClearScriptDataMethod(Handle L)
{
	State S(L);
	// Stack: script key
	LgScript* pScript = Check(S, 1);
	if (!pScript)
	{
		return S.argError(1, "not a script");
	}
	const char* pszKey = S.checkString(2,NULL);
	pScript->ClearScriptData(pszKey);
	return 0;
}

int LgScript::NewIndexEnvMethod(Handle L)
{
	State S(L);
	// Stack: screnv key value
	S.rawSet(1, 2, 3);
	if (S.isFunction(3) && S.isString(2))
	{
		size_t len;
		const char* psz = S.asString(2, &len);
		S.getMetatable(1).getField("upperindex");  // screnv key value mt upper
		Buffer upper(S);
		while (len--)
			upper.add(toupper(*psz++));
		upper.push();
		S.copy(2).rawSet();
	}
	return 0;
}
