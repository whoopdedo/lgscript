/******************************************************************************
 *  LgServices2.cpp
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
#define _DARKGAME 2
#define _NETWORKING 1
#define LUAX_INLINE
#include "luax.h"

#include "LgServices.h"
#include "ScriptModule.h"
#include "LgMultiParm.h"
#include "mod/modlib.h"

#include <lg/scrservices.h>

namespace Lgs
{

const luax::Registry ActReact2Service::Methods[] = {
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

int ActReact2Service::Stimulate(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1); // destination
	object arg2 = S.checkInteger(2); // stimulus
	float arg3 = S.checkNumber(3);  // intensity
	object arg4 = S.optInteger(4); // source
	long ret = ActReactSrv->Stimulate(arg1,arg2,arg3,arg4);
	S.push(ret == 0);
	return 1;
}

const luax::Registry CameraService::Methods[] = {
	{"StaticAttach",StaticAttach},
	{"DynamicAttach",DynamicAttach},
	{"CameraReturn",CameraReturn},
	{"ForceCameraReturn",ForceCameraReturn},
	{NULL,NULL}
};
int CameraService::StaticAttach(luax::Handle L)
{
	luax::State S(L);
	CameraSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	CameraSrv->StaticAttach(arg1);
	return 0;
}
int CameraService::DynamicAttach(luax::Handle L)
{
	luax::State S(L);
	CameraSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	CameraSrv->DynamicAttach(arg1);
	return 0;
}
int CameraService::CameraReturn(luax::Handle L)
{
	luax::State S(L);
	CameraSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = CameraSrv->CameraReturn(arg1);
	S.push(ret == 0);
	return 1;
}
int CameraService::ForceCameraReturn(luax::Handle L)
{
	luax::State S(L);
	CameraSrv.set(g_pScriptManager);
	CameraSrv->ForceCameraReturn();
	return 0;
}

const luax::Registry DarkGame2Service::Methods[] = {
	{"KillPlayer",KillPlayer},
	{"EndMission",EndMission},
	{"FadeToBlack",FadeToBlack},
	{"FoundObject",FoundObject},
	{"ConfigIsDefined",ConfigIsDefined},
	{"ConfigGetInt",ConfigGetInt},
	{"ConfigGetFloat",ConfigGetFloat},
	{"BindingGetFloat",BindingGetFloat},
	{"GetAutomapLocationVisited",GetAutomapLocationVisited},
	{"SetAutomapLocationVisited",SetAutomapLocationVisited},
	{NULL,NULL}
};
int DarkGame2Service::FoundObject(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	DarkGameSrv->FoundObject(arg1);
	return 0;
}
int DarkGame2Service::ConfigIsDefined(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	bool ret = DarkGameSrv->ConfigIsDefined(arg1);
	S.push(ret);
	return 1;
}
int DarkGame2Service::ConfigGetInt(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int ret;
	if (DarkGameSrv->ConfigGetInt(arg1,ret))
		S.push(ret);
	else
		S.push(luax::Nil());
	return 1;
}
int DarkGame2Service::ConfigGetFloat(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	float ret;
	if (DarkGameSrv->ConfigGetFloat(arg1,ret))
		S.push(ret);
	else
		S.push(luax::Nil());
	return 1;
}
int DarkGame2Service::BindingGetFloat(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	float ret = DarkGameSrv->BindingGetFloat(arg1);
	S.push(ret);
	return 1;
}
int DarkGame2Service::GetAutomapLocationVisited(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	bool ret = DarkGameSrv->GetAutomapLocationVisited(arg1,arg2);
	S.push(ret);
	return 1;
}
int DarkGame2Service::SetAutomapLocationVisited(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	DarkGameSrv->SetAutomapLocationVisited(arg1,arg2);
	return 0;
}

const luax::Registry Door2Service::Methods[] = {
	{"CloseDoor",CloseDoor},
	{"OpenDoor",OpenDoor},
	{"GetDoorState",GetDoorState},
	{"ToggleDoor",ToggleDoor},
	{"SetBlocking",SetBlocking},
	{"GetSoundBlocking",GetSoundBlocking},
	{NULL,NULL}
};
int Door2Service::SetBlocking(luax::Handle L)
{
	luax::State S(L);
	DoorSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	S.checkAny(2);
	bool arg2 = S.toBoolean(2);
	bool ret = DoorSrv->SetBlocking(arg1,arg2);
	S.push(ret);
	return 1;
}
int Door2Service::GetSoundBlocking(luax::Handle L)
{
	luax::State S(L);
	DoorSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = DoorSrv->GetSoundBlocking(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry Networking2Service::Methods[] = {
	{"Broadcast",Broadcast},
	{"SendToProxy",SendToProxy},
	{"TakeOver",TakeOver},
	{"GiveTo",GiveTo},
	{"IsPlayer",IsPlayer},
	{"IsMultiplayer",IsMultiplayer},
	{"SetProxyOneShotTimer",SetProxyOneShotTimer},
	{"FirstPlayer",FirstPlayer},
	{"NextPlayer",NextPlayer},
	{"Suspend",Suspend},
	{"Resume",Resume},
	{"HostedHere",HostedHere},
	{"IsProxy",IsProxy},
	{"LocalOnly",LocalOnly},
	{"IsNetworking",IsNetworking},
	{"Owner",Owner},
	{NULL,NULL}
};
int Networking2Service::Owner(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object ret;
	NetworkingSrv->Owner(ret,arg1);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}

const luax::Registry Object2Service::Methods[] = {
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
	{"IsPositionValid",IsPositionValid},
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
int Object2Service::IsPositionValid(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	true_bool ret;
	ObjectSrv->IsPositionValid(ret,arg1);
	S.push(bool(ret));
	return 1;
}
int Object2Service::FindClosestObjectNamed(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	object ret;
	ObjectSrv->FindClosestObjectNamed(ret,arg1,arg2);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int Object2Service::AddMetaPropertyToMany(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	int ret = ObjectSrv->AddMetaPropertyToMany(arg1,arg2);
	S.push(ret);
	return 1;
}
int Object2Service::RemoveMetaPropertyFromMany(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	int ret = ObjectSrv->RemoveMetaPropertyFromMany(arg1,arg2);
	S.push(ret);
	return 1;
}
int Object2Service::RenderedThisFrame(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	true_bool ret;
	ObjectSrv->RenderedThisFrame(ret,arg1);
	S.push(bool(ret));
	return 1;
}

const luax::Registry Phys2Service::Methods[] = {
	{"SubscribeMsg",SubscribeMsg},
	{"UnsubscribeMsg",UnsubscribeMsg},
	{"LaunchProjectile",LaunchProjectile},
	{"SetVelocity",SetVelocity},
	{"GetVelocity",GetVelocity},
	{"ControlVelocity",ControlVelocity},
	{"StopControlVelocity",StopControlVelocity},
	{"SetGravity",SetGravity},
	{"GetGravity",GetGravity},
	{"HasPhysics",HasPhysics},
	{"IsSphere",IsSphere},
	{"IsOBB",IsOBB},
	{"ControlCurrentLocation",ControlCurrentLocation},
	{"ControlCurrentRotation",ControlCurrentRotation},
	{"ControlCurrentPosition",ControlCurrentPosition},
	{"DeregisterModel",DeregisterModel},
	{"PlayerMotionSetOffset",PlayerMotionSetOffset},
	{"Activate",Activate},
	{"ValidPos",ValidPos},
	{NULL,NULL}
};
int Phys2Service::ControlVelocity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrVec arg2;
	const lvector* v2 = reinterpret_cast<const lvector*>(
				S.checkUserdata(2,LVECTOR_NAME));
	arg2.x = v2->v.x; arg2.y = v2->v.y; arg2.z = v2->v.z;
	PhysSrv->ControlVelocity(arg1,arg2);
	return 0;
}
int Phys2Service::StopControlVelocity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->StopControlVelocity(arg1);
	return 0;
}
int Phys2Service::SetGravity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	float arg2 = S.checkNumber(2);
	PhysSrv->SetGravity(arg1,arg2);
	return 0;
}
int Phys2Service::GetGravity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	float ret = PhysSrv->GetGravity(arg1);
	S.push(ret);
	return 1;
}
int Phys2Service::HasPhysics(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->HasPhysics(arg1);
	S.push(ret);
	return 1;
}
int Phys2Service::IsSphere(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->IsSphere(arg1);
	S.push(ret);
	return 1;
}
int Phys2Service::IsOBB(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->IsOBB(arg1);
	S.push(ret);
	return 1;
}
int Phys2Service::ControlCurrentLocation(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->ControlCurrentLocation(arg1);
	return 0;
}
int Phys2Service::ControlCurrentRotation(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->ControlCurrentRotation(arg1);
	return 0;
}
int Phys2Service::ControlCurrentPosition(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->ControlCurrentPosition(arg1);
	return 0;
}
int Phys2Service::DeregisterModel(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->DeregisterModel(arg1);
	return 0;
}
int Phys2Service::PlayerMotionSetOffset(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	int arg1 = 0;
	cScrVec arg2;
	if (S.getTop() >= 2)
	{
		arg1 = S.checkInteger(1);
		const lvector* v2 = reinterpret_cast<const lvector*>(
					S.checkUserdata(2,LVECTOR_NAME));
		arg2.x = v2->v.x; arg2.y = v2->v.y; arg2.z = v2->v.z;
	}
	else
	{
		const lvector* v2 = reinterpret_cast<const lvector*>(
					S.checkUserdata(1,LVECTOR_NAME));
		arg2.x = v2->v.x; arg2.y = v2->v.y; arg2.z = v2->v.z;
	}
	PhysSrv->PlayerMotionSetOffset(arg1,arg2);
	return 0;
}
int Phys2Service::Activate(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = PhysSrv->Activate(arg1);
	S.push(ret == 0);
	return 1;
}
int Phys2Service::ValidPos(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->ValidPos(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry Property2Service::Methods[] = {
	{"Get",Get},
	{"Set",Set},
	{"SetLocal",SetLocal},
	{"Add",Add},
	{"Remove",Remove},
	{"CopyFrom",CopyFrom},
	{"Possessed",Possessed},
	{NULL,NULL}
};
int Property2Service::Get(luax::Handle L)
{
	luax::State S(L);
	PropertySrv.set(g_pScriptManager);
	S.setTop(3);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	const char * arg3 = S.optString(3,NULL,NULL);
	cMultiParm ret;
	PropertySrv->Get(ret,arg1,arg2,arg3);
	ScriptMultiParm(S).push(ret);
	return 1;
}
int Property2Service::Set(luax::Handle L)
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
int Property2Service::SetLocal(luax::Handle L)
{
	luax::State S(L);
	PropertySrv.set(g_pScriptManager);
	S.setTop(4);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	const char * arg3 = S.checkString(3,NULL);
	ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
	long ret = PropertySrv->SetLocal(arg1,arg2,arg3,arg4);
	S.push(ret == 0);
	return 1;
}

const luax::Registry Sound2Service::Methods[] = {
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
int Sound2Service::Play(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	true_bool ret;
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	if (!(S.isNoneOrNil(3) || S.isBoolean(3)))
	{
		eSoundSpecial arg4 = S.toBoolean(4) ? kSoundLoop : kSoundNormal;
		eSoundNetwork arg5 = eSoundNetwork(S.checkOption(5,SoundNetwork,"normal"));
		if (S.isNumber(3))
		{
			object arg3 = S.toInteger(3);
			SoundSrv->PlayAtObject(ret,arg1,arg2,arg3,arg4,arg5);
		}
		else
		{
			cScrVec arg3;
			const lvector* v3 = reinterpret_cast<const lvector*>(
						S.checkUserdata(3,LVECTOR_NAME));
			arg3.x = v3->v.x; arg3.y = v3->v.y; arg3.z = v3->v.z;
			SoundSrv->PlayAtLocation(ret,arg1,arg2,arg3,arg4,arg5);
		}
	}
	else
	{
		eSoundSpecial arg3 = S.toBoolean(3) ? kSoundLoop : kSoundNormal;
		eSoundNetwork arg4 = eSoundNetwork(S.checkOption(4,SoundNetwork,"normal"));
		SoundSrv->Play(ret,arg1,arg2,arg3,arg4);
	}
	S.push(bool(ret));
	return 1;
}
int Sound2Service::PlayAmbient(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	eSoundSpecial arg3 = S.toBoolean(3) ? kSoundLoop : kSoundNormal;
	eSoundNetwork arg4 = eSoundNetwork(S.checkOption(4,SoundNetwork,"normal"));
	true_bool ret;
	SoundSrv->PlayAmbient(ret,arg1,arg2,arg3,arg4);
	S.push(bool(ret));
	return 1;
}
int Sound2Service::PlaySchema(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	true_bool ret;
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	if (!(S.isNoneOrNil(3) || S.isBoolean(3)))
	{
		eSoundNetwork arg4 = eSoundNetwork(S.checkOption(4,SoundNetwork,"normal"));
		if (S.isNumber(3))
		{
			object arg3 = S.toInteger(3);
			SoundSrv->PlaySchemaAtObject(ret,arg1,arg2,arg3,arg4);
		}
		else
		{
			cScrVec arg3;
			const lvector* v3 = reinterpret_cast<const lvector*>(
						S.checkUserdata(3,LVECTOR_NAME));
			arg3.x = v3->v.x; arg3.y = v3->v.y; arg3.z = v3->v.z;
			SoundSrv->PlaySchemaAtLocation(ret,arg1,arg2,arg3,arg4);
		}
	}
	else
	{
		eSoundNetwork arg3 = eSoundNetwork(S.checkOption(3,SoundNetwork,"normal"));
		SoundSrv->PlaySchema(ret,arg1,arg2,arg3);
	}
	S.push(bool(ret));
	return 1;
}
int Sound2Service::PlaySchemaAmbient(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	eSoundNetwork arg3 = eSoundNetwork(S.checkOption(3,SoundNetwork,"normal"));
	true_bool ret;
	SoundSrv->PlaySchemaAmbient(ret,arg1,arg2,arg3);
	S.push(bool(ret));
	return 1;
}
int Sound2Service::PlayEnvSchema(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	object arg3 = S.optInteger(3);
	object arg4 = S.optInteger(4);
	eEnvSoundLoc arg5 = eEnvSoundLoc(S.checkOption(5,EnvSoundLoc,"onobj"));
	eSoundNetwork arg6 = eSoundNetwork(S.checkOption(6,SoundNetwork,"normal"));
	true_bool ret;
	SoundSrv->PlayEnvSchema(ret,arg1,arg2,arg3,arg4,arg5,arg6);
	S.push(bool(ret));
	return 1;
}
int Sound2Service::PlayVoiceOver(luax::Handle L)
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
int Sound2Service::Halt(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.optString(2,NULL,"");
	object arg3 = S.optInteger(3,0);
	int ret = SoundSrv->Halt(arg1,arg2,arg3);
	S.push(luax::Number(ret));
	return 1;
}
int Sound2Service::HaltSchema(luax::Handle L)
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
int Sound2Service::HaltSpeech(luax::Handle L)
{
	luax::State S(L);
	SoundSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	SoundSrv->HaltSpeech(arg1);
	return 0;
}
int Sound2Service::PreLoad(luax::Handle L)
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
