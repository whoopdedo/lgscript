/******************************************************************************
 *    LgMessage.cpp
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
#define LUAX_INLINE
#include "luax.h"
#include "LgMessage.h"
#include "LgMultiParm.h"
#include <cstddef>
#include <darkhook.h>

using namespace Lgs;
using namespace luax;
using namespace std;

const char ScriptMessage::s_ClassName[] = "LgMessage";

const ScriptMessage::MetatableDef ScriptMessage::Registry[] = {
	{"sSimMsg", SimMsgFields},
	{"sDarkGameModeScrMsg", DarkGameModeMsgFields},
	{"sAIModeChangeMsg", AIModeChangeMsgFields},
	{"sAIAlertnessMsg", AIAlertnessMsgFields},
	{"sAIHighAlertMsg", AIAlertnessMsgFields},
	{"sAIResultMsg", AIResultMsgFields},
	{"sAIObjActResultMsg", AIObjActResultMsgFields},
	{"sAIPatrolPointMsg", AIPatrolPointMsgFields},
	{"sAISignalMsg", AISignalMsgFields},
	{"sAttackMsg", AttackMsgFields},
	{"sCombineScrMsg", CombineScrMsgFields},
	{"sContainedScrMsg", ContainedScrMsgFields},
	{"sContainerScrMsg", ContainerScrMsgFields},
	{"sDamageScrMsg", DamageScrMsgFields},
	{"sDiffScrMsg", DiffScrMsgFields},
	{"sDoorMsg", DoorMsgFields},
	{"sFrobMsg", FrobMsgFields},
	{"sBodyMsg", BodyMsgFields},
	{"sPickStateScrMsg", PickStateScrMsgFields},
	{"sPhysMsg", PhysMsgFields},
	{"sRoomMsg", RoomMsgFields},
	{"sSlayMsg", SlayMsgFields},
	{"sSchemaDoneMsg", SchemaDoneMsgFields},
	{"sSoundDoneMsg", SchemaDoneMsgFields},
	{"sStimMsg", StimMsgFields},
	{"sScrTimerMsg", ScrTimerMsgFields},
	{"sTweqMsg", TweqMsgFields},
	{"sWaypointMsg", WaypointMsgFields},
	{"sMovingTerrainMsg", MovingTerrainMsgFields},
	{"sQuestMsg", QuestMsgFields},
	{"sMediumTransMsg", MediumTransMsgFields},
	{"sYorNMsg", YorNMsgFields},
	{"sKeypadMsg", KeypadMsgFields},
	{"sDHNotifyNullMsg", DarkHookNullFields},
	{"sDHNotifyPropMsg", DarkHookPropFields},
	{"sDHNotifyRelMsg", DarkHookRelFields},
	{"sDHNotifyObjMsg", DarkHookObjFields},
	{"sDHNotifyTraitMsg", DarkHookTraitFields},
	{NULL,NULL}
};

void ScriptMessage::push(sScrMsg* msg)
{
	*reinterpret_cast<sScrMsg**>(
		m_lua.newUserdata(sizeof(sScrMsg*))
		) = msg;
	int iMsg = m_lua.getTop();
	const char* pszClass = "";
#ifdef _MSC_VER
	if (dynamic_cast<sPersistent*>(msg) != NULL)
		pszClass = msg->GetName();
#else
	// GCC & Borland need to use an assembly kludge, so we
	// check for a valid vtable there.
	pszClass = msg->Persistent_GetName();
#endif
	if (!pszClass)
		pszClass = "";
	m_lua.getField(s_ClassName, LUA_REGISTRYINDEX);
	if (m_lua.getField(pszClass).isNil())
	{
		if (0 == _stricmp(pszClass, "sDHNotifyMsg"))
		{
			m_lua.pop();
			switch (static_cast<sDHNotifyMsg*>(msg)->typeDH)
			{
			case kDH_Property:
				m_lua.getField("sDHNotifyPropMsg");
				break;
			case kDH_Relation:
				m_lua.getField("sDHNotifyRelMsg");
				break;
			case kDH_Object:
				m_lua.getField("sDHNotifyObjMsg");
				break;
			case kDH_Trait:
				m_lua.getField("sDHNotifyTraitMsg");
				break;
			default:
				m_lua.getField("sDHNotifyNullMsg");
				break;
			}
		}
		else
			m_lua.pop().getField("");
	}
	m_lua.setMetatable(iMsg).setTop(iMsg);
	msg->AddRef();
}

void ScriptMessage::Init(State& S)
{
	int iRegistry = S.createTableI(0, (sizeof(Registry)/sizeof(Registry[0]))+1);
	Metafields(S, ScrMsgFields);
	S.copy();
	Metatable(S, 1);
	S.copy().setField("sScrMsg", iRegistry);
	S.setField("", iRegistry);
	for (const MetatableDef* mt = Registry;
		mt->name; mt++)
	{
		S.copy();
		Metafields(S, mt->fields);
		Metatable(S, 2);
		S.setField(mt->name, iRegistry);
	}
	S.pop().setField(s_ClassName, LUA_REGISTRYINDEX);
}

void ScriptMessage::Metafields(State& S, const MetafieldDef* F)
{
	const MetafieldDef* f = F;
	while (f->name) ++f;
	S.createTable(0,f-F);
	for (f = F; f->name; f++)
	{
		if (f->sup)
			S.push(const_cast<void*>(f->sup))
			 .push(Integer(f->nup)).push(f->func, 2);
		else
			S.push(Integer(f->nup)).push(f->func, 1);
		S.setField(f->name);
	}
}

void ScriptMessage::Metatable(State& S, int up)
{
	int iMT = S.getTop() - up + 1;
	S.createTable(0,3).insert(iMT);
	for (int i=0; i<up; i++)
		S.copy(-up);
	S.push(Iter, up).setField("__pairs", iMT);
	S.push(Index, up).setField("__index", iMT);
	S.push(Release).setField("__gc", iMT);
	S_pushLiteral(S, s_ClassName).setField("class", iMT);
	S.setTop(iMT);
}

int ScriptMessage::Iter(Handle L)
{
	State S(L);
	Check(S, 1);
	int up = 1;
	S.copy(Upvalue(1));
	if (!S.isNone(Upvalue(2)))
	{
		S.copy(Upvalue(2));
		up++;
	}
	S.push(Next, up).copy(1).push(Nil());
	return 3;
}

int ScriptMessage::Next(Handle L)
{
	State S(L);
	S.setTop(2);
	int up = Upvalue(1);
	if (!S.isNil(2))
	{
		if (S.rawGet(up, 2).isNil()
		 && !S.isNone(Upvalue(2)))
		{
			up = Upvalue(2);
		}
		S.pop();
	}
	if (!S.next(up))
	{
		if (!(up == Upvalue(1) && !S.isNone(Upvalue(2))
		    && S.push(Nil()).next(Upvalue(2))))
		{
			S.push(Nil());
			return 1;
		}
	}
	S.copy(1).call(1,1);
	return 2;

}

int ScriptMessage::Index(Handle L)
{
	State S(L);
	S.checkString(2, NULL);
	if (!S.rawGet(Upvalue(1), 2).isNil())
	{
		S.copy(1).call(1,1);
		return 1;
	}
	if (!S.isNone(Upvalue(2)))
	{
		S.pop();
		if (!S.rawGet(Upvalue(2), 2).isNil())
		{
			S.copy(1).call(1,1);
			return 1;
		}
	}
	return 1;
}

int ScriptMessage::Release(Handle L)
{
	State S(L);
	sScrMsg** msg = S.toUserdata(Userdata<sScrMsg*>(), 1);
	if (*msg)
	{
		(*msg)->Release();
		*msg = NULL;
	}
	return 1;
}

sScrMsg* ScriptMessage::Check(State& S, int arg)
{
	void* pv = S.toUserdata(arg);
	if (pv != NULL && *reinterpret_cast<sScrMsg**>(pv) != NULL
	 && !S.getMetatable(arg).isNil())
	{
		if (!S.getField("class").isNil()
		 && (0 == strcmp(S.asString(), s_ClassName)))
		{
			S.pop(2);
			return *reinterpret_cast<sScrMsg**>(pv);
		}
	}
	S.typeError(arg, s_ClassName);
	return NULL;
}

int ScriptMessage::getDataFields(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	ScriptMultiParm mp(S);
	S.createTable(3);
	mp.push(pMsg->data);
	S.rawSetN(1);
	mp.push(pMsg->data2);
	S.rawSetN(2);
	mp.push(pMsg->data3);
	S.rawSetN(3);
	return 1;
}

#define pointerto(P,N)	(reinterpret_cast<char*>(P)+(N))
#define valueat(T,P,N)	(*reinterpret_cast<T*>(pointerto(P,N)))

int ScriptMessage::getIntegerField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	int iVal = valueat(int,pMsg,iOffset);
	S.push(iVal);
	return 1;
}

int ScriptMessage::getFloatField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	float fVal = valueat(float,pMsg,iOffset);
	S.push(fVal);
	return 1;
}

int ScriptMessage::getStringField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	const char* pszVal = valueat(const char*,pMsg,iOffset);
	S.push(pszVal);
	return 1;
}

int ScriptMessage::getBooleanField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	bool bVal = bool(valueat(BOOL,pMsg,iOffset));
	S.push(bVal);
	return 1;
}

int ScriptMessage::getVectorField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	mxs_vector& vVal = valueat(mxs_vector,pMsg,iOffset);
	S.getGlobal("vector")
	 .push(vVal.x).push(vVal.y).push(vVal.z)
	 .call(3,1);
	return 1;
}

int ScriptMessage::getMultiParmField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	cMultiParm& mp = valueat(cMultiParm,pMsg,iOffset);
	ScriptMultiParm(S).push(mp);
	return 1;
}

int ScriptMessage::getConstantField(Handle L)
{
	State S(L);
	const char* s = reinterpret_cast<const char*>(S.toUserdata(Upvalue(2)));
	S.push(s);
	return 1;
}

int ScriptMessage::getEnumeratedField(Handle L)
{
	State S(L);
	sScrMsg* pMsg = Check(S);
	int iOffset = S.toInteger(Upvalue(1));
	const char* const* s = reinterpret_cast<const char* const*>(S.toUserdata(Upvalue(2)));
	int i = valueat(int,pMsg,iOffset);
	if (0 <= i)
	{
		int n = 0;
		for (; s[n]; n++)
			if (n == i)
				break;
		if (s[n])
			S.push(s[n]);
		else
			S.push(i);
	}
	else
		S.push(i);
	return 1;
}



#define getObjectField getIntegerField

const ScriptMessage::MetafieldDef ScriptMessage::ScrMsgFields[] = {
	{"from", getObjectField, offsetof(sScrMsg,from), NULL},
	{"to", getObjectField, offsetof(sScrMsg,to), NULL},
	{"message", getStringField, offsetof(sScrMsg,message), NULL},
	{"time", getIntegerField, offsetof(sScrMsg,time), NULL},
	{"flags", getIntegerField, offsetof(sScrMsg,flags), NULL},
	{"data", getDataFields, 0, NULL},
	{NULL,NULL,0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::SimMsgFields[] = {
	{"fStarting", getBooleanField, offsetof(sSimMsg,fStarting), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::DarkGameModeMsgFields[] = {
	{"fEntering", getBooleanField, offsetof(sDarkGameModeScrMsg,fEntering), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AIModeChangeMsgFields[] = {
	{"mode", getIntegerField, offsetof(sAIModeChangeMsg,mode), NULL},
	{"previous_mode", getIntegerField, offsetof(sAIModeChangeMsg,previous_mode), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AIAlertnessMsgFields[] = {
	{"level", getIntegerField, offsetof(sAIAlertnessMsg,level), NULL},
	{"oldLevel", getIntegerField, offsetof(sAIAlertnessMsg,oldLevel), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AIResultMsgFields[] = {
	{"action", getIntegerField, offsetof(sAIResultMsg,action), NULL},
	{"result", getIntegerField, offsetof(sAIResultMsg,result), NULL},
	{"result_data", getMultiParmField, offsetof(sAIResultMsg,result_data), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AIObjActResultMsgFields[] = {
	{"action", getIntegerField, offsetof(sAIObjActResultMsg,action), NULL},
	{"result", getIntegerField, offsetof(sAIObjActResultMsg,result), NULL},
	{"result_data", getMultiParmField, offsetof(sAIObjActResultMsg,result_data), NULL},
	{"target", getObjectField, offsetof(sAIObjActResultMsg,target), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AIPatrolPointMsgFields[] = {
	{"patrolObj", getObjectField, offsetof(sAIPatrolPointMsg,patrolObj), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AISignalMsgFields[] = {
	{"signal", getStringField, offsetof(sAISignalMsg,signal), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::AttackMsgFields[] = {
	{"weapon", getObjectField, offsetof(sAttackMsg,weapon), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::CombineScrMsgFields[] = {
	{"combiner", getObjectField, offsetof(sCombineScrMsg,combiner), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::ContainedScrMsgFields[] = {
	{"event", getIntegerField, offsetof(sContainedScrMsg,event), NULL},
	{"container", getObjectField, offsetof(sContainedScrMsg,container), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::ContainerScrMsgFields[] = {
	{"event", getIntegerField, offsetof(sContainerScrMsg,event), NULL},
	{"containee", getObjectField, offsetof(sContainerScrMsg,containee), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::DamageScrMsgFields[] = {
	{"kind", getObjectField, offsetof(sDamageScrMsg,kind), NULL},
	{"damage", getIntegerField, offsetof(sDamageScrMsg,damage), NULL},
	{"culprit", getObjectField, offsetof(sDamageScrMsg,culprit), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::DiffScrMsgFields[] = {
	{"difficulty", getIntegerField, offsetof(sDiffScrMsg,difficulty), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::DoorMsgFields[] = {
	{"ActionType", getIntegerField, offsetof(sDoorMsg,ActionType), NULL},
	{"PrevActionType", getIntegerField, offsetof(sDoorMsg,PrevActionType), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::FrobMsgFields[] = {
	{"SrcObjId", getObjectField, offsetof(sFrobMsg,SrcObjId), NULL},
	{"DstObjId", getObjectField, offsetof(sFrobMsg,DstObjId), NULL},
	{"Frobber", getObjectField, offsetof(sFrobMsg,Frobber), NULL},
	{"SrcLoc", getIntegerField, offsetof(sFrobMsg,SrcLoc), NULL},
	{"DstLoc", getIntegerField, offsetof(sFrobMsg,DstLoc), NULL},
	{"Sec", getFloatField, offsetof(sFrobMsg,Sec), NULL},
	{"Abort", getBooleanField, offsetof(sFrobMsg,Abort), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::BodyMsgFields[] = {
	{"ActionType", getIntegerField, offsetof(sBodyMsg,ActionType), NULL},
	{"MotionName", getStringField, offsetof(sBodyMsg,MotionName), NULL},
	{"FlagValue", getIntegerField, offsetof(sBodyMsg,FlagValue), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::PickStateScrMsgFields[] = {
	{"PrevState", getIntegerField, offsetof(sPickStateScrMsg,PrevState), NULL},
	{"NewState", getIntegerField, offsetof(sPickStateScrMsg,NewState), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::PhysMsgFields[] = {
	{"Submod", getIntegerField, offsetof(sPhysMsg,Submod), NULL},
	{"collType", getIntegerField, offsetof(sPhysMsg,collType), NULL},
	{"collObj", getObjectField, offsetof(sPhysMsg,collObj), NULL},
	{"collSubmod", getIntegerField, offsetof(sPhysMsg,collSubmod), NULL},
	{"collMomentum", getFloatField, offsetof(sPhysMsg,collMomentum), NULL},
	{"collNormal", getVectorField, offsetof(sPhysMsg,collNormal), NULL},
	{"collPt", getVectorField, offsetof(sPhysMsg,collPt), NULL},
	{"contactType", getIntegerField, offsetof(sPhysMsg,contactType), NULL},
	{"contactObj", getObjectField, offsetof(sPhysMsg,contactObj), NULL},
	{"contactSubmod", getIntegerField, offsetof(sPhysMsg,contactSubmod), NULL},
	{"transObj", getObjectField, offsetof(sPhysMsg,transObj), NULL},
	{"transSubmod", getIntegerField, offsetof(sPhysMsg,transSubmod), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::RoomMsgFields[] = {
	{"FromObjId", getObjectField, offsetof(sRoomMsg,FromObjId), NULL},
	{"ToObjId", getObjectField, offsetof(sRoomMsg,ToObjId), NULL},
	{"MoveObjId", getObjectField, offsetof(sRoomMsg,MoveObjId), NULL},
	{"ObjType", getIntegerField, offsetof(sRoomMsg,ObjType), NULL},
	{"TransitionType", getIntegerField, offsetof(sRoomMsg,TransitionType), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::SlayMsgFields[] = {
	{"culprit", getObjectField, offsetof(sSlayMsg,culprit), NULL},
	{"kind", getObjectField, offsetof(sSlayMsg,kind), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::SchemaDoneMsgFields[] = {
	{"coordinates", getVectorField, offsetof(sSchemaDoneMsg,coordinates), NULL},
	{"targetObject", getObjectField, offsetof(sSchemaDoneMsg,targetObject), NULL},
	{"name", getStringField, offsetof(sSchemaDoneMsg,name), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::StimMsgFields[] = {
	{"stimulus", getObjectField, offsetof(sStimMsg,stimulus), NULL},
	{"intensity", getFloatField, offsetof(sStimMsg,intensity), NULL},
	{"sensor", getIntegerField, offsetof(sStimMsg,sensor), NULL},
	{"source", getIntegerField, offsetof(sStimMsg,source), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::ScrTimerMsgFields[] = {
	{"name", getStringField, offsetof(sScrTimerMsg,name), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::TweqMsgFields[] = {
	{"Type", getIntegerField, offsetof(sTweqMsg,Type), NULL},
	{"Op", getIntegerField, offsetof(sTweqMsg,Op), NULL},
	{"Dir", getIntegerField, offsetof(sTweqMsg,Dir), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::WaypointMsgFields[] = {
	{"moving_terrain", getObjectField, offsetof(sWaypointMsg,moving_terrain), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::MovingTerrainMsgFields[] = {
	{"waypoint", getObjectField, offsetof(sMovingTerrainMsg,waypoint), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::MediumTransMsgFields[] = {
	{"nFromType", getIntegerField, offsetof(sMediumTransMsg,nFromType), NULL},
	{"nToType", getIntegerField, offsetof(sMediumTransMsg,nToType), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::QuestMsgFields[] = {
	{"m_pName", getStringField, offsetof(sQuestMsg,m_pName), NULL},
	{"m_oldValue", getIntegerField, offsetof(sQuestMsg,m_oldValue), NULL},
	{"m_newValue", getIntegerField, offsetof(sQuestMsg,m_newValue), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::YorNMsgFields[] = {
	{"YorN", getBooleanField, offsetof(sYorNMsg,YorN), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::KeypadMsgFields[] = {
	{"code", getIntegerField, offsetof(sKeypadMsg,code), NULL},
	{NULL, NULL, 0, NULL}
};

const ScriptMessage::MetafieldDef ScriptMessage::DarkHookNullFields[] = {
	{"typeDH", getConstantField, 0, "Null"},
	{NULL, NULL, 0, NULL}
};
const char* const ScriptMessage::DarkHookEvents[] = {
	"Null","Change","Add","Remove",NULL
};
const ScriptMessage::MetafieldDef ScriptMessage::DarkHookPropFields[] = {
	{"typeDH", getConstantField, 0, "Property"},
	{"event", getEnumeratedField, offsetof(sDarkPropMsg,event)+sizeof(eDarkHookType), DarkHookEvents},
	{"PropName", getStringField, offsetof(sDarkPropMsg,pszPropName)+sizeof(eDarkHookType), NULL},
	{"idObj", getIntegerField, offsetof(sDarkPropMsg,idObj)+sizeof(eDarkHookType), NULL},
	{NULL, NULL, 0, NULL}
};
const ScriptMessage::MetafieldDef ScriptMessage::DarkHookRelFields[] = {
	{"typeDH", getConstantField, 0, "Relation"},
	{"event", getEnumeratedField, offsetof(sDarkRelMsg,event)+sizeof(eDarkHookType), DarkHookEvents},
	{"RelName", getStringField, offsetof(sDarkRelMsg,pszRelName)+sizeof(eDarkHookType), NULL},
	{"LinkId", getIntegerField, offsetof(sDarkRelMsg,lLinkId)+sizeof(eDarkHookType), NULL},
	{"LinkSource", getIntegerField, offsetof(sDarkRelMsg,iLinkSource)+sizeof(eDarkHookType), NULL},
	{"LinkDest", getIntegerField, offsetof(sDarkRelMsg,iLinkDest)+sizeof(eDarkHookType), NULL},
	{NULL, NULL, 0, NULL}
};
const char* const ScriptMessage::DarkHookObjEvents[] = {
	"Create","Destroy",NULL
};
const ScriptMessage::MetafieldDef ScriptMessage::DarkHookObjFields[] = {
	{"typeDH", getConstantField, 0, "Object"},
	{"event", getEnumeratedField, offsetof(sDarkObjMsg,event)+sizeof(eDarkHookType), DarkHookObjEvents},
	{"idObj", getIntegerField, offsetof(sDarkObjMsg,idObj)+sizeof(eDarkHookType), NULL},
	{NULL, NULL, 0, NULL}
};
const char* const ScriptMessage::DarkHookTraitEvents[] = {
	"AddDonor","DelDonor","AddChild","DelChild",NULL
};
const ScriptMessage::MetafieldDef ScriptMessage::DarkHookTraitFields[] = {
	{"typeDH", getConstantField, 0, "Trait"},
	{"event", getEnumeratedField, offsetof(sDarkTraitMsg,event)+sizeof(eDarkHookType), DarkHookTraitEvents},
	{"idObj", getIntegerField, offsetof(sDarkTraitMsg,idObj)+sizeof(eDarkHookType), NULL},
	{"idSubj", getIntegerField, offsetof(sDarkTraitMsg,idSubj)+sizeof(eDarkHookType), NULL},
	{NULL, NULL, 0, NULL}
};

