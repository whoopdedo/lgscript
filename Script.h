/******************************************************************************
 *    Script.h
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

#ifndef SCRIPT_H
#define SCRIPT_H

#include <lg/objstd.h>
#include <lg/script.h>
#include <lg/scrmsgs.h>

class cScript : public IScript
{
public:
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID,void**);
	STDMETHOD_(ULONG,AddRef)(void);
	STDMETHOD_(ULONG,Release)(void);
	// IScript
	STDMETHOD_(const char*,GetClassName)(void);
	STDMETHOD_(long,ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);

private:
	int m_iRef;
	char* m_szName;
	int m_iObjId;

public:
	virtual ~cScript();
	cScript(const char* pszName, int iHostObjId);

protected:
	const char* Name() const { return m_szName; };
	int ObjId() const { return m_iObjId; };

};

#endif // SCRIPT_H
