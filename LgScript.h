/******************************************************************************
 *  LgScript.h
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
#ifndef LGSCRIPT_H
#define LGSCRIPT_H

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/types.h>
#include <lg/script.h>
#include <lg/scrmsgs.h>
#include <lg/scrmanagers.h>

#include "Script.h"

#include "luax.h"

namespace Lgs
{

class ScriptInterpreter;

class LgScript : public cScript
{
public:
	virtual ~LgScript();
	LgScript(ScriptInterpreter* pInterpreter, const char* pszName, int iHostObjId);

	STDMETHOD(ReceiveMessage)(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	cMultiParm SendMessage(int iDest, const char* pszMessage,
				const cMultiParm& mpData1 = cMultiParm::Undef,
				const cMultiParm& mpData2 = cMultiParm::Undef,
				const cMultiParm& mpData3 = cMultiParm::Undef);
	void PostMessage(int iDest, const char* pszMessage,
				const cMultiParm& mpData1 = cMultiParm::Undef,
				const cMultiParm& mpData2 = cMultiParm::Undef,
				const cMultiParm& mpData3 = cMultiParm::Undef,
				unsigned long flags = 0);
	tScrTimer SetTimedMessage(const char* pszName, unsigned long iTime, eScrTimedMsgKind eType,
				const cMultiParm& mpData = cMultiParm::Undef);
	void KillTimedMessage(tScrTimer hTimer);

	bool IsScriptDataSet(const char* pszName);
	cMultiParm GetScriptData(const char* pszName);
	long SetScriptData(const char* pszName, const cMultiParm& mpData);
	cMultiParm ClearScriptData(const char* pszName);

private:
	ScriptInterpreter* m_pInterpreter;
	int m_iLastMsg;
	bool m_bFailed;

protected:
	long DispatchMessage(sScrMsg* pMsg, cMultiParm* pReply);
	void LookupMessage(luax::State&, const char*);
	void BeginScript(void);
	void EndScript(void);

	static void Init(luax::State&);
	static LgScript* Check(luax::State&,int);
	static void Environment(luax::State&);

	friend class ScriptInterpreter;
	void* operator new(size_t, luax::State&);
	void operator delete(void*, luax::State&);
	void operator delete(void*);
private:
	void* operator new(size_t);

public:
	static const char s_ClassName[];

private:
	static const luax::Registry Methods[];

	static int IndexMethod(luax::Handle);
	static int SendMessageMethod(luax::Handle);
	static int PostMessageMethod(luax::Handle);
	static int SetTimedMessageMethod(luax::Handle);
	static int KillTimedMessageMethod(luax::Handle);
	static int IsScriptDataSetMethod(luax::Handle);
	static int GetScriptDataMethod(luax::Handle);
	static int SetScriptDataMethod(luax::Handle);
	static int ClearScriptDataMethod(luax::Handle);
	static int NewIndexEnvMethod(luax::Handle);
};

} // namespace Lgs

#endif // LGSCRIPT_H
