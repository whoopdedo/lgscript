/******************************************************************************
 *    LgScript.h
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
#ifndef LGSCRIPT_H
#define LGSCRIPT_H

#include <lg/types.h>
#include <lg/script.h>
#include <lg/scrmsgs.h>
#include <lg/scrmanagers.h>

#include "Script.h"

#define LUAX_INLINE
#include "luax.h"

namespace Lgs
{

class ScriptInterpreter;

class LgScript : public cScript
{
public:
	virtual ~LgScript();
	LgScript(ScriptInterpreter* pInterpreter, const char* pszName, int iHostObjId);

	long __stdcall ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	cMultiParm SendMessage(int iDest, const char* pszMessage, const cMultiParm& mpData1, const cMultiParm& mpData2, const cMultiParm& mpData3);
	void PostMessage(int iDest, const char* pszMessage, const cMultiParm& mpData1, const cMultiParm& mpData2, const cMultiParm& mpData3);
	tScrTimer SetTimedMessage(const char* pszName, unsigned long iTime, eScrTimedMsgKind eType, const cMultiParm& mpData);
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
	void BeginScript(void);
	void EndScript(void);

	friend class ScriptInterpreter;
	void* operator new(size_t, luax::State&);
	void operator delete(void*);

	static void Init(luax::State&);
	static LgScript* Check(luax::State&,int);

public:
	static const char s_ClassName[];

private:
	static const luax::Registry Methods[];

	static int IndexMethod(luax::Handle);
	static int NewIndexMethod(luax::Handle);
	static int SendMessageMethod(luax::Handle);
	static int PostMessageMethod(luax::Handle);
	static int SetTimedMessageMethod(luax::Handle);
	static int KillTimedMessageMethod(luax::Handle);
	static int IsScriptDataSetMethod(luax::Handle);
	static int GetScriptDataMethod(luax::Handle);
	static int SetScriptDataMethod(luax::Handle);
	static int ClearScriptDataMethod(luax::Handle);
};

} // namespace Lgs

#endif // LGSCRIPT_H
