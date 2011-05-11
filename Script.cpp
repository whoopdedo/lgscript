/******************************************************************************
 *  Script.cpp
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
#include "Script.h"
#include "ScriptModule.h"

#include <cstring>


cScript::~cScript()
{
	delete[] m_szName;
}

cScript::cScript(const char* pszName, int iHostObjId)
	    : m_iObjId(iHostObjId)
{
	if (pszName == NULL)
		pszName = "cScript";
	m_szName = new char[strlen(pszName)+1];
	strcpy(m_szName, pszName);
}

STDMETHODIMP_(const char*) cScript::GetClassName(void)
{
	return Name();
}

STDMETHODIMP cScript::ReceiveMessage(sScrMsg* pMsg, sMultiParm*, eScrTraceAction eTrace)
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
