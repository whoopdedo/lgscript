/******************************************************************************
 *  ScriptModule.h
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
#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/interfaceimp.h>
#include <lg/types.h>
#include <lg/dynarray.h>
#include <lg/script.h>
#include <lg/scrmanagers.h>

#include "LgInterpreter.h"

extern IMalloc* g_pMalloc;
extern IScriptMan* g_pScriptManager;

typedef int (__cdecl *MPrintfProc)(const char*, ...);
// GCC 3.3 is foolishly trying to optimize away the dereference here.
extern volatile MPrintfProc g_pfnMPrintf;

class ScriptModule : public cInterfaceImp<IScriptModule,IID_Def<IScriptModule>,kInterfaceImpStatic>
{
public:
	// IUnknown
	//STDMETHOD(QueryInterface)(REFIID,void**);
	//STDMETHOD_(ulong,AddRef)(void);
	//STDMETHOD_(ulong,Release)(void);
	// IScriptModule
	STDMETHOD_(const char*,GetName)(void);
	STDMETHOD_(const sScrClassDesc*,GetFirstClass)(tScrIter*);
	STDMETHOD_(const sScrClassDesc*,GetNextClass)(tScrIter*);
	STDMETHOD_(void,EndClassIter)(tScrIter*);

protected:
	virtual void DisposeRef(void);

private:
	char* m_pszName;
	static const char s_ScriptModuleName[];

	cDynArray<sScrClassDesc> m_ScriptsArray;

	Lgs::ScriptInterpreter* m_pScriptInterpreter;

	static IScript* __cdecl ScriptFactory(const char* pszName, int iObjId);
public:
	virtual ~ScriptModule();
	ScriptModule();

	bool Init(const char* pszName, bool bEditor);
	unsigned long BuildScripts(void);

	static int MPrintf(const char*, ...);
	static void PostMessage(int, int, const char*, const cMultiParm&, const cMultiParm&, const cMultiParm&, ulong);
};
