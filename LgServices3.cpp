/******************************************************************************
 *    LgServices3.cpp
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
#undef _DARKGAME
#define _DARKGAME 3
#include "LgServices.h"
#include "ScriptModule.h"
#include "LgMultiParm.h"
#include "utils.h"

#include <cstring>

#include <lg/scrservices.h>
#include <lua/vec.h>

namespace Lgs
{

const luax::Registry DarkUI3Service::Methods[] = {
	{"TextMessage",TextMessage},
	{"ReadBook",ReadBook},
	{"InvItem",InvItem},
	{"InvWeapon",InvWeapon},
	{"InvSelect",InvSelect},
	{"IsCommandBound",IsCommandBound},
	{"DescribeKeyBinding",DescribeKeyBinding},
	{NULL,NULL}
};
int DarkUI3Service::TextMessage(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int arg2 = S.optInteger(2);
	ShockGameSrv->AddText(arg1,0,arg2);
	return 0;
}

const luax::Registry NetworkingService::Methods[] = {
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
	{NULL,NULL}
};
int NetworkingService::Broadcast(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	S.setTop(4);
	object arg1 = S.checkInteger(1);
	const char* arg2 = S.checkString(2,NULL);
	int arg3 = S.checkInteger(3);
	ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
	long ret = NetworkingSrv->Broadcast(arg1,arg2,arg3,arg4);
	S.push(ret != 0);
	return 1;
}
int NetworkingService::SendToProxy(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	S.setTop(4);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	const char* arg3 = S.checkString(3,NULL);
	ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
	long ret = NetworkingSrv->SendToProxy(arg1,arg2,arg3,arg4);
	S.push(ret != 0);
	return 1;
}
int NetworkingService::TakeOver(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = NetworkingSrv->TakeOver(arg1);
	S.push(ret != 0);
	return 1;
}
int NetworkingService::GiveTo(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = NetworkingSrv->GiveTo(arg1,arg2);
	S.push(ret != 0);
	return 1;
}
int NetworkingService::IsPlayer(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = NetworkingSrv->IsPlayer(arg1);
	S.push(ret);
	return 1;
}
int NetworkingService::IsMultiplayer(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	bool ret = NetworkingSrv->IsMultiplayer();
	S.push(ret);
	return 1;
}
int NetworkingService::SetProxyOneShotTimer(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	S.setTop(4);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	float arg3 = S.checkNumber(3);
	ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
	int ret = NetworkingSrv->SetProxyOneShotTimer(arg1,arg2,arg3,arg4);
	S.push(ret);
	return 1;
}
int NetworkingService::FirstPlayer(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object ret;
	NetworkingSrv->FirstPlayer(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int NetworkingService::NextPlayer(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object ret;
	NetworkingSrv->NextPlayer(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int NetworkingService::Suspend(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	NetworkingSrv->Suspend();
	return 0;
}
int NetworkingService::Resume(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	NetworkingSrv->Resume();
	return 0;
}
int NetworkingService::HostedHere(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = NetworkingSrv->HostedHere(arg1);
	S.push(ret);
	return 1;
}
int NetworkingService::IsProxy(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = NetworkingSrv->IsProxy(arg1);
	S.push(ret);
	return 1;
}
int NetworkingService::LocalOnly(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = NetworkingSrv->LocalOnly(arg1);
	S.push(ret);
	return 1;
}
int NetworkingService::IsNetworking(luax::Handle L)
{
	luax::State S(L);
	NetworkingSrv.set(g_pScriptManager);
	bool ret = NetworkingSrv->IsNetworking();
	S.push(ret);
	return 1;
}

const luax::Registry Object3Service::Methods[] = {
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
int Object3Service::IsPositionValid(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	true_bool ret;
	ObjectSrv->IsPositionValid(ret,arg1);
	S.push(bool(ret));
	return 1;
}
int Object3Service::FindClosestObjectNamed(luax::Handle L)
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
		true_bool valid;
		double distance = double(LONG_MAX)+1;
		SInterface<IObjectQuery> q;
		q = TraitMan->Query(arc,kTraitQueryChildren|kTraitQueryFull);
		for (; !q->Done(); q->Next())
		{
			object o = q->Object();
			if (o < 0)
				continue;
			ObjectSrv->IsPositionValid(valid,o);
			if (!valid)
				continue;
			ObjectSrv->Position(other,o);
			double d = pos.Distance(other);
			if (d < distance)
				ret = o;
		}
	}
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int Object3Service::AddMetaPropertyToMany(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	int ret = ObjectSrv->AddMetaPropertyToMany(arg1,arg2);
	S.push(luax::Number(ret));
	return 1;
}
int Object3Service::RemoveMetaPropertyFromMany(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	int ret = ObjectSrv->RemoveMetaPropertyFromMany(arg1,arg2);
	S.push(luax::Number(ret));
	return 1;
}
int Object3Service::RenderedThisFrame(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	true_bool ret;
	ObjectSrv->RenderedThisFrame(ret,arg1);
	S.push(bool(ret));
	return 1;
}

const luax::Registry Phys3Service::Methods[] = {
	{"SubscribeMsg",SubscribeMsg},
	{"UnsubscribeMsg",UnsubscribeMsg},
	{"LaunchProjectile",LaunchProjectile},
	{"SetVelocity",SetVelocity},
	{"GetVelocity",GetVelocity},
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
int Phys3Service::HasPhysics(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->HasPhysics(arg1);
	S.push(ret);
	return 1;
}
int Phys3Service::IsSphere(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->IsSphere(arg1);
	S.push(ret);
	return 1;
}
int Phys3Service::IsOBB(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->IsOBB(arg1);
	S.push(ret);
	return 1;
}
int Phys3Service::ControlCurrentLocation(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->ControlCurrentLocation(arg1);
	return 0;
}
int Phys3Service::ControlCurrentRotation(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->ControlCurrentRotation(arg1);
	return 0;
}
int Phys3Service::ControlCurrentPosition(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->ControlCurrentPosition(arg1);
	return 0;
}
int Phys3Service::DeregisterModel(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	PhysSrv->DeregisterModel(arg1);
	return 0;
}
int Phys3Service::PlayerMotionSetOffset(luax::Handle L)
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
int Phys3Service::Activate(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = PhysSrv->Activate(arg1);
	S.push(ret == 0);
	return 1;
}
int Phys3Service::ValidPos(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PhysSrv->ValidPos(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry ShockGameService::Methods[] = {
	{"DestroyCursorObj",DestroyCursorObj},
	{"DestroyInvObj",DestroyInvObj},
	{"DestroyAllByName",DestroyAllByName},
	{"SlayAllByName",SlayAllByName},
	{"TweqAllByName",TweqAllByName},
	{"HideInvObj",HideInvObj},
	{"AddInvObj",AddInvObj},
	{"SetPlayerPsiPoints",SetPlayerPsiPoints},
	{"GetPlayerPsiPoints",GetPlayerPsiPoints},
	{"GetPlayerMaxPsiPoints",GetPlayerMaxPsiPoints},
	{"GrantPsiPower",GrantPsiPower},
	{"IsPsiActive",IsPsiActive},
	{"SetSelectedPsiPower",SetSelectedPsiPower},
	{"Replicator",Replicator},
	{"SetLogTime",SetLogTime},
	{"UseLog",UseLog},
	{"TriggerLog",TriggerLog},
	{"FindLogData",FindLogData},
	{"AttachCamera",AttachCamera},
	{"CutSceneModeOn",CutSceneModeOn},
	{"CutSceneModeOff",CutSceneModeOff},
	{"CreatePlayerPuppet",CreatePlayerPuppet},
	{"DestroyPlayerPuppet",DestroyPlayerPuppet},
	{"Container",Container},
	{"YorN",YorN},
	{"Keypad",Keypad},
	{"HRM",HRM},
	{"TechTool",TechTool},
	{"PayNanites",PayNanites},
	{"OverlayChange",OverlayChange},
	{"OverlaySetObj",OverlaySetObj},
	{"OverlayGetObj",OverlayGetObj},
	{"OverlayChangeObj",OverlayChangeObj},
	{"OverlayOn",OverlayOn},
	{"Equipped",Equipped},
	{"Equip",Equip},
	{"WearArmor",WearArmor},
	{"LevelTransport",LevelTransport},
	{"SpewLockData",SpewLockData},
	{"CheckLocked",CheckLocked},
	{"AddText",AddText},
	{"AddTranslatableText",AddTranslatableText},
	{"AmmoLoad",AmmoLoad},
	{"GetClip",GetClip},
	{"SetModify",SetModify},
	{"AddExp",AddExp},
	{"HasTrait",HasTrait},
	{"HasImplant",HasImplant},
	{"HealObj",HealObj},
	{"Research",Research},
	{"ResearchConsume",ResearchConsume},
	{"GetArchetypeName",GetArchetypeName},
	{"GetStat",GetStat},
	{"GetSelectedObj",GetSelectedObj},
	{"GetDistantSelectedObj",GetDistantSelectedObj},
	{"FindSpawnPoint",FindSpawnPoint},
	{"CountEcoMatching",CountEcoMatching},
	{"RecalcStats",RecalcStats},
	{"PlayVideo",PlayVideo},
	{"ClearRadiation",ClearRadiation},
	{"RadiationHack",RadiationHack},
	{"LoadCursor",LoadCursor},
	{"SetPlayerVolume",SetPlayerVolume},
	{"RandRange",RandRange},
	{"AddSpeedControl",AddSpeedControl},
	{"RemoveSpeedControl",RemoveSpeedControl},
	{"PreventSwap",PreventSwap},
	{"SetObjState",SetObjState},
	{"DisableAlarmGlobal",DisableAlarmGlobal},
	{"AddAlarm",AddAlarm},
	{"RemoveAlarm",RemoveAlarm},
	{"IsAlarmActive",IsAlarmActive},
	{"Frob",Frob},
	{"SetExplored",SetExplored},
	{"RemoveFromContainer",RemoveFromContainer},
	{"ActivateMap",ActivateMap},
	{"SimTime",SimTime},
	{"StartFadeIn",StartFadeIn},
	{"StartFadeOut",StartFadeOut},
	{"PlayerModeSimple",PlayerModeSimple},
	{"PlayerMode",PlayerMode},
	{"EndGame",EndGame},
	{"AllowDeath",AllowDeath},
	{"GetHazardResistance",GetHazardResistance},
	{"GetBurnDmg",GetBurnDmg},
	{"PlayerGun",PlayerGun},
	{"PsiRadarScan",PsiRadarScan},
	{"PseudoProjectile",PseudoProjectile},
	{"Censored",Censored},
	{"DebriefMode",DebriefMode},
	{"TlucTextAdd",TlucTextAdd},
	{"Mouse",Mouse},
	{"MouseCursor",MouseCursor},
	{"RefreshInv",RefreshInv},
	{"TreasureTable",TreasureTable},
	{"VaporizeInv",VaporizeInv},
	{"ShutoffPsi",ShutoffPsi},
	{"SetQBHacked",SetQBHacked},
	{"ZeroControls",ZeroControls},
	{"NoMove",NoMove},
	{"ValidGun",ValidGun},
	{"UpdateMovingTerrainVelocity",UpdateMovingTerrainVelocity},
	{NULL,NULL}
};
int ShockGameService::DestroyCursorObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->DestroyCursorObj();
	return 0;
}
int ShockGameService::DestroyInvObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->DestroyInvObj(arg1);
	return 0;
}
int ShockGameService::DestroyAllByName(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	long ret = ShockGameSrv->DestroyAllByName(arg1);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::SlayAllByName(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	long ret = ShockGameSrv->SlayAllByName(arg1);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::TweqAllByName(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int arg2 = S.isNone(2) ? 1 : S.toBoolean(2);
	long ret = ShockGameSrv->TweqAllByName(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::HideInvObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->HideInvObj(arg1);
	return 0;
}
int ShockGameService::AddInvObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockGameSrv->AddInvObj(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::SetPlayerPsiPoints(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	ShockGameSrv->SetPlayerPsiPoints(arg1);
	return 0;
}
int ShockGameService::GetPlayerPsiPoints(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int ret = ShockGameSrv->GetPlayerPsiPoints();
	S.push(ret);
	return 1;
}
int ShockGameService::GetPlayerMaxPsiPoints(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int ret = ShockGameSrv->GetPlayerMaxPsiPoints();
	S.push(ret);
	return 1;
}
int ShockGameService::GrantPsiPower(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	if (0 > arg2 || arg2 >= 64)
		return S.argError(2,"out of range");
	ShockGameSrv->GrantPsiPower(arg1,arg2);
	return 0;
}
int ShockGameService::IsPsiActive(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (0 > arg1 || arg1 >= 64)
		return S.argError(1,"out of range");
	bool ret = ShockGameSrv->IsPsiActive(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::SetSelectedPsiPower(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (0 > arg1 || arg1 >= 64)
		return S.argError(1,"out of range");
	ShockGameSrv->SetSelectedPsiPower(arg1);
	return 0;
}
int ShockGameService::Replicator(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->Replicator(arg1);
	return 0;
}
const char* const ShockGameService::LogType[] = {
	"Emails","Logs","Notes","Info","Research",NULL
};
int ShockGameService::SetLogTime(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,LogType);
	int arg3 = S.checkInteger(3);
	if (0 > arg3 || arg3 >= 32)
		return S.argError(1,"out of range");
	long ret = ShockGameSrv->SetLogTime(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}
int ShockGameService::UseLog(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.toBoolean(2);
	long ret = ShockGameSrv->UseLog(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::TriggerLog(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkOption(1,LogType);
	int arg2 = S.checkInteger(2);
	int arg3 = S.checkInteger(3);
	int arg4 = S.toBoolean(4);
	int ret = ShockGameSrv->TriggerLog(arg1,arg2,arg3,arg4);
	S.push(ret);
	return 1;
}
int ShockGameService::FindLogData(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	int ret1,ret2;
	long ret = ShockGameSrv->FindLogData(arg1,arg2,&ret1,&ret2);
	if (ret == 0)
	{
		S.push(ret1).push(ret2);
		return 2;
	}
	S.push(luax::Nil());
	return 1;
}
int ShockGameService::AttachCamera(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	cScrStr arg1 = S.checkString(1,NULL);
	ShockGameSrv->AttachCamera(arg1);
	return 0;
}
int ShockGameService::CutSceneModeOn(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	cScrStr arg1 = S.checkString(1,NULL);
	ShockGameSrv->CutSceneModeOn(arg1);
	return 0;
}
int ShockGameService::CutSceneModeOff(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->CutSceneModeOff();
	return 0;
}
int ShockGameService::CreatePlayerPuppet(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	cScrStr arg1 = S.optString(1,NULL,NULL);
	int ret;
	if (arg1)
		ret = ShockGameSrv->CreatePlayerPuppet(arg1);
	else
		ret = ShockGameSrv->CreatePlayerPuppetDefault();
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int ShockGameService::DestroyPlayerPuppet(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->DestroyPlayerPuppet();
	return 0;
}
int ShockGameService::Container(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->Container(arg1);
	return 0;
}
int ShockGameService::YorN(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	ShockGameSrv->YorN(arg1,arg2);
	return 0;
}
int ShockGameService::Keypad(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->Keypad(arg1);
	return 0;
}
int ShockGameService::HRM(luax::Handle L)
{
	static const char* const TechType[] = {
		"Hack","Repair","Modify","Maintain","Research",NULL
	};
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkOption(1,TechType);
	object arg2 = S.checkInteger(2);
	int arg3 = S.toBoolean(3);
	long ret = ShockGameSrv->HRM(arg1,arg2,arg3);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::TechTool(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = ShockGameSrv->TechTool(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::PayNanites(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	long ret = ShockGameSrv->PayNanites(arg1);
	S.push(ret == 0);
	return 1;
}
const char* const ShockGameService::OverlayChangeMode[] = {
	"off","on","toggle", // not sure about the last one
	NULL
};
int ShockGameService::OverlayChange(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,OverlayChangeMode);
	if (0 > arg1 || arg1 >= 50)
		return S.argError(1,"out of range");
	ShockGameSrv->OverlayChange(arg1,arg2);
	return 0;
}
int ShockGameService::OverlaySetObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	if (0 > arg1 || arg1 >= 50)
		return S.argError(1,"out of range");
	ShockGameSrv->OverlaySetObj(arg1,arg2);
	return 0;
}
int ShockGameService::OverlayGetObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object ret;
	ShockGameSrv->OverlayGetObj(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::OverlayChangeObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,OverlayChangeMode);
	object arg3 = S.checkInteger(3);
	if (0 > arg1 || arg1 >= 50)
		return S.argError(1,"out of range");
	ShockGameSrv->OverlayChangeObj(arg1,arg2,arg3);
	return 0;
}
int ShockGameService::OverlayOn(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (0 > arg1 || arg1 >= 50)
		return S.argError(1,"out of range");
	bool ret = ShockGameSrv->OverlayOn(arg1);
	S.push(bool(ret));
	return 1;
}
int ShockGameService::Equipped(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	object ret;
	ShockGameSrv->Equipped(ret,arg1);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::Equip(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockGameSrv->Equip(arg1,arg2);
	return 0;
}
int ShockGameService::WearArmor(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.optInteger(1);
	ShockGameSrv->WearArmor(arg1);
	return 0;
}
int ShockGameService::LevelTransport(luax::Handle L)
{
	static const char* const TransportFlags[] = {
		"RoomObjs","NetSynch",NULL
	};
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int arg2 = S.checkInteger(2);
	unsigned int arg3 = S.checkFlags(3,TransportFlags,"");
	long ret = ShockGameSrv->LevelTransport(arg1,arg2,arg3);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::SpewLockData(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	S.checkAny(2);
	int arg2 = S.toBoolean(2);
	ShockGameSrv->SpewLockData(arg1,arg2);
	return 0;
}
int ShockGameService::CheckLocked(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.toBoolean(2);
	object arg3;
	if (arg2)
		arg3 = S.checkInteger(3);
	else
		arg3 = S.optInteger(3,0);
	bool ret = ShockGameSrv->CheckLocked(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}
int ShockGameService::AddText(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	if (S.isInteger(1))
	{
		object arg1 = S.checkInteger(1); // object
		const char * arg2 = S.checkString(2,NULL); // property
		object arg3; // player
		int arg4; // time
		if (S.getTop() >= 4)
		{
			arg3 = S.optInteger(3);
			arg4 = S.checkIntOrNumber(4);
		}
		else
		{
			arg3 = 0;
			arg4 = S.checkIntOrNumber(3);
		}
		ShockGameSrv->AddTextObjProp(arg1,arg2,arg3,arg4);
	}
	else
	{
		const char * arg1 = S.checkString(1,NULL); // string
		object arg2; // player
		int arg3; // time
		if (S.getTop() >= 3)
		{
			arg2 = S.optInteger(2);
			arg3 = S.checkIntOrNumber(3);
		}
		else
		{
			arg2 = 0;
			arg3 = S.checkIntOrNumber(2);
		}
		ShockGameSrv->AddText(arg1,arg2,arg3);
	}
	return 0;
}
int ShockGameService::AddTranslatableText(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL); // name
	const char * arg2 = S.checkString(2,NULL); // resource
	object arg3 = S.checkInteger(3); // object
	int arg4,arg5,arg6;
	arg4 = S.checkIntOrNumber(4);
	switch (S.getTop())
	{
	default: // arg4 - index
		arg5 = S.checkIntOrNumber(5); // param
		arg6 = S.checkIntOrNumber(6); // time
		ShockGameSrv->AddTranslatableTextIndexInt(arg1,arg2,arg3,arg4,arg5,arg6);
		break;
	case 5: // arg4 - parameter
		arg5 = S.checkIntOrNumber(5); // time
		ShockGameSrv->AddTranslatableTextIndexInt(arg1,arg2,arg3,-1,arg4,arg5);
		break;
	case 4: // arg4 - time
		ShockGameSrv->AddTranslatableText(arg1,arg2,arg3,arg4);
		break;
	}
	return 0;
}
int ShockGameService::AmmoLoad(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockGameSrv->AmmoLoad(arg1,arg2);
	return 0;
}
int ShockGameService::GetClip(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int ret = ShockGameSrv->GetClip(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::SetModify(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkIntOrNumber(2);
	ShockGameSrv->SetModify(arg1,arg2);
	return 0;
}
int ShockGameService::AddExp(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkIntOrNumber(2);
	int arg3 = S.isNone(3) ? 1 : S.toBoolean(3);
	ShockGameSrv->AddExp(arg1,arg2,arg3);
	return 0;
}
int ShockGameService::HasTrait(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	bool ret = ShockGameSrv->HasTrait(arg1,arg2);
	S.push(ret);
	return 1;
}
int ShockGameService::HasImplant(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	bool ret = ShockGameSrv->HasImplant(arg1,arg2);
	S.push(ret);
	return 1;
}
int ShockGameService::HealObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkIntOrNumber(2);
	long ret = ShockGameSrv->HealObj(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::Research(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->Research();
	return 0;
}
int ShockGameService::ResearchConsume(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockGameSrv->ResearchConsume(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::GetArchetypeName(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrStr ret;
	ShockGameSrv->GetArchetypeName(ret,arg1);
	if (ret.IsEmpty()) // XXX: does this happen?
		S.push(luax::Nil());
	else
		S.push(ret);
	ret.Free();
	return 1;
}
int ShockGameService::GetStat(luax::Handle L)
{
	static const char* const StatsDesc[] = {
		"STR","END","PSI","AGI","CYB",NULL
	};
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,StatsDesc);
	int ret = ShockGameSrv->GetStat(arg1,arg2);
	S.push(ret); // XXX: what if not a player?
	return 1;
}
int ShockGameService::GetSelectedObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object ret;
	ShockGameSrv->GetSelectedObj(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::GetDistantSelectedObj(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object ret;
	ShockGameSrv->GetDistantSelectedObj(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::FindSpawnPoint(luax::Handle L)
{
	static const char* const SpawnFlags[] = {
		"PopLimit","PlrDist","GotoLoc",
		"SelfMarker","Raycast","Farthest",
		NULL
	};
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	unsigned int arg2 = S.checkFlags(2,SpawnFlags,"");
	object ret;
	ShockGameSrv->FindSpawnPoint(ret,arg1,arg2);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::CountEcoMatching(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int ret = ShockGameSrv->CountEcoMatching(arg1);
	S.push(luax::Number(ret));
	return 1;
}
int ShockGameService::RecalcStats(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->RecalcStats(arg1);
	return 0;
}
int ShockGameService::PlayVideo(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	ShockGameSrv->PlayVideo(arg1);
	return 0;
}
int ShockGameService::ClearRadiation(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->ClearRadiation();
	return 0;
}
int ShockGameService::RadiationHack(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	long ret = ShockGameSrv->RadiationHack();
	S.push(ret);
	return 1;
}
int ShockGameService::LoadCursor(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockGameSrv->LoadCursor(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::SetPlayerVolume(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	float arg1 = S.checkNumber(1);
	ShockGameSrv->SetPlayerVolume(arg1);
	return 0;
}
int ShockGameService::RandRange(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	int arg2 = S.checkIntOrNumber(2);
	int ret = ShockGameSrv->RandRange(arg1,arg2);
	S.push(luax::Number(ret));
	return 1;
}
int ShockGameService::AddSpeedControl(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	float arg2 = S.checkNumber(2);
	float arg3 = S.optNumber(3,1.0);
	ShockGameSrv->AddSpeedControl(arg1,arg2,arg3);
	return 0;
}
int ShockGameService::RemoveSpeedControl(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	ShockGameSrv->RemoveSpeedControl(arg1);
	return 0;
}
int ShockGameService::PreventSwap(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->PreventSwap();
	return 0;
}
const char* const ShockGameService::ObjStates[] = {
	"Normal","Broken","Destroyed","Unresearched","Locked","Hacked",NULL
};
int ShockGameService::SetObjState(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,ObjStates);
	ShockGameSrv->SetObjState(arg1,arg2);
	return 0;
}
int ShockGameService::DisableAlarmGlobal(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->DisableAlarmGlobal();
	return 0;
}
int ShockGameService::AddAlarm(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	ShockGameSrv->AddAlarm(arg1);
	return 0;
}
int ShockGameService::RemoveAlarm(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->RemoveAlarm();
	return 0;
}
int ShockGameService::IsAlarmActive(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	bool ret = ShockGameSrv->IsAlarmActive();
	S.push(ret);
	return 1;
}
int ShockGameService::Frob(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.toBoolean(1);
	ShockGameSrv->Frob(arg1);
	return 0;
}
int ShockGameService::SetExplored(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool arg2 = S.isNone(2) ? true : S.toBoolean(2);
	ShockGameSrv->SetExplored(arg1,arg2);
	return 0;
}
int ShockGameService::RemoveFromContainer(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockGameSrv->RemoveFromContainer(arg1,arg2);
	return 0;
}
int ShockGameService::ActivateMap(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->ActivateMap();
	return 0;
}
int ShockGameService::SimTime(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	unsigned long ret = ShockGameSrv->SimTime();
	S.push(luax::Number(ret));
	return 1;
}
int ShockGameService::StartFadeIn(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	int arg2,arg3,arg4;
	if (S.getTop() >= 4)
	{
		arg2 = S.checkIntOrNumber(2);
		arg3 = S.checkIntOrNumber(3);
		arg4 = S.checkIntOrNumber(4);
	}
	else if (S.isString(2))
	{
		unsigned long color = strtocolor(S.asString(2));
		arg2 = (unsigned char)(color);
		arg3 = (unsigned char)(color >> 8);
		arg4 = (unsigned char)(color >> 16);
	}
	else
	{
		unsigned long color = S.optInteger(2);
		arg2 = (unsigned char)(color);
		arg3 = (unsigned char)(color >> 8);
		arg4 = (unsigned char)(color >> 16);
	}
	ShockGameSrv->StartFadeIn(arg1,arg2,arg3,arg4);
	return 0;
}
int ShockGameService::StartFadeOut(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	int arg2,arg3,arg4;
	if (S.getTop() >= 4)
	{
		arg2 = S.checkIntOrNumber(2);
		arg3 = S.checkIntOrNumber(3);
		arg4 = S.checkIntOrNumber(4);
	}
	else if (S.isString(2))
	{
		unsigned long color = strtocolor(S.asString(2));
		arg2 = (unsigned char)(color);
		arg3 = (unsigned char)(color >> 8);
		arg4 = (unsigned char)(color >> 16);
	}
	else
	{
		unsigned long color = S.optInteger(2);
		arg2 = (unsigned char)(color);
		arg3 = (unsigned char)(color >> 8);
		arg4 = (unsigned char)(color >> 16);
	}
	ShockGameSrv->StartFadeOut(arg1,arg2,arg3,arg4);
	return 0;
}
int ShockGameService::PlayerModeSimple(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	ShockGameSrv->PlayerModeSimple(arg1);
	return 0;
}
int ShockGameService::PlayerMode(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	ShockGameSrv->PlayerMode(arg1);
	return 0;
}
int ShockGameService::EndGame(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->EndGame();
	return 0;
}
int ShockGameService::AllowDeath(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	bool ret = ShockGameSrv->AllowDeath();
	S.push(ret);
	return 1;
}
int ShockGameService::GetHazardResistance(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkIntOrNumber(1);
	float ret = ShockGameSrv->GetHazardResistance(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::GetBurnDmg(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int ret = ShockGameSrv->GetBurnDmg();
	S.push(ret);
	return 1;
}
int ShockGameService::PlayerGun(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object ret;
	ShockGameSrv->PlayerGun(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::PsiRadarScan(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->PsiRadarScan();
	return 0;
}
int ShockGameService::PseudoProjectile(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	object ret;
	ShockGameSrv->PseudoProjectile(ret,arg1,arg2);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockGameService::Censored(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	bool ret = ShockGameSrv->Censored();
	S.push(ret);
	return 1;
}
int ShockGameService::DebriefMode(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	ShockGameSrv->DebriefMode(arg1);
	return 0;
}
int ShockGameService::TlucTextAdd(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	const char * arg2 = S.checkString(2,NULL);
	int arg3 = S.optIntOrNumber(3,-1);
	ShockGameSrv->TlucTextAdd(arg1,arg2,arg3);
	return 0;
}
int ShockGameService::Mouse(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	S.checkAny(1);
	int arg1 = S.toBoolean(1); // mouse mode
	int arg2 = S.toBoolean(2); // drop?
	ShockGameSrv->Mouse(arg1,arg2);
	return 0;
}
int ShockGameService::MouseCursor(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	bool ret = ShockGameSrv->MouseCursor();
	S.push(ret);
	return 1;
}
int ShockGameService::RefreshInv(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->RefreshInv();
	return 0;
}
int ShockGameService::TreasureTable(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockGameSrv->TreasureTable(arg1);
	return 0;
}
int ShockGameService::VaporizeInv(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->VaporizeInv();
	return 0;
}
int ShockGameService::ShutoffPsi(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	ShockGameSrv->ShutoffPsi();
	// Can technically return non-zero,
	// but only because of not getting the iterator.
	return 1;
}
int ShockGameService::SetQBHacked(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	cScrStr arg1 = S.checkString(1,NULL);
	int arg2 = S.checkIntOrNumber(2);
	long ret = ShockGameSrv->SetQBHacked(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ShockGameService::ZeroControls(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.toBoolean(2);
	ShockGameSrv->ZeroControls(arg1,arg2);
	return 0;
}
int ShockGameService::NoMove(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	S.checkAny(1);
	int arg1 = S.toBoolean(1);
	ShockGameSrv->NoMove(arg1);
	return 0;
}
int ShockGameService::ValidGun(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockGameSrv->ValidGun(arg1);
	S.push(ret);
	return 1;
}
int ShockGameService::UpdateMovingTerrainVelocity(luax::Handle L)
{
	luax::State S(L);
	ShockGameSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	float arg3 = S.checkNumber(3);
	ShockGameSrv->UpdateMovingTerrainVelocity(arg1,arg2,arg3);
	return 0;
}

const luax::Registry ShockObjService::Methods[] = {
	{"FindScriptDonor",FindScriptDonor},
	{NULL,NULL}
};
int ShockObjService::FindScriptDonor(luax::Handle L)
{
	luax::State S(L);
	ShockObjSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	size_t len2;
	const char * arg2 = S.checkString(2,&len2);
	char * str2 = reinterpret_cast<char*>(g_pMalloc->Alloc(len2 + 1));
	memcpy(str2, arg2, len2 + 1);
	int ret = ShockObjSrv->FindScriptDonor(arg1,str2);
	if (ret == 0) // XXX: well?
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}

const luax::Registry ShockPsiService::Methods[] = {
	{"OnDeactivate",OnDeactivate},
	{"GetActiveTime",GetActiveTime},
	{"IsOverloaded",IsOverloaded},
	{NULL,NULL}
};
int ShockPsiService::OnDeactivate(luax::Handle L)
{
	luax::State S(L);
	ShockPsiSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (0 > arg1 || arg1 >= 64)
		return S.argError(1,"out of range");
	ShockPsiSrv->OnDeactivate(ePsiPowers(arg1));
	return 0;
}
int ShockPsiService::GetActiveTime(luax::Handle L)
{
	luax::State S(L);
	ShockPsiSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (0 > arg1 || arg1 >= 64)
		return S.argError(1,"out of range");
	unsigned long ret = ShockPsiSrv->GetActiveTime(ePsiPowers(arg1));
	S.push(luax::Number(ret));
	return 1;
}
int ShockPsiService::IsOverloaded(luax::Handle L)
{
	luax::State S(L);
	ShockPsiSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (0 > arg1 || arg1 >= 64)
		return S.argError(1,"out of range");
	bool ret = ShockPsiSrv->IsOverloaded(ePsiPowers(arg1));
	S.push(ret);
	return 1;
}

const luax::Registry ShockAIService::Methods[] = {
	{"Stun",Stun},
	{"IsStunned",IsStunned},
	{"UnStun",UnStun},
	{"Freeze",Freeze},
	{"UnFreeze",UnFreeze},
	{"NotifyEnterTripwire",NotifyEnterTripwire},
	{"NotifyExitTripwire",NotifyExitTripwire},
	{"ObjectLocked",ObjectLocked},
	{"ValidateSpawn",ValidateSpawn},
	{NULL,NULL}
};
int ShockAIService::Stun(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	size_t len2, len3;
	const char * arg2 = S.checkString(2,&len2);
	const char * arg3 = S.checkString(3,&len3);
	float arg4 = S.checkNumber(4);
	// Someone screwed up and forgot to put '&' on the interface.
	char * str2 = reinterpret_cast<char*>(g_pMalloc->Alloc(len2 + 1));
	char * str3 = reinterpret_cast<char*>(g_pMalloc->Alloc(len3 + 1));
	memcpy(str2, arg2, len2 + 1);
	memcpy(str3, arg3, len3 + 1);
	bool ret = ShockAISrv->Stun(arg1,str2,str3,arg4);
	S.push(ret);
	return 1;
}
int ShockAIService::IsStunned(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockAISrv->IsStunned(arg1);
	S.push(ret);
	return 1;
}
int ShockAIService::UnStun(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockAISrv->UnStun(arg1);
	S.push(ret);
	return 1;
}
int ShockAIService::Freeze(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	float arg2 = S.checkNumber(2);
	bool ret = ShockAISrv->Freeze(arg1,arg2);
	S.push(ret);
	return 1;
}
int ShockAIService::UnFreeze(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockAISrv->UnFreeze(arg1);
	S.push(ret);
	return 1;
}
int ShockAIService::NotifyEnterTripwire(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockAISrv->NotifyEnterTripwire(arg1,arg2);
	return 0;
}
int ShockAIService::NotifyExitTripwire(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockAISrv->NotifyExitTripwire(arg1,arg2);
	return 0;
}
int ShockAIService::ObjectLocked(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = ShockAISrv->ObjectLocked(arg1);
	S.push(ret);
	return 1;
}
int ShockAIService::ValidateSpawn(luax::Handle L)
{
	luax::State S(L);
	ShockAISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockAISrv->ValidateSpawn(arg1,arg2);
	return 0;
}

const luax::Registry ShockWeaponService::Methods[] = {
	{"SetWeaponModel",SetWeaponModel},
	{"GetWeaponModel",GetWeaponModel},
	{"TargetScan",TargetScan},
	{"Home",Home},
	{"DestroyMelee",DestroyMelee},
	{NULL,NULL}
};
int ShockWeaponService::SetWeaponModel(luax::Handle L)
{
	luax::State S(L);
	ShockWeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockWeaponSrv->SetWeaponModel(arg1);
	return 0;
}
int ShockWeaponService::GetWeaponModel(luax::Handle L)
{
	luax::State S(L);
	ShockWeaponSrv.set(g_pScriptManager);
	object ret;
	ShockWeaponSrv->GetWeaponModel(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockWeaponService::TargetScan(luax::Handle L)
{
	luax::State S(L);
	ShockWeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object ret;
	ShockWeaponSrv->TargetScan(ret,arg1);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int ShockWeaponService::Home(luax::Handle L)
{
	luax::State S(L);
	ShockWeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ShockWeaponSrv->Home(arg1,arg2);
	return 0;
}
int ShockWeaponService::DestroyMelee(luax::Handle L)
{
	luax::State S(L);
	ShockWeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	ShockWeaponSrv->DestroyMelee(arg1);
	return 0;
}

}
