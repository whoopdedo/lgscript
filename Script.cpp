/******************************************************************************
 *    Script.cpp
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
#include "Script.h"
#include "ScriptModule.h"

#include <cstring>


cScript::~cScript()
{
	delete[] m_szName;
}

cScript::cScript(const char* pszName, int iHostObjId)
	    : m_iRef(1), m_iObjId(iHostObjId)
{
	if (pszName == NULL)
		pszName = "cScript";
	m_szName = new char[strlen(pszName)+1];
	strcpy(m_szName, pszName);
}

HRESULT __stdcall cScript::QueryInterface(REFIID riid, void** ppout)
{
	if (riid == IID_IUnknown)
		*ppout = static_cast<IUnknown*>(this);
	else if (riid == IID_IScript)
		*ppout = static_cast<IScript*>(this);
	else
		return E_NOINTERFACE;
	static_cast<IUnknown*>(*ppout)->AddRef();
	return S_OK;
}

ULONG __stdcall cScript::AddRef()
{
	return ++m_iRef;
}

ULONG __stdcall cScript::Release()
{
	int iRefCnt = --m_iRef;
	if (iRefCnt == 0)
		delete this;
	return iRefCnt;
}

const char* __stdcall cScript::GetClassName(void)
{
	// Name MUST match the one in the list.
	// Still, we don't want to toss a NULL pointer around, do we?
	return Name();
}

long __stdcall cScript::ReceiveMessage(sScrMsg* pMsg, sMultiParm*, eScrTraceAction eTrace)
{
	long iRet = 0;
	switch (eTrace)
	{
	case kSpew:
	{
		g_pfnMPrintf("[%s:%d] Got message \"%s\" at %d\n",
			m_szName, m_iObjId, pMsg->message, pMsg->time);
		break;
	}
	case kBreak:
	{
		g_pfnMPrintf("[%s:%d] Break on message \"%s\" at %d\n",
			m_szName, m_iObjId, pMsg->message, pMsg->time);
		__asm("int 3");
		break;
	}
	default:
		break;
	}

	return iRet;
}
