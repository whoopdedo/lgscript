/******************************************************************************
 *  ScriptModule.cpp
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

#include "ScriptModule.h"

#include <exception>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <io.h>

#include <lg/malloc.h>
#include <lg/editor.h>

class cScrClassDesc : public sScrClassDesc
{
public:
	cScrClassDesc(const char* mod, const char* cl, const char* base, ScriptFactoryProc proc)
	{
		pszClass = cl;
		pszModule = mod;
		pszBaseClass = base;
		pfnFactory = proc;
	}
};


static int __cdecl NullPrintf(const char*, ...);

IMalloc* g_pMalloc = NULL;
IScriptMan* g_pScriptManager = NULL;
volatile MPrintfProc g_pfnMPrintf = NullPrintf;

const char ScriptModule::s_ScriptModuleName[] = "lgs";

static int __cdecl NullPrintf(const char*, ...)
{
	return 0;
}

int ScriptModule::MPrintf(const char* pszFormat, ...)
{
	// No need to be concerned about threads or reentrancy or blowing the stack.
	static char psz[1000];
	va_list va;
	va_start(va, pszFormat);
	_vsnprintf(psz, 1000, pszFormat, va);
	va_end(va);
	return g_pfnMPrintf("%s", psz);
}

void ScriptModule::PostMessage(int iSrc, int iDest, const char* psz,
				const cMultiParm& data1, const cMultiParm& data2, const cMultiParm& data3, ulong flags)
{
#ifdef __GNUC__
	asm("push edi\n"
	"\tmov edi,esp\n"
	"\tsub esp,0x20\n"
	"\tmov eax,%7\n"
	"\tmov dword ptr [esp+0x1C],eax\n"
	"\tmov eax,%0\n"
	"\tmov edx,dword ptr [eax]\n"
	"\tmov ecx,%6\n"
	"\tmov dword ptr [esp+0x18],ecx\n"
	"\tmov ecx,%5\n"
	"\tmov dword ptr [esp+0x14],ecx\n"
	"\tmov ecx,%4\n"
	"\tmov dword ptr [esp+0x10],ecx\n"
	"\tmov ecx,%3\n"
	"\tmov dword ptr [esp+0xC],ecx\n"
	"\tmov ecx,%2\n"
	"\tmov dword ptr [esp+0x8],ecx\n"
	"\tmov ecx,%1\n"
	"\tmov dword ptr [esp+0x4],ecx\n"
	"\tmov dword ptr [esp],eax\n"
	"\tcall dword ptr [edx+0x6C]\n"
	"\tmov esp,edi\n"
	"\tpop edi\n"
	:: "m"(g_pScriptManager), "g"(iSrc), "g"(iDest), "g"(psz), "g"(&data1), "g"(&data2), "g"(&data3), "g"(flags)
	);
#else
	_asm {
		push edi
		mov  edi,esp
		sub  esp,0x20
		mov  eax,flags
		mov  dword ptr [esp+0x1C],eax
		mov  eax,g_pScriptManager
		mov  edx,dword ptr [eax]
		mov  ecx,&data3
		mov  dword ptr [esp+0x18],ecx
		mov  ecx,&data2
		mov  dword ptr [esp+0x14],ecx
		mov  ecx,&data1
		mov  dword ptr [esp+0x10],ecx
		mov  ecx,psz
		mov  dword ptr [esp+0xC],ecx
		mov  ecx,dword ptr iDest
		mov  dword ptr [esp+0x8],ecx
		mov  ecx,dword ptr iSrc
		mov  dword ptr [esp+0x4],ecx
		mov  dword ptr [esp],eax
		call dword ptr [edx+0x6C]
		mov  esp,edi
		pop  edi
	}
#endif
}

ScriptModule::~ScriptModule()
{
	delete m_pScriptInterpreter;
	const sScrClassDesc* p = m_ScriptsArray.begin();
	for (; p != m_ScriptsArray.end(); ++p)
		delete[] p->pszClass;
	if (m_pszName != s_ScriptModuleName)
		delete[] m_pszName;
}

ScriptModule::ScriptModule()
	    : m_pScriptInterpreter(NULL)
{
	m_pszName = const_cast<char*>(s_ScriptModuleName);
}

void ScriptModule::DisposeRef(void)
{
	delete m_pScriptInterpreter;
	m_pScriptInterpreter = NULL;
	const sScrClassDesc* p = m_ScriptsArray.begin();
	for (; p != m_ScriptsArray.end(); ++p)
		delete[] p->pszClass;
	m_ScriptsArray.clear();
	if (m_pszName != s_ScriptModuleName)
		delete[] m_pszName;
	m_pszName = const_cast<char*>(s_ScriptModuleName);
	cInterfaceImp<IScriptModule,IID_Def<IScriptModule>,kInterfaceImpStatic>::DisposeRef();
}

bool ScriptModule::Init(const char* pszName, bool bEditor)
{
	try
	{
	if (m_pszName != s_ScriptModuleName)
		delete[] m_pszName;
	if (pszName)
	{
		m_pszName = new char[strlen(pszName)+1];
		strcpy(m_pszName, pszName);
	}
	else
		m_pszName = const_cast<char*>(s_ScriptModuleName);

	m_pScriptInterpreter = new Lgs::ScriptInterpreter(bEditor);
	}
	catch (std::exception& err)
	{
		MPrintf("!!! Exception during initialization: %s\n", err.what());
		return false;
	}
	catch (...)
	{
		MPrintf("!!! Unhandled exception in ScriptModule::Init\n");
		return false;
	}
	return true;
}

const char* __stdcall ScriptModule::GetName(void)
{
	return m_pszName;
}

const sScrClassDesc* __stdcall ScriptModule::GetFirstClass(tScrIter* pIterParam)
{
	const sScrClassDesc* pRet;
	try
	{
		if (m_ScriptsArray.size() == 0)
		{
			if (0 == BuildScripts())
				return NULL;
		}
		*reinterpret_cast<unsigned int*>(pIterParam) = 0;
		pRet = &m_ScriptsArray[0];
	}
	catch (std::exception& err)
	{
		MPrintf("!!! Exception while enumerating scripts: %s\n", err.what());
		return NULL;
	}
	catch (...)
	{
		MPrintf("!!! Unhandled exception in ScriptModule::GetFirstClass\n");
		return NULL;
	}
	return pRet;
}

const sScrClassDesc* __stdcall ScriptModule::GetNextClass(tScrIter* pIterParam)
{
	const sScrClassDesc* pRet = NULL;
	try
	{
		register unsigned int index = *reinterpret_cast<unsigned int*>(pIterParam);
		if (++index < m_ScriptsArray.size())
			pRet = &m_ScriptsArray[index];
		*reinterpret_cast<unsigned int*>(pIterParam) = index;
	}
	catch (std::exception& err)
	{
		MPrintf("!!! Exception while enumerating scripts: %s\n", err.what());
		return NULL;
	}
	catch (...)
	{
		MPrintf("!!! Unhandled exception in ScriptModule::GetNextClass\n");
		return NULL;
	}
	return pRet;
}

void __stdcall ScriptModule::EndClassIter(tScrIter*)
{
	// Nothing to do here
}

unsigned long ScriptModule::BuildScripts(void)
{
	MPrintf("=== Rebuilding LgScript name cache...\n");
	const sScrClassDesc* p = m_ScriptsArray.begin();
	for (; p != m_ScriptsArray.end(); ++p)
		delete[] p->pszClass;
	m_ScriptsArray.clear();

	char* pszModule = m_pszName;
	const char* pszBaseClass = "LgScript";
	ScriptFactoryProc pfnFactory = &ScriptFactory;
	_finddata_t ffdata;
	char ffpath[FILENAME_MAX];
	strcpy(ffpath, ".\\scripts\\");
	strcat(ffpath, "*");
	int hFFile = _findfirst(ffpath, &ffdata);
	if (hFFile != -1)
	{
		try
		{
		do
		{
			if (ffdata.name[0] != '_' &&
			    (ffdata.attrib & (_A_HIDDEN|_A_SYSTEM|_A_SUBDIR)) == 0)
			{
				char* fileext = strrchr(ffdata.name, '.');
				size_t namelen = fileext - ffdata.name;
				if ((_stricmp(fileext, ".luac") == 0 ||
					 _stricmp(fileext, ".lua") == 0) &&
					_strnicmp(ffdata.name, "init", namelen) != 0)
				{
					char* pszClass = new char[namelen+1];
					memcpy(pszClass, ffdata.name, namelen);
					pszClass[namelen] = '\0';
					m_ScriptsArray.append(cScrClassDesc(pszModule,pszClass,pszBaseClass,pfnFactory));
				}
			}
		}
		while (_findnext(hFFile, &ffdata) != -1);
		}
		catch (std::exception& err)
		{
			MPrintf("!!! Exception while enumerating scripts: %s\n", err.what());
		}
		catch (...)
		{
			MPrintf("!!! Unhandled exception in ScriptModule::BuildScripts\n");
		}
		_findclose(hFFile);
	}
	MPrintf("--- Found %d scripts\n", m_ScriptsArray.size());
	return m_ScriptsArray.size();
}

ScriptModule  g_ScriptModule;

IScript* __cdecl ScriptModule::ScriptFactory(const char* pszName, int iObjId)
{
	IScript* pScript;
	try
	{
		pScript = g_ScriptModule.m_pScriptInterpreter->
				LoadScript(pszName, iObjId);
	}
	catch (std::exception& err)
	{
		MPrintf("!!! Cannot create script [%s:%d]: %s\n",
			pszName, iObjId, err.what());
		return NULL;
	}
	catch (...)
	{
		MPrintf("!!! Cannot create script [%s:%d]: Unknown error\n",
			pszName, iObjId);
		return NULL;
	}
	return pScript;
}

extern "C"
Bool __declspec(dllexport) __stdcall
ScriptModuleInit (const char* pszName,
                  IScriptMan* pScriptMan,
                  MPrintfProc pfnMPrintf,
                  IMalloc* pMalloc,
                  IScriptModule** pOutInterface)
{
	*pOutInterface = NULL;

	g_pScriptManager = pScriptMan;
#ifdef _DEBUG
	pMalloc->QueryInterface(IID_IDebugMalloc, reinterpret_cast<void**>(&g_pMalloc));
	if (!g_pMalloc)
		g_pMalloc = pMalloc;
#else
	g_pMalloc = pMalloc;
#endif

	if (pfnMPrintf)
		g_pfnMPrintf = pfnMPrintf;

	if (!g_pScriptManager || !g_pMalloc)
		return FALSE;

	bool bEditor = false;
	IUnknown* pIFace;
	if (pScriptMan->QueryInterface(IID_IEditTools, reinterpret_cast<void**>(&pIFace)) == S_OK)
	{
		bEditor = true;
		pIFace->Release();
	}
	if (!g_ScriptModule.Init(pszName, bEditor))
		return FALSE;
	g_ScriptModule.QueryInterface(IID_IScriptModule, reinterpret_cast<void**>(pOutInterface));

	return TRUE;
}
