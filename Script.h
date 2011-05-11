/******************************************************************************
 *  Script.h
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

#ifndef SCRIPT_H
#define SCRIPT_H

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/interfaceimp.h>
#include <lg/script.h>
#include <lg/scrmsgs.h>

class cScript : public cInterfaceImp<IScript>
{
public:
	// IUnknown
	//STDMETHOD(QueryInterface)(REFIID,void**);
	//STDMETHOD_(ulong,AddRef)(void);
	//STDMETHOD_(ulong,Release)(void);
	// IScript
	STDMETHOD_(const char*,GetClassName)(void);
	STDMETHOD(ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);

private:
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
