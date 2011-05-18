/******************************************************************************
 *  LgMessage.h
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
#ifndef LGMESSAGE_H
#define LGMESSAGE_H

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/scrmsgs.h>

#include "luax.h"

namespace Lgs
{

class ScriptMessage
{
public:
	ScriptMessage(luax::State& L)
		: m_lua(L)
		{ }

	//operator const cScrMsg& ()
	//	{ return pop(); }
	ScriptMessage& operator= (sScrMsg* msg)
		{ push(msg); return *this; }

	void push(sScrMsg* msg);
	//const cScrMsg& pop(void);

	static void Init(luax::State& S);

	static const char s_ClassName[];
	static const char s_HackName[];

	static sScrMsg* Check(luax::State& S, int arg = 1);

private:
	luax::State& m_lua;

	static int Iter(luax::Handle L);
	static int Next(luax::Handle L);
	static int Index(luax::Handle L);
	static int Release(luax::Handle L);
	// construct table from message data fields
	static int getDataFields(luax::Handle L);
	static int getIntegerField(luax::Handle L);
	static int getFloatField(luax::Handle L);
	static int getStringField(luax::Handle L);
	static int getBooleanField(luax::Handle L);
	static int getVectorField(luax::Handle L);
	static int getMultiParmField(luax::Handle L);
	static int getConstantField(luax::Handle L);
	static int getEnumeratedField(luax::Handle L);

	struct MetafieldDef
	{
		const char* name;
		luax::CFunction func;
		size_t nup;
		const void* sup;
	};
	struct MetatableDef
	{
		const char* name;
		const char* message;
		const MetafieldDef* fields;
	};

	static void Metafields(luax::State& S, const MetafieldDef* F);
	static void Metatable(luax::State& S, int up);

	static const MetatableDef Registry[];
	static const MetafieldDef ScrMsgFields[];
	static const MetafieldDef SimMsgFields[];
	static const MetafieldDef DarkGameModeMsgFields[];
	static const MetafieldDef AIModeChangeMsgFields[];
	static const MetafieldDef AIAlertnessMsgFields[];
	static const MetafieldDef AIResultMsgFields[];
	static const MetafieldDef AIObjActResultMsgFields[];
	static const MetafieldDef AIPatrolPointMsgFields[];
	static const MetafieldDef AISignalMsgFields[];
	static const MetafieldDef AttackMsgFields[];
	static const MetafieldDef CombineScrMsgFields[];
	static const MetafieldDef ContainedScrMsgFields[];
	static const MetafieldDef ContainerScrMsgFields[];
	static const MetafieldDef DamageScrMsgFields[];
	static const MetafieldDef DiffScrMsgFields[];
	static const MetafieldDef DoorMsgFields[];
	static const MetafieldDef FrobMsgFields[];
	static const MetafieldDef BodyMsgFields[];
	static const MetafieldDef PickStateScrMsgFields[];
	static const MetafieldDef PhysMsgFields[];
	static const MetafieldDef RoomMsgFields[];
	static const MetafieldDef SlayMsgFields[];
	static const MetafieldDef SchemaDoneMsgFields[];
	static const MetafieldDef StimMsgFields[];
	static const MetafieldDef ScrTimerMsgFields[];
	static const MetafieldDef TweqMsgFields[];
	static const MetafieldDef WaypointMsgFields[];
	static const MetafieldDef MovingTerrainMsgFields[];
	static const MetafieldDef MediumTransMsgFields[];
	static const MetafieldDef QuestMsgFields[];
	static const MetafieldDef YorNMsgFields[];
	static const MetafieldDef KeypadMsgFields[];

	static const MetafieldDef DarkHookNullFields[];
	static const MetafieldDef DarkHookPropFields[];
	static const MetafieldDef DarkHookRelFields[];
	static const MetafieldDef DarkHookObjFields[];
	static const MetafieldDef DarkHookTraitFields[];

	static const char* const DarkHookEvents[];
	static const char* const DarkHookObjEvents[];
	static const char* const DarkHookTraitEvents[];
};

} // namespace Lgs

#endif // LGMESSAGE_H
