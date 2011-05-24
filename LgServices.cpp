/******************************************************************************
 *  LgServices.cpp
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
#define LUAX_INLINE
#include "luax.h"

#include "LgServices.h"
#include "ScriptModule.h"
#include "LgMultiParm.h"
#include "LgLinkset.h"
#include "LgLink.h"
#include "LgObject.h"
#include "utils.h"
#include "mod/modlib.h"

#include <lg/iids.h>
#include <lg/links.h>
#include <lg/properties.h>

#include <cstdlib>
#include <cmath>

namespace Lgs
{

SService<IActReactSrv> ScriptServices::ActReactSrv;
SService<IAIScrSrv> ScriptServices::AISrv;
SService<IAnimTextureSrv> ScriptServices::AnimTextureSrv;
SService<IBowSrv> ScriptServices::BowSrv;
SService<ICameraSrv> ScriptServices::CameraSrv;
//SService<IContainSrv> ScriptServices::ContainSrv;
//SService<IDamageSrv> ScriptServices::DamageSrv;
SService<IDarkGameSrv> ScriptServices::DarkGameSrv;
SService<IDarkUISrv> ScriptServices::DarkUISrv;
SService<IDataSrv> ScriptServices::DataSrv;
SService<IDebugScrSrv> ScriptServices::DebugSrv;
SService<IDoorSrv> ScriptServices::DoorSrv;
SService<IDarkInvSrv> ScriptServices::DarkInvSrv;
SService<IDarkPowerupsSrv> ScriptServices::DarkPowerupsSrv;
//SService<IKeySrv> ScriptServices::KeySrv;
SService<ILightScrSrv> ScriptServices::LightSrv;
SService<ILinkSrv> ScriptServices::LinkSrv;
SService<ILinkToolsSrv> ScriptServices::LinkToolsSrv;
SService<ILockSrv> ScriptServices::LockSrv;
SService<INetworkingSrv> ScriptServices::NetworkingSrv;
SService<IObjectSrv> ScriptServices::ObjectSrv;
SService<IPGroupSrv> ScriptServices::PGroupSrv;
SService<IPhysSrv> ScriptServices::PhysSrv;
SService<IPickLockSrv> ScriptServices::PickLockSrv;
SService<IPlayerLimbsSrv> ScriptServices::PlayerLimbsSrv;
SService<IPropertySrv> ScriptServices::PropertySrv;
SService<IPuppetSrv> ScriptServices::PuppetSrv;
//SService<IQuestSrv> ScriptServices::QuestSrv;
SService<IShockGameSrv> ScriptServices::ShockGameSrv;
SService<IShockObjSrv> ScriptServices::ShockObjSrv;
SService<IShockPsiSrv> ScriptServices::ShockPsiSrv;
SService<IShockAISrv> ScriptServices::ShockAISrv;
SService<IShockWeaponSrv> ScriptServices::ShockWeaponSrv;
SService<ISoundScrSrv> ScriptServices::SoundSrv;
SService<IWeaponSrv> ScriptServices::WeaponSrv;
SService<IDarkHookScriptService> ScriptServices::DarkHookSrv;
SInterface<IPropertyManager> ScriptServices::PropertyMan;
SInterface<ILinkManager> ScriptServices::LinkMan;
SInterface<IObjectSystem> ScriptServices::ObjectSys;
SInterface<ITraitManager> ScriptServices::TraitMan;
SInterface<IContainSys> ScriptServices::ContainSys;
SInterface<IDamageModel> ScriptServices::DamageSys;
SInterface<IKeySys> ScriptServices::KeySys;
SInterface<IQuestData> ScriptServices::QuestData;
SInterface<IReactions> ScriptServices::ARReactions;
SInterface<IStimuli> ScriptServices::ARStimuli;

const ScriptServices::ServiceDef ScriptServices::Thief1Services[] = {
	{"ActReactSrv",ActReact1Service::Methods},
	{"AISrv",AIService::Methods},
	{"AnimTextureSrv",AnimTextureService::Methods},
	{"BowSrv",BowService::Methods},
	{"ContainSrv",ContainService::Methods},
	{"DamageSrv",DamageService::Methods},
	{"DarkGameSrv",DarkGameService::Methods},
	{"DarkInvSrv",DarkInvService::Methods},
	{"DarkPowerupsSrv",DarkPowerupsService::Methods},
	{"DarkUISrv",DarkUIService::Methods},
	{"DataSrv",DataService::Methods},
	{"DebugSrv",DebugService::Methods},
	{"DoorSrv",DoorService::Methods},
	{"KeySrv",KeyService::Methods},
	{"LightSrv",LightService::Methods},
	{"LinkSrv",LinkService::Methods},
	{"LockSrv",LockService::Methods},
	{"ObjectSrv",Object1Service::Methods},
	{"PGroupSrv",PGroupService::Methods},
	{"PhysSrv",Phys1Service::Methods},
	{"PickLockSrv",PickLockService::Methods},
	{"PlayerLimbsSrv",PlayerLimbsService::Methods},
	{"PropertySrv",PropertyService::Methods},
	{"PuppetSrv",PuppetService::Methods},
	{"QuestSrv",QuestService::Methods},
	{"SoundSrv",Sound1Service::Methods},
	{"WeaponSrv",WeaponService::Methods},
	{"DarkHookSrv",DarkHookService::Methods},
	{NULL,NULL}
};
const ScriptServices::ServiceDef ScriptServices::Thief2Services[] = {
	{"ActReactSrv",ActReact2Service::Methods},
	{"AISrv",AIService::Methods},
	{"AnimTextureSrv",AnimTextureService::Methods},
	{"BowSrv",BowService::Methods},
	{"CameraSrv",CameraService::Methods},
	{"ContainSrv",ContainService::Methods},
	{"DamageSrv",DamageService::Methods},
	{"DarkGameSrv",DarkGame2Service::Methods},
	{"DarkInvSrv",DarkInvService::Methods},
	{"DarkPowerupsSrv",DarkPowerupsService::Methods},
	{"DarkUISrv",DarkUIService::Methods},
	{"DataSrv",DataService::Methods},
	{"DebugSrv",DebugService::Methods},
	{"DoorSrv",Door2Service::Methods},
	{"KeySrv",KeyService::Methods},
	{"LightSrv",LightService::Methods},
	{"LinkSrv",LinkService::Methods},
	{"LockSrv",LockService::Methods},
	{"NetworkingSrv",Networking2Service::Methods},
	{"ObjectSrv",Object2Service::Methods},
	{"PGroupSrv",PGroupService::Methods},
	{"PhysSrv",Phys2Service::Methods},
	{"PickLockSrv",PickLockService::Methods},
	{"PlayerLimbsSrv",PlayerLimbsService::Methods},
	{"PropertySrv",Property2Service::Methods},
	{"PuppetSrv",PuppetService::Methods},
	{"QuestSrv",QuestService::Methods},
	{"SoundSrv",Sound2Service::Methods},
	{"WeaponSrv",WeaponService::Methods},
	{"DarkHookSrv",DarkHookService::Methods},
	{NULL,NULL}
};
const ScriptServices::ServiceDef ScriptServices::SShock2Services[] = {
	{"ActReactSrv",ActReact2Service::Methods},
	{"AISrv",AIService::Methods},
	{"AnimTextureSrv",AnimTextureService::Methods},
	{"BowSrv",BowService::Methods},
	{"ContainSrv",ContainService::Methods},
	{"DamageSrv",DamageService::Methods},
	{"DarkGameSrv",DarkGameService::Methods},
	{"DarkInvSrv",DarkInvService::Methods},
	{"DarkPowerupsSrv",DarkPowerupsService::Methods},
	{"DarkUISrv",DarkUI3Service::Methods},
	{"DataSrv",DataService::Methods},
	{"DebugSrv",DebugService::Methods},
	{"DoorSrv",DoorService::Methods},
	{"KeySrv",KeyService::Methods},
	{"LightSrv",LightService::Methods},
	{"LinkSrv",LinkService::Methods},
	{"LockSrv",LockService::Methods},
	{"NetworkingSrv",NetworkingService::Methods},
	{"ObjectSrv",Object3Service::Methods},
	{"PGroupSrv",PGroupService::Methods},
	{"PhysSrv",Phys3Service::Methods},
	{"PickLockSrv",PickLockService::Methods},
	{"PlayerLimbsSrv",PlayerLimbsService::Methods},
	{"PropertySrv",Property2Service::Methods},
	{"PuppetSrv",PuppetService::Methods},
	{"QuestSrv",QuestService::Methods},
	{"ShockAISrv",ShockAIService::Methods},
	{"ShockGameSrv",ShockGameService::Methods},
	{"ShockObjSrv",ShockObjService::Methods},
	{"ShockPsiSrv",ShockPsiService::Methods},
	{"ShockWeaponSrv",ShockWeaponService::Methods},
	{"SoundSrv",Sound2Service::Methods},
	{"WeaponSrv",WeaponService::Methods},
	{"DarkHookSrv",DarkHookService::Methods},
	{NULL,NULL}
};

void ScriptServices::Init(luax::State& S)
{
	static const luax::Registry module[] = {
		{"link",CreateLink},
		{"object",CreateObject},
		{NULL,NULL}
	};
	const ServiceDef* srv;

	IUnknown* pIFace;
	if (SUCCEEDED(g_pScriptManager->QueryInterface(IID_IShockPlayer, reinterpret_cast<void**>(&pIFace))))
	{
		srv = SShock2Services;
		ScriptModule::MPrintf("--- ScriptServices SShock2\n");
		pIFace->Release();
	}
	else
	if (SUCCEEDED(g_pScriptManager->QueryInterface(IID_IObjIDManager, reinterpret_cast<void**>(&pIFace))))
	{
		srv = Thief2Services;
		ScriptModule::MPrintf("--- ScriptServices Thief2\n");
		pIFace->Release();
	}
	else
	{
		srv = Thief1Services;
		ScriptModule::MPrintf("--- ScriptServices Thief1\n");
	}
	S.registerLib(module, "lgs");
	for (; srv->name; srv++)
	{
		size_t num = 0;
		while (srv->reg[num].name) ++num;
		S.createTable(0,num)
		 .registerLib(srv->reg)
		 .setField(srv->name);
	}
	S.pop();
}

int ScriptServices::CreateLink(luax::Handle L)
{
	luax::State S(L);
	new(S) LgLink(S.optInteger(1));
	return 1;
}

int ScriptServices::CreateObject(luax::Handle L)
{
	luax::State S(L);
	if (S.getType(1) == luax::TString)
		new(S) LgObject(S.asString(1));
	else
		new(S) LgObject(S.optInteger(1));
	return 1;
}

int ActReactService::React(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	reaction_kind arg1;
	if (S.getType(1) == luax::TString)
		arg1 = ActReactSrv->GetReactionNamed(S.asString(1));
	else
		arg1 = S.checkInteger(1);
	float arg2 = S.checkNumber(2);  // intensity
	object arg3 = S.checkInteger(3); // source
	object arg4 = S.checkInteger(4); // destination
	S.setTop(12);
	ScriptMultiParm mp5(S); cMultiParm const& arg5 = mp5.pop(5);
	ScriptMultiParm mp6(S); cMultiParm const& arg6 = mp6.pop(6);
	ScriptMultiParm mp7(S); cMultiParm const& arg7 = mp7.pop(7);
	ScriptMultiParm mp8(S); cMultiParm const& arg8 = mp8.pop(8);
	ScriptMultiParm mp9(S); cMultiParm const& arg9 = mp9.pop(9);
	ScriptMultiParm mp10(S); cMultiParm const& arg10 = mp10.pop(10);
	ScriptMultiParm mp11(S); cMultiParm const& arg11 = mp11.pop(11);
	ScriptMultiParm mp12(S); cMultiParm const& arg12 = mp12.pop(12);
	long ret = ActReactSrv->React(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
	S.push(ret == 0);
	return 1;
}
int ActReactService::GetReactionNamed(luax::Handle L)
{
	luax::State S(L);
	ARReactions.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	long ret = ARReactions->GetReactionNamed(arg1);
	if (ret == -1)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int ActReactService::GetReactionName(luax::Handle L)
{
	luax::State S(L);
	ARReactions.set(g_pScriptManager);
	long arg1 = S.checkInteger(1);
	const sReactionDesc * desc = ARReactions->DescribeReaction(arg1);
	if (desc)
		S.push(desc->szName);
	else
		S.push(luax::Nil());
	return 1;
}
int ActReactService::SubscribeToStimulus(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = ActReactSrv->SubscribeToStimulus(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ActReactService::UnsubscribeToStimulus(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = ActReactSrv->UnsubscribeToStimulus(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ActReactService::BeginContact(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	if (arg1 == 0 || arg2 == 0)
	{
		S.push(false);
		return 1;
	}
	long ret = ActReactSrv->BeginContact(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ActReactService::EndContact(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	if (arg1 == 0 || arg2 == 0)
	{
		S.push(false);
		return 1;
	}
	long ret = ActReactSrv->EndContact(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ActReactService::SetSingleSensorContact(luax::Handle L)
{
	luax::State S(L);
	ActReactSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	if (arg1 == 0 || arg2 == 0)
	{
		S.push(false);
		return 1;
	}
	long ret = ActReactSrv->SetSingleSensorContact(arg1,arg2);
	S.push(ret == 0);
	return 1;
}
int ActReactService::GetStimulusNamed(luax::Handle L)
{
	luax::State S(L);
	ARStimuli.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int ret = ARStimuli->GetStimulusNamed(arg1);
	if (ret == 0)
		S.push(ret);
	else
		S.push(luax::Nil());
	return 1;
}
int ActReactService::GetStimulusName(luax::Handle L)
{
	luax::State S(L);
	ARStimuli.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * ret = ARStimuli->GetStimulusName(arg1);
	if (ret == NULL)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int ActReactService::IsStimulus(luax::Handle L)
{
	luax::State S(L);
	ARStimuli.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = ARStimuli->IsStimulus(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry AIService::Methods[] = {
	{"MakeGotoObjLoc",MakeGotoObjLoc},
	{"MakeFrobObjWith",MakeFrobObjWith},
	{"MakeFrobObj",MakeFrobObj},
	{"GetAlertLevel",GetAlertLevel},
	{"SetMinimumAlert",SetMinimumAlert},
	{"ClearGoals",ClearGoals},
	{"SetScriptFlags",SetScriptFlags},
	{"ClearAlertness",ClearAlertness},
	{"Signal",Signal},
	{"StartConversation",StartConversation},
	{NULL,NULL}
};
const char* const AIService::ScriptSpeed[] = { "slow","normal","fast",NULL };
const char* const AIService::ActionPriority[] = { "low","normal","high",NULL };
int AIService::MakeGotoObjLoc(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	S.setTop(5);
	int arg1 = S.checkInteger(1);  // AI object
	object arg2 = S.checkInteger(2); // destination
	eAIScriptSpeed arg3 = eAIScriptSpeed(S.checkOption(3,ScriptSpeed,"normal"));
	eAIActionPriority arg4 = eAIActionPriority(S.checkOption(4,ActionPriority,"normal"));
	ScriptMultiParm mp5(S); cMultiParm const& arg5 = mp5.pop(5);
	true_bool ret;
	AISrv->MakeGotoObjLoc(ret,arg1,arg2,arg3,arg4,arg5);
	S.push(bool(ret));
	return 1;
}
int AIService::MakeFrobObjWith(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	S.setTop(5);
	int arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	object arg3 = S.checkInteger(3);
	eAIActionPriority arg4 = eAIActionPriority(S.checkOption(4,ActionPriority,"normal"));
	ScriptMultiParm mp5(S); cMultiParm const& arg5 = mp5.pop(5);
	true_bool ret;
	AISrv->MakeFrobObjWith(ret,arg1,arg2,arg3,arg4,arg5);
	S.push(bool(ret));
	return 1;
}
int AIService::MakeFrobObj(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	S.setTop(4);
	int arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	eAIActionPriority arg3 = eAIActionPriority(S.checkOption(3,ActionPriority,"normal"));
	ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
	true_bool ret;
	AISrv->MakeFrobObj(ret,arg1,arg2,arg3,arg4);
	S.push(bool(ret));
	return 1;
}
int AIService::GetAlertLevel(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	eAIScriptAlertLevel ret = AISrv->GetAlertLevel(arg1);
	S.push(luax::Integer(ret));
	return 1;
}
int AIService::SetMinimumAlert(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	eAIScriptAlertLevel arg2 = eAIScriptAlertLevel(S.checkInteger(2));
	AISrv->SetMinimumAlert(arg1,arg2);
	return 0;
}
int AIService::ClearGoals(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	AISrv->ClearGoals(arg1);
	return 0;
}
int AIService::SetScriptFlags(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.toBoolean(2) ? 1 : 0;
	AISrv->SetScriptFlags(arg1,arg2);
	return 0;
}
int AIService::ClearAlertness(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	AISrv->ClearAlertness(arg1);
	return 0;
}
int AIService::Signal(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	cScrStr arg2 = S.checkString(2,NULL);
	AISrv->Signal(arg1,arg2);
	return 0;
}
int AIService::StartConversation(luax::Handle L)
{
	luax::State S(L);
	AISrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	true_bool ret;
	AISrv->StartConversation(ret,arg1);
	S.push(bool(ret));
	return 1;
}

const luax::Registry AnimTextureService::Methods[] = {
	{"ChangeTexture",ChangeTexture},
	{NULL,NULL}
};
int AnimTextureService::ChangeTexture(luax::Handle L)
{
	luax::State S(L);
	AnimTextureSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	const char * arg2 = NULL;
	const char * arg3 = NULL;
	const char * arg4 = NULL;
	const char * arg5 = NULL;
	if (S.getTop() == 3)
	{
		arg3 = S.checkString(2,NULL);
		arg5 = S.checkString(3,NULL);
	}
	else
	{
		arg3 = S.checkString(3,NULL);
		arg5 = S.checkString(5,NULL);
		arg2 = S.optString(2,NULL,NULL);
		arg4 = S.optString(4,NULL,NULL);
	}
	long ret = AnimTextureSrv->ChangeTexture(arg1,arg2,arg3,arg4,arg5);
	S.push(ret == 0);
	return 0;
}

const luax::Registry BowService::Methods[] = {
	{"Equip",Equip},
	{"UnEquip",UnEquip},
	{"IsEquipped",IsEquipped},
	{"StartAttack",StartAttack},
	{"FinishAttack",FinishAttack},
	{"AbortAttack",AbortAttack},
	{"SetArrow",SetArrow},
	{NULL,NULL}
};
int BowService::Equip(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	long ret = BowSrv->Equip();
	S.push(ret != 0);
	return 1;
}
int BowService::UnEquip(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	long ret = BowSrv->UnEquip();
	S.push(ret != 0);
	return 1;
}
int BowService::IsEquipped(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	bool ret = BowSrv->IsEquipped();
	S.push(ret);
	return 1;
}
int BowService::StartAttack(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	long ret = BowSrv->StartAttack();
	S.push(ret != 0);
	return 1;
}
int BowService::FinishAttack(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	long ret = BowSrv->FinishAttack();
	S.push(ret != 0);
	return 1;
}
int BowService::AbortAttack(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	long ret = BowSrv->AbortAttack();
	S.push(ret != 0);
	return 1;
}
int BowService::SetArrow(luax::Handle L)
{
	luax::State S(L);
	BowSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int ret = BowSrv->SetArrow(arg1);
	S.push(ret != 0);
	return 1;
}

const luax::Registry ContainService::Methods[] = {
	{"Add",Add},
	{"Remove",Remove},
	{"MoveAllContents",MoveAllContents},
	{"StackAdd",StackAdd},
	{"IsHeld",IsHeld},
	{"GetContainer",GetContainer},
	{NULL,NULL}
};
const char* const ContainService::ContainsType[] = {
	"generic","belt","hand","alternate",NULL
};
int ContainService::Add(luax::Handle L)
{
	luax::State S(L);
	ContainSys.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	int arg4 = S.toBoolean(4);
	int arg3;
	if (S.getType(3) == luax::TString)
		arg3 = - S.checkOption(3,ContainsType);
	else
		arg3 = S.optInteger(3);
	long ret = ContainSys->Add(arg2,arg1,arg3,arg4);
	S.push(ret == 0);
	return 1;
}
int ContainService::Remove(luax::Handle L)
{
	luax::State S(L);
	ContainSys.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = ContainSys->Remove(arg2,arg1);
	S.push(ret == 0);
	return 1;
}
int ContainService::MoveAllContents(luax::Handle L)
{
	luax::State S(L);
	ContainSys.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	int arg3 = S.toBoolean(3);
	ContainSys->MoveAllContents(arg2,arg1,arg3);
	return 0;
}
int ContainService::StackAdd(luax::Handle L)
{
	luax::State S(L);
	ContainSys.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	long ret = ContainSys->StackAdd(arg1,arg2,2);
	S.push(ret);
	return 1;
}
int ContainService::IsHeld(luax::Handle L)
{
	luax::State S(L);
	ContainSys.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	int ret = ContainSys->IsHeld(arg1,arg2);
	if (ret == LONG_MAX)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int ContainService::GetContainer(luax::Handle L)
{
	luax::State S(L);
	ContainSys.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int ret = ContainSys->GetContainer(arg1);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}

const luax::Registry DamageService::Methods[] = {
	{"Damage",Damage},
	{"Slay",Slay},
	{"Resurrect",Resurrect},
	{"Terminate",Terminate},
	{NULL,NULL}
};
int DamageService::Damage(luax::Handle L)
{
	luax::State S(L);
	sDamage sd;
	DamageSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	sd.damage = S.checkInteger(3);
	sd.type = S.optInteger(4);
	unsigned long ret = DamageSys->DamageObject(arg1,arg2,&sd,NULL,0);
	S.push(luax::Integer(ret));
	return 1;
}
int DamageService::Slay(luax::Handle L)
{
	luax::State S(L);
	DamageSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	unsigned long ret = DamageSys->SlayObject(arg1,arg2,NULL);
	S.push(luax::Integer(ret));
	return 1;
}
int DamageService::Resurrect(luax::Handle L)
{
	luax::State S(L);
	DamageSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	unsigned long ret = DamageSys->ResurrectObject(arg1,arg2,NULL);
	S.push(luax::Integer(ret));
	return 1;
}
int DamageService::Terminate(luax::Handle L)
{
	luax::State S(L);
	DamageSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	unsigned long ret = DamageSys->TerminateObject(arg1,NULL);
	S.push(luax::Integer(ret));
	return 1;
}

const luax::Registry DarkGameService::Methods[] = {
	{"KillPlayer",KillPlayer},
	{"EndMission",EndMission},
	{"FadeToBlack",FadeToBlack},
	{NULL,NULL}
};
int DarkGameService::KillPlayer(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	long ret = DarkGameSrv->KillPlayer();
	S.push(ret == 0);
	return 1;
}
int DarkGameService::EndMission(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	long ret = DarkGameSrv->EndMission();
	S.push(ret == 0);
	return 1;
}
int DarkGameService::FadeToBlack(luax::Handle L)
{
	luax::State S(L);
	DarkGameSrv.set(g_pScriptManager);
	float arg1 = S.checkNumber(1);
	long ret = DarkGameSrv->FadeToBlack(arg1);
	S.push(ret == 0);
	return 1;
}

const luax::Registry DarkInvService::Methods[] = {
	{"CapabilityControl",CapabilityControl},
	{"AddSpeedControl",AddSpeedControl},
	{"RemoveSpeedControl",RemoveSpeedControl},
	{NULL,NULL}
};
int DarkInvService::CapabilityControl(luax::Handle L)
{
	static const char* const InvCap[] = {
		"cycle","worldfrob","worldfocus","invfrob",NULL
	};
	static const char* const InvControl[] = {
		"on","off","toggle",NULL
	};
	luax::State S(L);
	DarkInvSrv.set(g_pScriptManager);
	eDrkInvCap arg1 = eDrkInvCap(S.checkOption(1,InvCap));
	eDrkInvControl arg2 = eDrkInvControl(S.checkOption(2,InvControl,"toggle"));
	DarkInvSrv->CapabilityControl(arg1,arg2);
	return 0;
}
int DarkInvService::AddSpeedControl(luax::Handle L)
{
	luax::State S(L);
	DarkInvSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	float arg2 = S.checkNumber(2);
	float arg3 = S.optNumber(3,1.0);
	DarkInvSrv->AddSpeedControl(arg1,arg2,arg3);
	return 0;
}
int DarkInvService::RemoveSpeedControl(luax::Handle L)
{
	luax::State S(L);
	DarkInvSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	DarkInvSrv->RemoveSpeedControl(arg1);
	return 0;
}

const luax::Registry DarkPowerupsService::Methods[] = {
	{"TriggerWorldFlash",TriggerWorldFlash},
	{"ObjTryDeploy",ObjTryDeploy},
	{"CleanseBlood",CleanseBlood},
	{NULL,NULL}
};
int DarkPowerupsService::TriggerWorldFlash(luax::Handle L)
{
	luax::State S(L);
	DarkPowerupsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	DarkPowerupsSrv->TriggerWorldFlash(arg1);
	return 0;
}
int DarkPowerupsService::ObjTryDeploy(luax::Handle L)
{
	luax::State S(L);
	DarkPowerupsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	int ret = DarkPowerupsSrv->ObjTryDeploy(arg1,arg2);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int DarkPowerupsService::CleanseBlood(luax::Handle L)
{
	luax::State S(L);
	DarkPowerupsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	float arg2 = S.checkNumber(2);
	DarkPowerupsSrv->CleanseBlood(arg1,arg2);
	return 0;
}

const luax::Registry DarkUIService::Methods[] = {
	{"TextMessage",TextMessage},
	{"ReadBook",ReadBook},
	{"InvItem",InvItem},
	{"InvWeapon",InvWeapon},
	{"InvSelect",InvSelect},
	{"IsCommandBound",IsCommandBound},
	{"DescribeKeyBinding",DescribeKeyBinding},
	{NULL,NULL}
};
// TODO: use IInventory
int DarkUIService::TextMessage(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int arg2 = S.optInteger(2,-1001);
	int arg3;
	if (S.getType(3) == luax::TString)
		arg3 = strtocolor(S.asString(3));
	else
		arg3 = S.optInteger(3);
	DarkUISrv->TextMessage(arg1,arg3,arg2);
	return 0;
}
int DarkUIService::ReadBook(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	const char * arg2 = S.checkString(2,NULL);
	DarkUISrv->ReadBook(arg1,arg2);
	return 0;
}
int DarkUIService::InvItem(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	object ret;
	DarkUISrv->InvItem(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int DarkUIService::InvWeapon(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	object ret;
	DarkUISrv->InvWeapon(ret);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int DarkUIService::InvSelect(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = DarkUISrv->InvSelect(arg1);
	S.push(ret == 0);
	return 1;
}
int DarkUIService::IsCommandBound(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	cScrStr arg1 = S.checkString(1,NULL);
	true_bool ret;
	DarkUISrv->IsCommandBound(ret,arg1);
	S.push(bool(ret));
	return 1;
}
int DarkUIService::DescribeKeyBinding(luax::Handle L)
{
	luax::State S(L);
	DarkUISrv.set(g_pScriptManager);
	cScrStr arg1 = S.checkString(1,NULL);
	cScrStr ret;
	DarkUISrv->DescribeKeyBinding(ret,arg1);
	S.push(ret);
	ret.Free();
	return 1;
}

const luax::Registry DataService::Methods[] = {
	{"GetString",GetString},
	{"GetObjString",GetObjString},
	{"DirectRand",DirectRand},
	{"RandInt",RandInt},
	{"RandFlt0to1",RandFlt0to1},
	{"RandFltNeg1to1",RandFltNeg1to1},
	{NULL,NULL}
};
int DataService::GetString(luax::Handle L)
{
	luax::State S(L);
	DataSrv.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	const char * arg2 = S.checkString(2,NULL);
	const char * arg4 = S.optString(3,NULL,"strings");
	cScrStr ret;
	DataSrv->GetString(ret,arg1,arg2,"",arg4);
	S.push(ret);
	ret.Free();
	return 1;
}
int DataService::GetObjString(luax::Handle L)
{
	luax::State S(L);
	DataSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	cScrStr ret;
	DataSrv->GetObjString(ret,arg1,arg2);
	S.push(ret);
	ret.Free();
	return 1;
}
int DataService::DirectRand(luax::Handle L)
{
	luax::State S(L);
	int ret = std::rand() & 0x7FFF;
	S.push(ret);
	return 1;
}
int DataService::RandInt(luax::Handle L)
{
	luax::State S(L);
	int low,high;
	low = S.checkInteger(1);
	if (S.getTop() == 1)
	{
		high = low;
		low = 0;
	}
	else
		high = S.checkInteger(2);
	luax::Number r = luax::Number(std::rand()%RAND_MAX) / luax::Number(RAND_MAX);
	int ret;
	lua_number2integer(ret,std::floor(r * luax::Number(high-low+1)) + luax::Number(low));
	S.push(ret);
	return 1;
}
int DataService::RandFlt0to1(luax::Handle L)
{
	luax::State S(L);
	luax::Number r = (luax::Number(std::rand()) / luax::Number(RAND_MAX));
	S.push(r);
	return 1;
}
int DataService::RandFltNeg1to1(luax::Handle L)
{
	luax::State S(L);
	luax::Number r = luax::Number(std::rand()%RAND_MAX) / luax::Number(RAND_MAX);
	S.push(1.0 - (2.0 * r));
	return 1;
}

const luax::Registry DebugService::Methods[] = {
	{"MPrint",MPrint},
	{"Command",Command},
	{"Break",Break},
	{NULL,NULL}
};
int DebugService::MPrint(luax::Handle L)
{
	luax::State S(L);
	DebugSrv.set(g_pScriptManager);
	std::string arg[8];
	int num = S.getTop();
	if (num > 8) num = 8;
	for (int i=0; i<num; i++)
		arg[i] = S.toString(i+1);
	DebugSrv->MPrint(arg[0].c_str(),arg[1].c_str(),arg[2].c_str(),arg[3].c_str(),arg[4].c_str(),arg[5].c_str(),arg[6].c_str(),arg[7].c_str());
	return 0;
}
int DebugService::Command(luax::Handle L)
{
	luax::State S(L);
	DebugSrv.set(g_pScriptManager);
	cScrStr cmd = S.checkString(1,NULL);
	std::string arg[7];
	int num = S.getTop();
	if (num > 8) num = 8;
	for (int i=1; i<num; i++)
		arg[i-1] = S.toString(i+1);
	DebugSrv->MPrint(cmd,arg[0].c_str(),arg[1].c_str(),arg[2].c_str(),arg[3].c_str(),arg[4].c_str(),arg[5].c_str(),arg[6].c_str());
	return 0;
}
int DebugService::Break(luax::Handle L)
{
	luax::State S(L);
	DebugSrv.set(g_pScriptManager);
	DebugSrv->Break();
	return 0;
}

const luax::Registry DoorService::Methods[] = {
	{"CloseDoor",CloseDoor},
	{"OpenDoor",OpenDoor},
	{"GetDoorState",GetDoorState},
	{"ToggleDoor",ToggleDoor},
	{NULL,NULL}
};
int DoorService::CloseDoor(luax::Handle L)
{
	luax::State S(L);
	DoorSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = DoorSrv->CloseDoor(arg1);
	S.push(ret);
	return 1;
}
int DoorService::OpenDoor(luax::Handle L)
{
	luax::State S(L);
	DoorSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = DoorSrv->OpenDoor(arg1);
	S.push(ret);
	return 1;
}
int DoorService::GetDoorState(luax::Handle L)
{
	luax::State S(L);
	DoorSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int ret = DoorSrv->GetDoorState(arg1);
	if (ret == kDoorStateInvalid)
		S.push(luax::Nil());
	else
		S.push(ret);
	// FIXME should be string
	return 1;
}
int DoorService::ToggleDoor(luax::Handle L)
{
	luax::State S(L);
	DoorSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = DoorSrv->ToggleDoor(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry KeyService::Methods[] = {
	{"TryToUseKey",TryToUseKey},
	{NULL,NULL}
};
int KeyService::TryToUseKey(luax::Handle L)
{
	static const char* const KeyUsage[] = {
		"default","open","close","check",NULL
	};
	luax::State S(L);
	KeySys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	eKeyUse arg3 = eKeyUse(S.checkOption(3,KeyUsage,"default"));
	bool ret = KeySys->TryToUseKey(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}

const luax::Registry LightService::Methods[] = {
	{"Set",Set},
	{"SetMode",SetMode},
	{"Activate",Activate},
	{"Deactivate",Deactivate},
	{"Subscribe",Subscribe},
	{"Unsubscribe",Unsubscribe},
	{"GetMode",GetMode},
	{"GetModeName",GetModeName},
	{NULL,NULL}
};
const char* const LightService::LightMode[] = {
	"flipminmax","slidesmoothly","random",
	"minbrightness","maxbrightness","zerobrightness",
	"smoothlybrighten","smoothlydim",
	"randombutcoherent","flickerminmax",
	NULL
};
int LightService::Set(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,LightMode);
	float arg3 = S.checkNumber(3);
	float arg4 = S.checkNumber(4);
	LightSrv->Set(arg1,arg2,arg3,arg4);
	return 0;
}
int LightService::SetMode(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkOption(2,LightMode);
	LightSrv->SetMode(arg1,arg2);
	return 0;
}
int LightService::Activate(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	LightSrv->Activate(arg1);
	return 0;
}
int LightService::Deactivate(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	LightSrv->Deactivate(arg1);
	return 0;
}
int LightService::Subscribe(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	LightSrv->Subscribe(arg1);
	return 0;
}
int LightService::Unsubscribe(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	LightSrv->Unsubscribe(arg1);
	return 0;
}
int LightService::GetMode(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int ret = LightSrv->GetMode(arg1);
	if (ret == -1)
		S.push(luax::Nil());
	else if (size_t(ret) < (sizeof(LightMode)/sizeof(LightMode[0]))
		&& LightMode[ret] != NULL)
		S.push(LightMode[ret]);
	else
		S.push(ret);
	return 1;
}
int LightService::GetModeName(luax::Handle L)
{
	luax::State S(L);
	LightSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (arg1 >= 0
	 && size_t(arg1) < (sizeof(LightMode)/sizeof(LightMode[0]))
	 && LightMode[arg1] != NULL)
	{
		S.push(LightMode[arg1]);
	}
	else
		S.push(luax::Nil());
	return 1;
}

const luax::Registry LinkService::Methods[] = {
	{"Create",Create},
	{"Destroy",Destroy},
	{"AnyExist",AnyExist},
	{"GetAll",GetAll},
	{"GetOne",GetOne},
	{"BroadcastOnAllLinks",BroadcastOnAllLinks},
	{"CreateMany",CreateMany},
	{"DestroyMany",DestroyMany},
	{"GetAllInherited",GetAllInherited},
	{"GetAllInheritedSingle",GetAllInheritedSingle},
	{"LinkKindNamed",LinkKindNamed},
	{"LinkKindName",LinkKindName},
	{"LinkGet",LinkGet},
	{"LinkGetData",LinkGetData},
	{"LinkSetData",LinkSetData},
	{NULL,NULL}
};
short LinkService::GetRelationNamed(const char * name)
{
	SInterface<IRelation> rel(LinkMan->GetRelationNamed(name));
	if (!rel)
		return 0;
	return rel->GetID();
}
int LinkService::Create(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	int arg2 = S.checkInteger(2);
	int arg3 = S.checkInteger(3);
	short arg1;
	if (S.getType(1) == luax::TString)
	{
		arg1 = GetRelationNamed(S.asString(1));
	}
	else
		arg1 = S.checkInteger(1);
	if (!arg1)
		S.argError(1,"invalid link flavor");
	long ret = LinkMan->Add(arg2,arg3,arg1);
	if (!ret)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int LinkService::Destroy(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	link arg1 = S.checkInteger(1);
	long ret = LinkMan->Remove(arg1);
	S.push(ret != 0);
	return 1;
}
int LinkService::AnyExist(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	int arg2 = S.optInteger(2);
	int arg3 = S.optInteger(3);
	short arg1 = 0;
	if (S.getType(1) == luax::TString)
	{
		const char * str1 = S.asString(1);
		if (str1[0] != '\0')
		{
			arg1 = GetRelationNamed(str1);
			if (!arg1)
				S.argError(1,"invalid link flavor");
		}
	}
	else
		arg1 = S.checkInteger(1);
	SInterface<ILinkQuery> q(LinkMan->Query(arg2,arg3,arg1));
	bool ret = q && !q->Done();
	S.push(ret);
	return 1;
}
int LinkService::GetAll(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	int arg2 = S.optInteger(2);
	int arg3 = S.optInteger(3);
	short arg1 = 0;
	if (S.getType(1) == luax::TString)
	{
		const char * str1 = S.asString(1);
		if (str1[0] != '\0')
		{
			arg1 = GetRelationNamed(str1);
			if (!arg1)
				S.argError(1,"invalid link flavor");
		}
	}
	else
		arg1 = S.checkInteger(1);
	SInterface<ILinkQuery> ret(LinkMan->Query(arg2,arg3,arg1));
	new(S) LinkSet(ret);
	return 1;
}
int LinkService::GetOne(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	int arg2 = S.optInteger(2);
	int arg3 = S.optInteger(3);
	short arg1 = 0;
	if (S.getType(1) == luax::TString)
	{
		const char * str1 = S.asString(1);
		if (str1[0] != '\0')
		{
			arg1 = GetRelationNamed(str1);
			if (!arg1)
				S.argError(1,"invalid link flavor");
		}
	}
	else
		arg1 = S.checkInteger(1);
	SInterface<ILinkQuery> q(LinkMan->Query(arg2,arg3,arg1));
	if (q && !q->Done())
	{
		long ret = q->ID();
		S.push(ret);
	}
	else
		S.push(luax::Nil());
	return 1;
}
int LinkService::BroadcastOnAllLinks(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	if (arg2[0] == '\0')
		S.argError(2,"invalid script message");
	short arg3 = 0;
	if (S.getType(3) == luax::TString)
	{
		const char * str3 = S.asString(3);
		if (str3[0] != '\0')
		{
			arg3 = GetRelationNamed(str3);
			if (!arg3)
				S.argError(3,"invalid link flavor");
		}
	}
	else
		arg3 = S.checkInteger(3);
	if (S.isNoneOrNil(4))
	{
		SInterface<ILinkQuery> q(LinkMan->Query(arg1,0,arg3));
		sLink sl;
		for (; !q->Done(); q->Next())
		{
			q->Link(&sl);
			ScriptModule::PostMessage(sl.source,sl.dest,arg2,
					cMultiParm::Undef,cMultiParm::Undef,cMultiParm::Undef,kScrMsgPostToOwner);
		}
	}
	else if (S.isFunction(4))
	{
		SInterface<ILinkQuery> q(LinkMan->Query(arg1,0,arg3));
		sLink sl;
		for (; !q->Done(); q->Next())
		{
			q->Link(&sl);
			S.copy(4)
			 .push(luax::Integer(sl.dest))
			 .push(luax::Integer(sl.source))
			 .push(luax::Integer(sl.flavor))
			 .push(q->ID())
			 .call(4,1);
			if (!S.isNil())
			{
				ScriptMultiParm ret(S);
				ScriptModule::PostMessage(sl.source,sl.dest,arg2,ret,
						cMultiParm::Undef,cMultiParm::Undef,kScrMsgPostToOwner);
			}
			S.pop();
		}
	}
	else
	{
		SInterface<IStructDescTools> StructTools(g_pScriptManager);
		SInterface<IRelation> rel(LinkMan->GetRelation(arg3));
		const sStructDesc * sd = NULL;
		if (rel->DescribeData()->uiTypeSize > 0)
			sd = StructTools->Lookup(rel->DescribeData()->szTypeName);
		if (sd && StructTools->IsSimple(sd))
		{
			// Filter links based on data.
			char buf[128];
			const char * arg4;
			if (S.getType(4) == luax::TBoolean)
				arg4 = S.toBoolean(4) ? "TRUE" : "FALSE";
			else
				arg4 = S.asString(4);
			SInterface<ILinkQuery> q(LinkMan->Query(arg1,0,arg3));
			sLink sl;
			for (; !q->Done(); q->Next())
			{
				void * p = q->Data();
				q->Link(&sl);
				if (p)
				{
					buf[0] = '\0';
					StructTools->UnparseSimple(sd,p,buf,sizeof(buf));
					if (0 == strcmp(buf,arg4))
						ScriptModule::PostMessage(sl.source,
								sl.dest,arg2,
								arg4,
								cMultiParm::Undef,
								cMultiParm::Undef,
								kScrMsgPostToOwner);
				}
				else
				{
					if (arg4[0] == '\0')
						ScriptModule::PostMessage(sl.source,
								sl.dest,arg2,
								arg4,
								cMultiParm::Undef,
								cMultiParm::Undef,
								kScrMsgPostToOwner);
				}
			}
		}
		else
		{
			// Send extra data with message, but don't filter links.
			ScriptMultiParm mp4(S); cMultiParm const& arg4 = mp4.pop(4);
			SInterface<ILinkQuery> q(LinkMan->Query(arg1,0,arg3));
			sLink sl;
			for (; !q->Done(); q->Next())
			{
				q->Link(&sl);
				ScriptModule::PostMessage(sl.source,sl.dest,arg2,arg4,
						cMultiParm::Undef,cMultiParm::Undef,kScrMsgPostToOwner);
			}
		}
	}
	return 0;
}
int LinkService::CreateMany(luax::Handle L)
{
	luax::State S(L);
	LinkSrv.set(g_pScriptManager);
	cScrStr arg2 = S.checkString(2,NULL);
	cScrStr arg3 = S.checkString(3,NULL);
	linkkind arg1;
	if (S.getType(1) == luax::TString)
	{
		LinkMan.set(g_pScriptManager);
		arg1 = GetRelationNamed(S.asString(1));
	}
	else
		arg1 = S.checkInteger(1);
	if (!arg1)
		S.argError(1,"invalid link flavor");
	long ret = LinkSrv->CreateMany(arg1,arg2,arg3);
	S.push(ret == 0);
	return 1;
}
int LinkService::DestroyMany(luax::Handle L)
{
	luax::State S(L);
	LinkSrv.set(g_pScriptManager);
	const cScrStr arg2 = S.checkString(2,NULL);
	const cScrStr arg3 = S.checkString(3,NULL);
	linkkind arg1;
	if (S.getType(1) == luax::TString)
	{
		LinkMan.set(g_pScriptManager);
		const char * str1 = S.asString(1);
		if (str1[0] != '\0')
		{
			arg1 = GetRelationNamed(str1);
			if (!arg1)
				S.argError(1,"invalid link flavor");
		}
	}
	else
		arg1 = S.checkInteger(1);
	long ret = LinkSrv->DestroyMany(arg1,arg2,arg3);
	S.push(ret == 0);
	return 1;
}
int LinkService::GetAllInherited(luax::Handle L)
{
	luax::State S(L);
	LinkSrv.set(g_pScriptManager);
	object arg2 = S.checkInteger(2);
	object arg3 = S.checkInteger(3);
	linkkind arg1;
	if (S.getType(1) == luax::TString)
	{
		LinkMan.set(g_pScriptManager);
		const char * str1 = S.asString(1);
		if (str1[0] != '\0')
		{
			arg1 = GetRelationNamed(str1);
			if (!arg1)
				S.argError(1,"invalid link flavor");
		}
	}
	else
		arg1 = S.checkInteger(1);
	if (!arg2)
		S.argError(2,"invalid object");
	if (!arg3)
		S.argError(3,"invalid object");
	linkset ret;
	LinkSrv->GetAllInherited(ret,arg1,arg2,arg3);
	new(S) LinkSet(ret);
	return 1;
}
int LinkService::GetAllInheritedSingle(luax::Handle L)
{
	luax::State S(L);
	LinkSrv.set(g_pScriptManager);
	object arg2 = S.checkInteger(2);
	object arg3 = S.optInteger(3);
	linkkind arg1;
	if (S.getType(1) == luax::TString)
	{
		LinkMan.set(g_pScriptManager);
		const char * str1 = S.asString(1);
		if (str1[0] != '\0')
		{
			arg1 = GetRelationNamed(str1);
			if (!arg1)
				S.argError(1,"invalid link flavor");
		}
	}
	else
		arg1 = S.checkInteger(1);
	if (!arg2)
		S.argError(2,"invalid object");
	linkset ret;
	LinkSrv->GetAllInheritedSingle(ret,arg1,arg2,arg3);
	new(S) LinkSet(ret);
	return 1;
}
int LinkService::LinkKindNamed(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	long ret = GetRelationNamed(arg1);
	if (!ret)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int LinkService::LinkKindName(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	short arg1 = S.checkInteger(1);
	SInterface<IRelation> rel(LinkMan->GetRelation(arg1));
	if (rel)
		S.push(rel->Describe()->szName);
	else
		S.push(luax::Nil());
	return 1;
}
int LinkService::LinkGet(luax::Handle L)
{
	luax::State S(L);
	LinkMan.set(g_pScriptManager);
	long arg1 = S.checkInteger(1);
	sLink ret;
	ret.source = ret.dest = 0;
	ret.flavor = 0;
	if (LinkMan->Get(arg1,&ret))
	{
		S.push(luax::Integer(ret.dest))
		 .push(luax::Integer(ret.source))
		 .push(luax::Integer(ret.flavor));
		return 3;
	}
	S.push(luax::Nil());
	return 1;
}
int LinkService::LinkGetData(luax::Handle L)
{
	luax::State S(L);
	LinkToolsSrv.set(g_pScriptManager);
	long arg1 = S.checkInteger(1);
	const char * arg2 = S.optString(2,NULL,NULL);
	cMultiParm ret;
	LinkToolsSrv->LinkGetData(ret,arg1,arg2);
	ScriptMultiParm(S).push(ret);
	return 1;
}
int LinkService::LinkSetData(luax::Handle L)
{
	luax::State S(L);
	LinkToolsSrv.set(g_pScriptManager);
	long arg1 = S.checkInteger(1);
	if (S.getTop() >= 3)
	{
		const char * arg2 = S.optString(2,NULL,NULL);
		ScriptMultiParm mp3(S); cMultiParm const& arg3 = mp3.pop(3);
		LinkToolsSrv->LinkSetData(arg1,arg2,arg3);
	}
	else
	{
		S.checkAny(2);
		ScriptMultiParm mp2(S); cMultiParm const& arg2 = mp2.pop(2);
		LinkToolsSrv->LinkSetData(arg1,NULL,arg2);
	}
	return 0;
}

const luax::Registry LockService::Methods[] = {
	{"IsLocked",IsLocked},
	{NULL,NULL}
};
int LockService::IsLocked(luax::Handle L)
{
	luax::State S(L);
	LockSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = LockSrv->IsLocked(arg1);
	S.push(ret);
	return 1;
}

int ObjectService::BeginCreate(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (ObjectSys->Exists(arg1))
	{
		int ret = ObjectSys->BeginCreate(arg1,1);
		if (ret != 0)
			S.push(ret);
		else
			S.push(luax::Nil());
	}
	else
		S.push(luax::Nil());
	return 1;
}
int ObjectService::EndCreate(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	long ret = ObjectSys->EndCreate(arg1);
	S.push(ret == 0);
	return 1;
}
int ObjectService::Create(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	if (ObjectSys->Exists(arg1))
	{
		int ret = ObjectSys->Create(arg1,1);
		if (ret != 0)
			S.push(ret);
		else
			S.push(luax::Nil());
	}
	else
		S.push(luax::Nil());
	return 1;
}
int ObjectService::Destroy(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	long ret = ObjectSys->Destroy(arg1);
	S.push(ret == 0);
	return 1;
}
int ObjectService::Exists(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = ObjectSys->Exists(arg1);
	S.push(ret);
	return 1;
}
int ObjectService::SetName(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	ObjectSys->NameObject(arg1,arg2);
	return 0;
}
int ObjectService::GetName(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * ret = ObjectSys->GetName(arg1);
	if (ret)
		S.push(ret);
	else
		S.push(luax::Nil());
	return 1;
}
int ObjectService::Named(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int ret = ObjectSys->GetObjectNamed(arg1);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int ObjectService::AddMetaProperty(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ObjectSrv->AddMetaProperty(arg1,arg2);
	return 0;
}
int ObjectService::RemoveMetaProperty(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	ObjectSrv->RemoveMetaProperty(arg1,arg2);
	return 0;
}
int ObjectService::HasMetaProperty(luax::Handle L)
{
	luax::State S(L);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	bool ret = TraitMan->ObjHasDonorIntrinsically(arg1,arg2);
	S.push(ret);
	return 1;
}
int ObjectService::InheritsFrom(luax::Handle L)
{
	luax::State S(L);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	bool ret = TraitMan->ObjHasDonor(arg1,arg2);
	S.push(ret);
	return 1;
}
int ObjectService::IsTransient(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = ObjectSys->ObjIsTransient(arg1);
	S.push(ret);
	return 1;
}
int ObjectService::SetTransience(luax::Handle L)
{
	luax::State S(L);
	ObjectSys.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	S.checkAny(2);
	int arg2 = S.toBoolean(2);
	ObjectSys->SetObjTransience(arg1,arg2);
	return 0;
}
int ObjectService::Position(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrVec ret;
	ObjectSrv->Position(ret,arg1);
	lmod_newvector(L, ret.x, ret.y, ret.z);
	return 1;
}
int ObjectService::Facing(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrVec ret;
	ObjectSrv->Facing(ret,arg1);
	lmod_newvector(L, ret.x, ret.y, ret.z);
	return 1;
}
int ObjectService::Teleport(luax::Handle L)
{
	luax::State S(L);
	ObjectSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrVec arg2,arg3;
	object arg4 = 0;
	if (S.getType(2) == luax::TNumber)
	{
		arg4 = S.toInteger(2);
	}
	else if (S.isNoneOrNil(2))
	{
		arg4 = S.checkInteger(4);
		if (!S.isNoneOrNil(3))
		{
			const lvector* v3 = reinterpret_cast<const lvector*>(
						S.checkUserdata(3,LVECTOR_NAME));
			arg3.x = v3->v.x; arg3.y = v3->v.y; arg3.z = v3->v.z;
		}
	}
	else
	{
		const lvector* v2 = reinterpret_cast<const lvector*>(
					S.checkUserdata(2,LVECTOR_NAME));
		arg2.x = v2->v.x; arg2.y = v2->v.y; arg2.z = v2->v.z;
		if (!S.isNoneOrNil(3))
		{
			const lvector* v3 = reinterpret_cast<const lvector*>(
						S.checkUserdata(3,LVECTOR_NAME));
			arg3.x = v3->v.x; arg3.y = v3->v.y; arg3.z = v3->v.z;
		}
		arg4 = S.optInteger(4);
	}
	ObjectSrv->Teleport(arg1,arg2,arg3,arg4);
	return 0;
}
int ObjectService::GetArchetype(luax::Handle L)
{
	luax::State S(L);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int ret = TraitMan->GetArchetype(arg1);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(ret);
	return 1;
}
int ObjectService::IsArchetype(luax::Handle L)
{
	luax::State S(L);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = TraitMan->IsArchetype(arg1);
	S.push(ret);
	return 1;
}
int ObjectService::IsMetaProperty(luax::Handle L)
{
	luax::State S(L);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = TraitMan->IsMetaProperty(arg1);
	S.push(ret);
	return 1;
}
int ObjectService::IsDonor(luax::Handle L)
{
	luax::State S(L);
	TraitMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = TraitMan->ObjIsDonor(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry PGroupService::Methods[] = {
	{"SetActive",SetActive},
	{NULL,NULL}
};
int PGroupService::SetActive(luax::Handle L)
{
	luax::State S(L);
	PGroupSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	S.checkAny(2);
	int arg2 = S.toBoolean(2);
	PGroupSrv->SetActive(arg1,arg2);
	return 0;
}

const char* const PhysService::PhysMessages[] = {
	"collision","contact","enterexit","fellasleep",NULL
};
int PhysService::SubscribeMsg(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkFlags(2,PhysMessages);
	PhysSrv->SubscribeMsg(arg1,arg2);
	return 0;
}
int PhysService::UnsubscribeMsg(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	int arg2 = S.checkFlags(2,PhysMessages);
	PhysSrv->UnsubscribeMsg(arg1,arg2);
	return 0;
}
int PhysService::LaunchProjectile(luax::Handle L)
{
	static const char* const EmitFlags[] = {
		"nophysics","pushout","relativevelocity","gravity",NULL
	};
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	float arg3 = S.optNumber(3,1.0);
	int arg4 = S.checkFlags(4,EmitFlags,"");
	cScrVec arg5;
	if (!S.isNoneOrNil(5))
	{
		const lvector* v5 = reinterpret_cast<const lvector*>(
					S.checkUserdata(5,LVECTOR_NAME));
		arg5.x = v5->v.x; arg5.y = v5->v.y; arg5.z = v5->v.z;
	}
	object ret;
	PhysSrv->LaunchProjectile(ret,arg1,arg2,arg3,arg4,arg5);
	if (ret == 0)
		S.push(luax::Nil());
	else
		S.push(luax::Integer(ret));
	return 1;
}
int PhysService::SetVelocity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrVec arg2;
	const lvector* v2 = reinterpret_cast<const lvector*>(
				S.checkUserdata(2,LVECTOR_NAME));
	arg2.x = v2->v.x; arg2.y = v2->v.y; arg2.z = v2->v.z;
	PhysSrv->SetVelocity(arg1,arg2);
	return 0;
}
int PhysService::GetVelocity(luax::Handle L)
{
	luax::State S(L);
	PhysSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	cScrVec ret;
	PhysSrv->GetVelocity(arg1,ret);
	lmod_newvector(L, ret.x, ret.y, ret.z);
	return 1;
}

const luax::Registry PickLockService::Methods[] = {
	{"Ready",Ready},
	{"UnReady",UnReady},
	{"StartPicking",StartPicking},
	{"FinishPicking",FinishPicking},
	{"CheckPick",CheckPick},
	{"DirectMotion",DirectMotion},
	{NULL,NULL}
};
int PickLockService::Ready(luax::Handle L)
{
	luax::State S(L);
	PickLockSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	bool ret = PickLockSrv->Ready(arg1,arg2);
	S.push(ret);
	return 1;
}
int PickLockService::UnReady(luax::Handle L)
{
	luax::State S(L);
	PickLockSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	bool ret = PickLockSrv->UnReady(arg1,arg2);
	S.push(ret);
	return 1;
}
int PickLockService::StartPicking(luax::Handle L)
{
	luax::State S(L);
	PickLockSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	object arg3 = S.checkInteger(3);
	bool ret = PickLockSrv->StartPicking(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}
int PickLockService::FinishPicking(luax::Handle L)
{
	luax::State S(L);
	PickLockSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	bool ret = PickLockSrv->FinishPicking(arg1);
	S.push(ret);
	return 1;
}
int PickLockService::CheckPick(luax::Handle L)
{
	luax::State S(L);
	PickLockSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	int arg3 = S.optInteger(3);
	int ret = PickLockSrv->CheckPick(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}
int PickLockService::DirectMotion(luax::Handle L)
{
	luax::State S(L);
	PickLockSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	bool ret = PickLockSrv->DirectMotion(arg1);
	S.push(ret);
	return 1;
}

const luax::Registry PlayerLimbsService::Methods[] = {
	{"Equip",Equip},
	{"UnEquip",UnEquip},
	{"StartUse",StartUse},
	{"FinishUse",FinishUse},
	{NULL,NULL}
};
int PlayerLimbsService::Equip(luax::Handle L)
{
	luax::State S(L);
	PlayerLimbsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = PlayerLimbsSrv->Equip(arg1);
	S.push(ret != 0);
	return 1;
}
int PlayerLimbsService::UnEquip(luax::Handle L)
{
	luax::State S(L);
	PlayerLimbsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = PlayerLimbsSrv->UnEquip(arg1);
	S.push(ret != 0);
	return 1;
}
int PlayerLimbsService::StartUse(luax::Handle L)
{
	luax::State S(L);
	PlayerLimbsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = PlayerLimbsSrv->StartUse(arg1);
	S.push(ret != 0);
	return 1;
}
int PlayerLimbsService::FinishUse(luax::Handle L)
{
	luax::State S(L);
	PlayerLimbsSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = PlayerLimbsSrv->FinishUse(arg1);
	S.push(ret != 0);
	return 1;
}

int PropertyService::Add(luax::Handle L)
{
	luax::State S(L);
	PropertyMan.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	if (arg1 && arg2)
	{
		SInterface<IProperty> p(PropertyMan->GetPropertyNamed(arg2));
		if (p->GetID() != -1)
		{
			long ret = p->Create(arg1);
			S.push(ret == 0);
		}
		else
			S.push(false);
	}
	else
		S.push(false);
	return 1;
}
int PropertyService::Remove(luax::Handle L)
{
	luax::State S(L);
	PropertyMan.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	if (arg1 && arg2)
	{
		SInterface<IProperty> p(PropertyMan->GetPropertyNamed(arg2));
		if (p->GetID() != -1)
		{
			long ret = p->Delete(arg1);
			S.push(ret == 0);
		}
		else
			S.push(false);
	}
	else
		S.push(false);
	return 1;
}
int PropertyService::CopyFrom(luax::Handle L)
{
	luax::State S(L);
	PropertyMan.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	object arg3 = S.checkInteger(3);
	if (arg1 && arg3 && arg2)
	{
		SInterface<IProperty> p(PropertyMan->GetPropertyNamed(arg2));
		if (p->GetID() != -1)
		{
			long ret = p->Copy(arg1,arg3);
			S.push(ret == 0);
		}
		else
			S.push(false);
	}
	else
		S.push(false);
	return 1;
}
int PropertyService::Possessed(luax::Handle L)
{
	luax::State S(L);
	PropertyMan.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	if (arg1 && arg2)
	{
		SInterface<IProperty> p(PropertyMan->GetPropertyNamed(arg2));
		if (p->GetID() != -1)
		{
			bool ret = p->IsRelevant(arg1);
			S.push(ret);
		}
		else
			S.push(false);
	}
	else
		S.push(false);
	return 1;
}

const luax::Registry PuppetService::Methods[] = {
	{"PlayMotion",PlayMotion},
	{NULL,NULL}
};
int PuppetService::PlayMotion(luax::Handle L)
{
	luax::State S(L);
	PuppetSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	true_bool ret;
	PuppetSrv->PlayMotion(ret,arg1,arg2);
	S.push(bool(ret));
	return 1;
}

const luax::Registry QuestService::Methods[] = {
	{"SubscribeMsg",SubscribeMsg},
	{"UnsubscribeMsg",UnsubscribeMsg},
	{"Set",Set},
	{"Get",Get},
	{"Exists",Exists},
	{"Delete",Delete},
	{NULL,NULL}
};
const char* const QuestService::QuestType[] = {
	"mission","campaign","any",NULL
};
int QuestService::SubscribeMsg(luax::Handle L)
{
	luax::State S(L);
	QuestData.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	int arg3 = S.checkOption(3,QuestType,"any");
	QuestData->SubscribeMsg(arg1,arg2,arg3);
	return 0;
}
int QuestService::UnsubscribeMsg(luax::Handle L)
{
	luax::State S(L);
	QuestData.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	QuestData->UnsubscribeMsg(arg1,arg2);
	return 0;
}
int QuestService::Set(luax::Handle L)
{
	luax::State S(L);
	QuestData.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int arg2 = S.checkInteger(2);
	int arg3;
	if (S.getType(3) == luax::TString)
	{
		arg3 = S.checkOption(3,QuestType);
		if (arg3 == 2)
			arg3 = 0;
	}
	else
		arg3 = S.toBoolean(3);
	if (!QuestData->Exists(arg1))
		QuestData->Create(arg1,arg2,arg3);
	else
		QuestData->Set(arg1,arg2);
	return 0;
}
int QuestService::Get(luax::Handle L)
{
	luax::State S(L);
	QuestData.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	int ret = QuestData->Get(arg1);
	S.push(ret);
	return 1;
}
int QuestService::Exists(luax::Handle L)
{
	luax::State S(L);
	QuestData.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	bool ret = QuestData->Exists(arg1);
	S.push(ret);
	return 1;
}
int QuestService::Delete(luax::Handle L)
{
	luax::State S(L);
	QuestData.set(g_pScriptManager);
	const char * arg1 = S.checkString(1,NULL);
	QuestData->Delete(arg1);
	return 0;
}

const char* const SoundService::EnvSoundLoc[] = {
	"onobj","atobjloc","ambient",NULL
};
const char* const SoundService::SoundNetwork[] = {
	"normal","broadcast","localonly",NULL
};

const luax::Registry WeaponService::Methods[] = {
	{"Equip",Equip},
	{"UnEquip",UnEquip},
	{"IsEquipped",IsEquipped},
	{"StartAttack",StartAttack},
	{"FinishAttack",FinishAttack},
	{NULL,NULL}
};
int WeaponService::Equip(luax::Handle L)
{
	luax::State S(L);
	WeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	eWeaponType arg2 = S.toBoolean(2) ? kWeaponBlackjack : kWeaponSword;
	long ret = WeaponSrv->Equip(arg1,arg2);
	S.push(ret != 0);
	return 1;
}
int WeaponService::UnEquip(luax::Handle L)
{
	luax::State S(L);
	WeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	long ret = WeaponSrv->UnEquip(arg1);
	S.push(ret != 0);
	return 1;
}
int WeaponService::IsEquipped(luax::Handle L)
{
	luax::State S(L);
	WeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = WeaponSrv->IsEquipped(arg1,arg2);
	S.push(ret != 0);
	return 1;
}
int WeaponService::StartAttack(luax::Handle L)
{
	luax::State S(L);
	WeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = WeaponSrv->StartAttack(arg1,arg2);
	S.push(ret != 0);
	return 1;
}
int WeaponService::FinishAttack(luax::Handle L)
{
	luax::State S(L);
	WeaponSrv.set(g_pScriptManager);
	object arg1 = S.checkInteger(1);
	object arg2 = S.checkInteger(2);
	long ret = WeaponSrv->FinishAttack(arg1,arg2);
	S.push(ret != 0);
	return 1;
}

const luax::Registry DarkHookService::Methods[] = {
	{"DarkHookInitialize",DarkHookInitialize},
	{"InstallPropHook",InstallPropHook},
	{"UninstallPropHook",UninstallPropHook},
	{"InstallRelHook",InstallRelHook},
	{"UninstallRelHook",UninstallRelHook},
	{"InstallObjHook",InstallObjHook},
	{"UninstallObjHook",UninstallObjHook},
	{"InstallHierarchyHook",InstallHierarchyHook},
	{"UninstallHierarchyHook",UninstallHierarchyHook},
	{NULL,NULL}
};
DarkHookService::DarkHookInitState DarkHookService::sm_DarkHookInitialized = kDH_Uninitialized;
int DarkHookService::DarkHookInitialize(luax::Handle L)
{
	luax::State S(L);
	switch (sm_DarkHookInitialized)
	{
	case kDH_Initialized: S.push(true); break;
	case kDH_Failed: S.push(false); break;
	case kDH_Uninitialized:
	{
		bool ret = DarkHookInitializeService(g_pScriptManager, g_pMalloc);
		sm_DarkHookInitialized = ret ? kDH_Initialized : kDH_Failed;
		S.push(ret);
		break;
	}
	}
	return 1;
}
void DarkHookService::DHInit(luax::State& S)
{
	switch (sm_DarkHookInitialized)
	{
	case kDH_Initialized: break;
	case kDH_Uninitialized:
		if (DarkHookInitializeService(g_pScriptManager, g_pMalloc))
			break;
	case kDH_Failed:
		S.error("cannot initialize Dark Hook service");
	}
}
int DarkHookService::InstallPropHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	S.checkAny(2);
	eDHRegisterFlags arg2 = S.toBoolean(2) ? kDHNotifyDefault : kDHNotifyAsync;
	const char * arg3 = S.checkString(3,NULL);
	int arg4 = S.checkInteger(4);
	bool ret = DarkHookSrv->InstallPropHook(arg1,arg2,arg3,arg4);
	S.push(ret);
	return 1;
}
int DarkHookService::UninstallPropHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	int arg3 = S.checkInteger(3);
	DarkHookSrv->UninstallPropHook(arg1,arg2,arg3);
	return 0;
}
int DarkHookService::InstallRelHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	S.checkAny(2);
	eDHRegisterFlags arg2 = S.toBoolean(2) ? kDHNotifyDefault : kDHNotifyAsync;
	const char * arg3 = S.checkString(3,NULL);
	int arg4 = S.checkInteger(4);
	bool ret = DarkHookSrv->InstallRelHook(arg1,arg2,arg3,arg4);
	S.push(ret);
	return 1;
}
int DarkHookService::UninstallRelHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	const char * arg2 = S.checkString(2,NULL);
	int arg3 = S.checkInteger(3);
	DarkHookSrv->UninstallRelHook(arg1,arg2,arg3);
	return 0;
}
int DarkHookService::InstallObjHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	S.checkAny(2);
	eDHRegisterFlags arg2 = S.toBoolean(2) ? kDHNotifyDefault : kDHNotifyAsync;
	int arg3 = S.checkInteger(3);
	bool ret = DarkHookSrv->InstallObjHook(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}
int DarkHookService::UninstallObjHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	DarkHookSrv->UninstallObjHook(arg1,arg2);
	return 0;
}
int DarkHookService::InstallHierarchyHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	S.checkAny(2);
	eDHRegisterFlags arg2 = S.toBoolean(2) ? kDHNotifyDefault : kDHNotifyAsync;
	int arg3 = S.checkInteger(3);
	bool ret = DarkHookSrv->InstallHierarchyHook(arg1,arg2,arg3);
	S.push(ret);
	return 1;
}
int DarkHookService::UninstallHierarchyHook(luax::Handle L)
{
	luax::State S(L);
	DHInit(S);
	DarkHookSrv.set(g_pScriptManager);
	int arg1 = S.checkInteger(1);
	int arg2 = S.checkInteger(2);
	DarkHookSrv->UninstallHierarchyHook(arg1,arg2);
	return 0;
}

}
