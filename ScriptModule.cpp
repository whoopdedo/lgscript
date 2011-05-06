/******************************************************************************
 *    ScriptModule.cpp
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

#include "ScriptModule.h"

#include <exception>
#include <cstring>
#include <io.h>

#include <lg/iids.h>
#include <lg/scrmanagers.h>
#include <lg/malloc.h>

using namespace std;

static int __cdecl NullPrintf(const char*, ...);

IMalloc* g_pMalloc = NULL;
IScriptMan* g_pScriptManager = NULL;
volatile MPrintfProc g_pfnMPrintf = NullPrintf;

static int __cdecl NullPrintf(const char*, ...)
{
	return 0;
}

int ScriptModule::MPrintf(const char* pszFormat, ...)
{
	// Waste cycles and stack space so we don't blow-out Dromed's buffer
	va_list va;
	char psz[1000];
	va_start(va, pszFormat);
	_vsnprintf(psz, 1000, pszFormat, va);
	va_end(va);
	return g_pfnMPrintf("%s", psz);
}

ScriptModule::~ScriptModule()
{
	delete m_pScriptInterpreter;
	const sScrClassDesc* p = m_ScriptsArray.begin();
	for (; p != m_ScriptsArray.end(); ++p)
		delete[] p->pszClass;
	delete[] m_pszName;
}

ScriptModule::ScriptModule()
	    : m_iRef(1), m_pszName(NULL),
	      m_pScriptInterpreter(NULL)
{
}

/*
ScriptModule::ScriptModule(const char* pszName)
	    : m_iRef(1), m_pszName(NULL),
	      m_pScriptInterpreter(NULL)
{
	Init(pszName);
}
*/

HRESULT __stdcall ScriptModule::QueryInterface(REFIID riid, void** ppout)
{
	if (riid == IID_IUnknown)
		*ppout = static_cast<IUnknown*>(this);
	else if (riid == IID_IScriptModule)
		*ppout = static_cast<IScriptModule*>(this);
	else
		return E_NOINTERFACE;
	reinterpret_cast<IUnknown*>(*ppout)->AddRef();
	return S_OK;
}

ULONG __stdcall ScriptModule::AddRef(void)
{
	return ++m_iRef;
}

ULONG __stdcall ScriptModule::Release(void)
{
	// This object is static, so we don't delete it.
	int iRefCnt = --m_iRef;
	if (iRefCnt < 0)
	{
		iRefCnt = m_iRef = 0;
		delete m_pScriptInterpreter;
		m_pScriptInterpreter = NULL;
		const sScrClassDesc* p = m_ScriptsArray.begin();
		for (; p != m_ScriptsArray.end(); ++p)
			delete[] p->pszClass;
		m_ScriptsArray.clear();
		delete[] m_pszName;
		m_pszName = NULL;
	}
	return iRefCnt;
}

bool ScriptModule::Init(const char* pszName, bool bEditor)
{
	try
	{
	if (m_pszName)
		delete[] m_pszName;
	if (pszName)
	{
		m_pszName = new char[::strlen(pszName)+1];
		strcpy(m_pszName, pszName);
	}
	else
		m_pszName = NULL;

	m_pScriptInterpreter = new Lgs::ScriptInterpreter(bEditor);
	}
	catch (exception& err)
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
	catch (exception& err)
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
	catch (exception& err)
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

	sScrClassDesc scrdesc;
	scrdesc.pszModule = m_pszName;
	scrdesc.pszBaseClass = "LgScript";
	scrdesc.pfnFactory = reinterpret_cast<ScriptFactoryProc>(&ScriptFactory);
	_finddata_t ffdata;
	char ffpath[MAX_PATH];
	strcpy(ffpath, ".\\scripts\\");
	strcat(ffpath, "*");
	int hFFile = _findfirst(ffpath, &ffdata);
	if (hFFile != -1)
	{
		try
		{
		do
		{
			if (ffdata.name[0] == '_' ||
			    (ffdata.attrib & (_A_HIDDEN|_A_SYSTEM)) != 0)
				continue;
			if ((ffdata.attrib & _A_SUBDIR) == 0)
			{
				char* fileext = strrchr(ffdata.name, '.');
				size_t namelen = fileext - ffdata.name;
				if ((_stricmp(fileext, ".luac") == 0 ||
				     _stricmp(fileext, ".lua") == 0) &&
				    _strnicmp(ffdata.name, "init", namelen) != 0)
				{
					scrdesc.pszClass = new char[namelen+1];
					memcpy(scrdesc.pszClass, ffdata.name, namelen);
					scrdesc.pszClass[namelen] = '\0';
					m_ScriptsArray.append(scrdesc);
				}
			}
#if 0
			else
			{
				strncpy(ffpath, ".\\script\\", MAX_PATH);
				strncat(ffpath, ffdata.name, MAX_PATH);
				strncat(ffpath, "\\init.lua", MAX_PATH);
				if (_access(ffpath, F_OK) == 0)
				{
					size_t namelen = strlen(ffdata.name) + 1;
					scrdesc.pszClass = new char[namelen];
					memcpy(scrdesc.pszClass, ffdata.name, namelen);
					m_ScriptsArray.append(scrdesc);
				}
			}
#endif
		}
		while (_findnext(hFFile, &ffdata) != -1);
		}
		catch (exception& err)
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
	catch (exception& err)
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
int __declspec(dllexport) __stdcall
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
		return 0;

	bool bEditor = false;
	IUnknown* pIFace;
	if (SUCCEEDED(pScriptMan->QueryInterface(IID_IEditTools, reinterpret_cast<void**>(&pIFace))))
	{
		bEditor = true;
		pIFace->Release();
	}
	if (!g_ScriptModule.Init(pszName, bEditor))
		return 0;
	g_ScriptModule.QueryInterface(IID_IScriptModule, reinterpret_cast<void**>(pOutInterface));

	return 1;
}

extern "C"
BOOL WINAPI
DllMain (HINSTANCE hDLL, DWORD dwReason, PVOID lpResv)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hDLL);
		return TRUE;
	}
	return TRUE;
#ifdef __GNUC__
	lpResv = lpResv;
#endif
#ifdef __BORLANDC__
#pragma argsused(hDLL,dwReason)
#endif
}
