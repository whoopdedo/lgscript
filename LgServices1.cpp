/******************************************************************************
 *  LgServices1.cpp
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
#undef _DARKGAME
#define _DARKGAME 1
#define LUAX_INLINE
#include "luax.h"

#include "LgServices.h"
#include "ScriptModule.h"
#include "LgMultiParm.h"
#include "mod/modlib.h"

namespace Lgs
{

const luax::Registry ActReact1Service::Methods[] = {
	{"React",React},
	{"Stimulate",Stimulate},
	{"GetReactionNamed",GetReactionNamed},
	{"GetReactionName",GetReactionName},
	{"SubscribeToStimulus",SubscribeToStimulus},
	{"UnsubscribeToStimulus",UnsubscribeToStimulus},
	{"BeginContact",BeginContact},
	{"EndContact",EndContact},
	{"SetSingleSensorContact",SetSingleSensorContact},
	{"GetStimulusName",GetStimulusName},
	{"GetStimulusNamed",GetStimulusNamed},
	{"IsStimulus",IsStimulus},
	{NULL,NULL}
};

int ActReact1Service::Stimulate(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1); // destination
	object arg2 = S.checkInteger(2); // stimulus
	float arg3 = S.checkNumber(3);  // intensity
	long ret = ActReactSrv->Stimulate(arg1,arg2,arg3);
	S.push(ret == 0);
	return 1;
}

const luax::Registry Object1Service::Methods[] = {
	{"BeginCreate",BeginCreate},
	{"EndCreate",EndCreate},
	{"Create",Create},
	{"Destroy",Destroy},
	{"Exists",Exists},
	{"SetName",SetName},
	{"GetName",GetName},
	{"Named",Named},
	{"AddMetaProperty",AddMetaProperty},
	{"RemoveMetaProperty",RemoveMetaProperty},
	{"HasMetaProperty",HasMetaProperty},
	{"InheritsFrom",InheritsFrom},
	{"IsTransient",IsTransient},
	{"SetTransience",SetTransience},
	{"Position",Position},
	{"Facing",Facing},
	{"Teleport",Teleport},
	{"FindClosestObjectNamed",FindClosestObjectNamed},
	{"AddMetaPropertyToMany",AddMetaPropertyToMany},
	{"RemoveMetaPropertyFromMany",RemoveMetaPropertyFromMany},
	{"RenderedThisFrame",RenderedThisFrame},
	{"GetArchetype",GetArchetype},
	{"IsArchetype",IsArchetype},
	{"IsMetaProperty",IsMetaProperty},
	{"IsDonor",IsDonor},
	{NULL,NULL}
};
int Object1Service::FindClosestObjectNamed(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	ObjectSys.set(g_pScriptManager);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	object ret;
	cScrVec pos;
	ObjectSrv->Position(pos,arg1);
	int arc = ObjectSys->GetObjectNamed(arg2);
	if (arc != 0)
	{
		cScrVec other;
		// FIXME
		double distance = double(LONG_MAX)+1;
		SInterface<IObjectQuery> q(TraitMan->Query(arc,kTraitQueryChildren|kTraitQueryFull));
		for (; !q->Done(); q->Next())
		{
			object o = q->Object();
			if (static_cast<int>(o) > 0)
			{
				ObjectSrv->Position(other,o);
				// can't check for out-of-world in D1, hope for the best
				double d = pos.Distance(other);
				if (d < distance)
					ret = o;
			}
		}
	}
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int Object1Service::AddMetaPropertyToMany(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	int ret = ObjectSrv->AddMetaPropertyToMany(arg1,arg2);
	S.push(ret);
	return 1;
}
int Object1Service::RemoveMetaPropertyFromMany(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	int ret = ObjectSrv->RemoveMetaPropertyFromMany(arg1,arg2);
	S.push(ret);
	return 1;
}
int Object1Service::RenderedThisFrame(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	true_bool ret;
	ObjectSrv->RenderedThisFrame(ret,arg1);
	S.push(bool(ret));
	return 1;
}

const luax::Registry Phys1Service::Methods[] = {
	{"SubscribeMsg",SubscribeMsg},
	{"UnsubscribeMsg",UnsubscribeMsg},
	{"LaunchProjectile",LaunchProjectile},
	{"SetVelocity",SetVelocity},
	{"GetVelocity",GetVelocity},
	{"SetGravity",SetGravity},
	{"GetGravity",GetGravity},
	{NULL,NULL}
};
int Phys1Service::SetGravity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	float arg2 = S.checkNumber(2);
	PhysSrv->SetGravity(arg1,arg2);
	return 0;
}
int Phys1Service::GetGravity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	float ret = PhysSrv->GetGravity(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry PropertyService::Methods[] = {
	{"Get",Get},
	{"Set",Set},
	{"Add",Add},
	{"Remove",Remove},
	{"CopyFrom",CopyFrom},
	{"Possessed",Possessed},
	{NULL,NULL}
};
int PropertyService::Get(luax::Handle L)
{
	luax::State S(L);
	PropertySrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	const char * arg3 = S.optString(3,NULL,NULL);
	cMultiParm ret;
	PropertySrv->Get(ret,arg1,arg2,arg3);
	ScriptMultiParm(S).push(ret);
	return 1;
}
int PropertyService::Set(luax::Handle L)
{
	luax::State S(L);
	PropertySrv.set(g_pScriptManager);
	long ret;
	object arg1 = S.checkInteger(1);
	if (S.getTop() >= 4)
	{
		const char * arg2 = S.checkString(2,NULL);
		const char * arg3 = S.checkString(3,NULL);
		ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
		ret = PropertySrv->Set(arg1,arg2,arg3,arg4);
	}
	else
	{
		const char * arg2 = S.checkString(2,NULL);
		S.checkAny(3);
		ScriptMultiParm mp3(S); cMultiParm const& arg3 = mp3.pop(3);
		ret = PropertySrv->Set(arg1,arg2,NULL,arg3);
	}
	S.push(ret == 0);
	return 1;
}

const luax::Registry Sound1Service::Methods[] = {
	{"Play",Play},
	{"PlayAmbient",PlayAmbient},
	{"PlaySchema",PlaySchema},
	{"PlaySchemaAmbient",PlaySchemaAmbient},
	{"PlayEnvSchema",PlayEnvSchema},
	{"PlayVoiceOver",PlayVoiceOver},
	{"Halt",Halt},
	{"HaltSchema",HaltSchema},
	{"HaltSpeech",HaltSpeech},
	{"PreLoad",PreLoad},
	{NULL,NULL}
};
int Sound1Service::Play(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	true_bool ret;
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	if (!(S.isNoneOrNil(3) || S.isBoolean(3)))
	{
		eSoundSpecial arg4 = S.toBoolean(4) ? kSoundLoop : kSoundNormal;
		if (S.isNumber(3))
		{
			object arg3 = S.toInteger(3);
			SoundSrv->PlayAtObject(ret,arg1,arg2,arg3,arg4);
		}
		else
		{
			cScrVec arg3;
			const lvector* v3 = reinterpret_cast<const lvector*>(
						S.checkUserdata(3,LVECTOR_NAME));
			arg3.x = v3->v.x; arg3.y = v3->v.y; arg3.z = v3->v.z;
			SoundSrv->PlayAtLocation(ret,arg1,arg2,arg3,arg4);
		}
	}
	else
	{
		eSoundSpecial arg3 = S.toBoolean(3) ? kSoundLoop : kSoundNormal;
		SoundSrv->Play(ret,arg1,arg2,arg3);
	}
	S.push(bool(ret));
	return 1;
}
int Sound1Service::PlayAmbient(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	eSoundSpecial arg3 = S.toBoolean(3) ? kSoundLoop : kSoundNormal;
	true_bool ret;
	SoundSrv->PlayAmbient(ret,arg1,arg2,arg3);
	S.push(bool(ret));
	return 1;
}
int Sound1Service::PlaySchema(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	true_bool ret;
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	if (!(S.isNoneOrNil(3) || S.isBoolean(3)))
	{
		if (S.isNumber(3))
		{
			object arg3 = S.toInteger(3);
			SoundSrv->PlaySchemaAtObject(ret,arg1,arg2,arg3);
		}
		else
		{
			cScrVec arg3;
			const lvector* v3 = reinterpret_cast<const lvector*>(
						S.checkUserdata(3,LVECTOR_NAME));
			arg3.x = v3->v.x; arg3.y = v3->v.y; arg3.z = v3->v.z;
			SoundSrv->PlaySchemaAtLocation(ret,arg1,arg2,arg3);
		}
	}
	else
	{
		SoundSrv->PlaySchema(ret,arg1,arg2);
	}
	S.push(bool(ret));
	return 1;
}
int Sound1Service::PlaySchemaAmbient(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	true_bool ret;
	SoundSrv->PlaySchemaAmbient(ret,arg1,arg2);
	S.push(bool(ret));
	return 1;
}
int Sound1Service::PlayEnvSchema(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	object arg3 = S.optInteger(3);
	object arg4 = S.optInteger(4);
	eEnvSoundLoc arg5 = eEnvSoundLoc(S.checkOption(5,EnvSoundLoc,"onobj"));
	true_bool ret;
	SoundSrv->PlayEnvSchema(ret,arg1,arg2,arg3,arg4,arg5);
	S.push(bool(ret));
	return 1;
}
int Sound1Service::PlayVoiceOver(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	true_bool ret;
	SoundSrv->PlayVoiceOver(ret,arg1,arg2);
	S.push(bool(ret));
	return 1;
}
int Sound1Service::Halt(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.optString(2,NULL,"");
	object arg3 = S.optInteger(3);
	int ret = SoundSrv->Halt(arg1,arg2,arg3);
	S.push(luax::Number(ret));
	return 1;
}
int Sound1Service::HaltSchema(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.optString(2,NULL,"");
	object arg3 = S.optInteger(3);
	true_bool ret;
	SoundSrv->HaltSchema(ret,arg1,arg2,arg3);
	S.push(bool(ret));
	return 1;
}
int Sound1Service::HaltSpeech(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	SoundSrv->HaltSpeech(arg1);
	return 0;
}
int Sound1Service::PreLoad(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	cScrStr arg1 = S.checkString(1,NULL);
	true_bool ret;
	SoundSrv->PreLoad(ret,arg1);
	S.push(bool(ret));
	return 1;
}

}
