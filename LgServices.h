/******************************************************************************
 *  LgServices.h
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
#ifndef LGSERVICES_H
#define LGSERVICES_H

#ifdef _DARKGAME
#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/interface.h>
#include <lg/scrservices.h>
#include <lg/scrmanagers.h>
#include <lg/objects.h>
#include <lg/tools.h>
#include <lg/quest.h>
#include <lg/actreact.h>
#include <darkhook.h>
#endif

#include "luax.h"

namespace Lgs
{

class ScriptServices
{
protected:
#ifdef _DARKGAME
	static SService<IActReactSrv> ActReactSrv;
	static SService<IAIScrSrv> AISrv;
	static SService<IAnimTextureSrv> AnimTextureSrv;
	static SService<IBowSrv> BowSrv;
	static SService<ICameraSrv> CameraSrv;
	//static SService<IContainSrv> ContainSrv;
	//static SService<IDamageSrv> DamageSrv;
	static SService<IDarkGameSrv> DarkGameSrv;
	static SService<IDarkUISrv> DarkUISrv;
	static SService<IDataSrv> DataSrv;
	static SService<IDebugScrSrv> DebugSrv;
	static SService<IDoorSrv> DoorSrv;
	static SService<IDarkInvSrv> DarkInvSrv;
	static SService<IDarkPowerupsSrv> DarkPowerupsSrv;
	//static SService<IKeySrv> KeySrv;
	static SService<ILightScrSrv> LightSrv;
	static SService<ILinkSrv> LinkSrv;
	static SService<ILinkToolsSrv> LinkToolsSrv;
	static SService<ILockSrv> LockSrv;
	static SService<INetworkingSrv> NetworkingSrv;
	static SService<IObjectSrv> ObjectSrv;
	static SService<IPGroupSrv> PGroupSrv;
	static SService<IPhysSrv> PhysSrv;
	static SService<IPickLockSrv> PickLockSrv;
	static SService<IPlayerLimbsSrv> PlayerLimbsSrv;
	static SService<IPropertySrv> PropertySrv;
	static SService<IPuppetSrv> PuppetSrv;
	//static SService<IQuestSrv> QuestSrv;
	static SService<IShockGameSrv> ShockGameSrv;
	static SService<IShockObjSrv> ShockObjSrv;
	static SService<IShockPsiSrv> ShockPsiSrv;
	static SService<IShockAISrv> ShockAISrv;
	static SService<IShockWeaponSrv> ShockWeaponSrv;
	static SService<ISoundScrSrv> SoundSrv;
	static SService<IWeaponSrv> WeaponSrv;
	static SService<IDarkHookScriptService> DarkHookSrv;
	static SInterface<IPropertyManager> PropertyMan;
	static SInterface<ILinkManager> LinkMan;
	static SInterface<IObjectSystem> ObjectSys;
	static SInterface<ITraitManager> TraitMan;
	static SInterface<IContainSys> ContainSys;
	static SInterface<IDamageModel> DamageSys;
	static SInterface<IKeySys> KeySys;
	static SInterface<IQuestData> QuestData;
	static SInterface<IReactions> ARReactions;
	static SInterface<IStimuli> ARStimuli;
#endif

private:
	struct ServiceDef
	{
		const char* name;
		const luax::Registry* reg;
	};
#ifdef _DARKGAME
	static const ServiceDef Thief1Services[];
	static const ServiceDef Thief2Services[];
	static const ServiceDef SShock2Services[];
#else
	static const ServiceDef ShellServices[];
#endif

public:
	static void Init(luax::State& S);

private:
	static int CreateLink(luax::Handle);
	static int CreateObject(luax::Handle);

};

/**
 * ActReactSrv
 */
class ActReactService : protected ScriptServices
{
protected:
	/**
	 * Perform a reaction response.
	 *
	 * @return boolean success
	 * @param integer|string reaction_kind
	 * @param number intensity
	 * @param integer source
	 * @param integer destination
	 * @param any data1 (optional)
	 * @param any data2 (optional)
	 * @param any data3 (optional)
	 * @param any data4 (optional)
	 * @param any data5 (optional)
	 * @param any data6 (optional)
	 * @param any data7 (optional)
	 * @param any data8 (optional)
	 */
	static int React(luax::Handle);
	/**
	 * Get the ID of a reaction response.
	 *
	 * @return integer|nil reaction_kind
	 * @param string reaction_name
	 */
	static int GetReactionNamed(luax::Handle);
	/**
	 * Get the name of a reaction response.
	 *
	 * @return string|nil reaction_name
	 * @param integer reaction_kind
	 */
	static int GetReactionName(luax::Handle);
	/**
	 * Register an object to receive stimulus messages of a particular type.
	 * Either subscribe by script, or use the ''Send to Scripts'' response, but not both.
	 *
	 * @return boolean success
	 * @param integer destination
	 * @param integer stimulus
	 */
	static int SubscribeToStimulus(luax::Handle);
	/**
	 * Stop listening for stimulus messages
	 *
	 * @return boolean success
	 * @param integer destination
	 * @param integer stimulus
	 */
	static int UnsubscribeToStimulus(luax::Handle);
	/**
	 * Initiate a stimulus contact on an object.
	 *
	 * @return boolean success
	 * @param integer stimulus
	 * @param integer destination
	 */
	static int BeginContact(luax::Handle);
	/**
	 * Stop a stimulus contact on an object.
	 *
	 * @return boolean success
	 * @param integer stimulus
	 * @param integer destination
	 */
	static int EndContact(luax::Handle);
	/**
	 * Briefly touch an object.
	 *
	 * @return boolean success
	 * @param integer stimulus
	 * @param integer destination
	 */
	static int SetSingleSensorContact(luax::Handle);
	static int GetStimulusName(luax::Handle);
	static int GetStimulusNamed(luax::Handle);
	static int IsStimulus(luax::Handle);
};
class ActReact1Service : protected ActReactService
{
protected:
	static int Stimulate(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class ActReact2Service : protected ActReactService
{
protected:
	/**
	 * Stimulate an object.
	 *
	 * @return boolean success
	 * @param integer target
	 * @param integer stimulus
	 * @param number intensity
	 * @param integer source
	 */
	static int Stimulate(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * AISrv
 */
class AIService : protected ScriptServices
{
protected:
	/**
	 * Instruct an AI to go to the current location of an object.
	 *
	 * @return boolean success
	 * @param integer ai_object
	 * @param integer target
	 * @param string|integer speed (optional)
	 * @param string|integer priority (optional)
	 * @param any data (optional)
	 */
	static int MakeGotoObjLoc(luax::Handle);
	/**
	 * Instruct an AI to frob an object with another object.
	 *
	 * @return boolean success
	 * @param integer ai_object
	 * @param integer target
	 * @param integer frob_with
	 * @param string|integer priority (optional)
	 * @param any data (optional)
	 */
	static int MakeFrobObjWith(luax::Handle);
	/**
	 * Instruct an AI to frob an object.
	 *
	 * @return boolean success
	 * @param integer ai_object
	 * @param integer target
	 * @param string|integer priority (optional)
	 * @param any data (optional)
	 */
	static int MakeFrobObj(luax::Handle);
	/**
	 * Get the current alert level of an AI.
	 *
	 * @return integer level
	 * @param integer ai
	 */
	static int GetAlertLevel(luax::Handle);
	/**
	 * Set the minimum alert level for an AI.
	 *
	 * @param integer ai
	 * @param integer level
	 */
	static int SetMinimumAlert(luax::Handle);
	/**
	 * Make an AI forget what it's doing.
	 *
	 * @param integer ai
	 */
	static int ClearGoals(luax::Handle);
	/**
	 * Turn off broadcasts from an AI.
	 *
	 * @param integer ai
	 * @param boolean disable
	 */
	static int SetScriptFlags(luax::Handle);
	/**
	 * Reset the alertness of an AI to the lowest allowable level.
	 *
	 * @param integer ai
	 */
	static int ClearAlertness(luax::Handle);
	/**
	 * Send a signal to an AI.
	 *
	 * @param integer ai
	 * @param string signal
	 */
	static int Signal(luax::Handle);
	/**
	 * Trigger a conversation.
	 *
	 * @return boolean success
	 * @param integer trap
	 */
	static int StartConversation(luax::Handle);
	static const char* const ScriptSpeed[];
	static const char* const ActionPriority[];
public:
	static const luax::Registry Methods[];
};
/**
 * AnimTextureSrv
 */
class AnimTextureService : protected ScriptServices
{
protected:
	/**
	 * Swap a texture on nearby terrain.
	 * Both family names can be omitted. If either family is given, then
	 * all parameters must be given even if they are ''nil''.
	 *
	 * @return boolean success
	 * @param integer marker
	 * @param string old_family (optional)
	 * @param string old_texture
	 * @param string new_family (optional)
	 * @param string new_texture
	 */
	static int ChangeTexture(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * BowSrv
 */
class BowService : protected ScriptServices
{
protected:
	/**
	 * Set the bow as the current weapon.
	 * Always succeeds.
	 *
	 * @return boolean success
	 */
	static int Equip(luax::Handle);
	/**
	 * Clear the current weapon if it was the bow.
	 * Always succeeds.
	 *
	 * @return boolean success
	 */
	static int UnEquip(luax::Handle);
	/**
	 * Check if the bow is the current weapon.
	 *
	 * @return boolean isequipped
	 */
	static int IsEquipped(luax::Handle);
	/**
	 * Begin drawing the bow.
	 *
	 * @return boolean success
	 */
	static int StartAttack(luax::Handle);
	/**
	 * Release the bow.
	 *
	 * @return boolean success
	 */
	static int FinishAttack(luax::Handle);
	/**
	 * Relax the bow without firing.
	 *
	 * @return boolean success
	 */
	static int AbortAttack(luax::Handle);
	/**
	 * Make an object the current projectile for the bow.
	 *
	 * @return boolean success
	 * @param integer arrow
	 */
	static int SetArrow(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * CameraSrv
 */
class CameraService : protected ScriptServices
{
protected:
	/**
	 * Attach the camera to an object. Do not allow freelook.
	 *
	 * @param integer object
	 */
	static int StaticAttach(luax::Handle);
	/**
	 * Attach the camera to an object. Allow freelook.
	 *
	 * @param integer object
	 */
	static int DynamicAttach(luax::Handle);
	/**
	 * Return the camera to the player if it's still attached to the object.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int CameraReturn(luax::Handle);
	/**
	 * Return the camera to the player no matter what object it is attached to.
	 *
	 */
	static int ForceCameraReturn(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * ContainSrv
 */
class ContainService : protected ScriptServices
{
protected:
	/**
	 * Put an object in a container.
	 *
	 * @return boolean success
	 * @param integer item
	 * @param integer container
	 * @param integer contains_type (optional)
	 * @param boolean combine (optional)
	 */
	static int Add(luax::Handle);
	/**
	 * Remove an object from a container.
	 *
	 * @return boolean success
	 * @param integer item
	 * @param integer container
	 */
	static int Remove(luax::Handle);
	/**
	 * Move all objects from one container into another.
	 *
	 * @param integer source
	 * @param integer destination
	 * @param boolean combine (optional)
	 */
	static int MoveAllContents(luax::Handle);
	/**
	 * Add to (or subtract from) the stack count of an object.
	 *
	 * @return integer actual_amount
	 * @param integer object
	 * @param integer amount
	 */
	static int StackAdd(luax::Handle);
	/**
	 * Get the contains type of an item if it is in a container.
	 *
	 * @return integer|nil contains_type
	 * @param integer container
	 * @param integer item
	 */
	static int IsHeld(luax::Handle);
	static int GetContainer(luax::Handle);
	static const char* const ContainsType[];

public:
	static const luax::Registry Methods[];
};
/**
 * DamageSrv
 */
class DamageService : protected ScriptServices
{
protected:
	/**
	 * Damages an object.
	 * The damage type is usually a stimulus.
	 *
	 * @return FIXME result
	 * @param integer victim
	 * @param integer culprit
	 * @param integer intensity
	 * @param integer damage_type (optional)
	 */
	static int Damage(luax::Handle);
	/**
	 * Makes the victim dead.
	 * Hit points are unaffected.
	 *
	 * @return FIXME result
	 * @param integer victim
	 * @param integer culprit
	 */
	static int Slay(luax::Handle);
	/**
	 * Makes the "victim" not-dead.
	 * Hit points are unaffected.
	 *
	 * @return FIXME result
	 * @param integer victim
	 * @param integer culprit
	 */
	static int Resurrect(luax::Handle);
	static int Terminate(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DarkGameSrv
 */
class DarkGameService : protected ScriptServices
{
protected:
	/**
	 * "Kills" the player by immobilizing him and tilting the camera.
	 * Does not actually affect the player's hit points. Does not end the mission.
	 *
	 * @return boolean success
	 */
	static int KillPlayer(luax::Handle);
	/**
	 * Immediately ends the mission.
	 *
	 * @return boolean success
	 */
	static int EndMission(luax::Handle);
	/**
	 * Gradually fade the screen to all-black.
	 * Fade time is in seconds. A negative value will immediately return
	 * the screen to full visibility.
	 *
	 * @return boolean success
	 * @param number seconds
	 */
	static int FadeToBlack(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class DarkGame2Service : protected DarkGameService
{
protected:
	/**
	 * Mark a hidden object as found.
	 *
	 * @param integer object
	 */
	static int FoundObject(luax::Handle);
	/**
	 * Check if a configuration variable has been set.
	 *
	 * @return boolean isset
	 * @param string config
	 */
	static int ConfigIsDefined(luax::Handle);
	/**
	 * Get a configuration variable as an integer.
	 *
	 * @return integer|nil value
	 * @param string config
	 */
	static int ConfigGetInt(luax::Handle);
	/**
	 * Get a configuration variable as a real number.
	 *
	 * @return number|nil value
	 * @param string config
	 */
	static int ConfigGetFloat(luax::Handle);
	/**
	 * Get an input binding variable.
	 *
	 * @return number value
	 * @param string config
	 */
	static int BindingGetFloat(luax::Handle);
	/**
	 * Check if an area of the automap has been visited.
	 *
	 * @return boolean isvisited
	 * @param integer page
	 * @param integer room
	 */
	static int GetAutomapLocationVisited(luax::Handle);
	/**
	 * Mark an area of the automap as visited.
	 *
	 * @param integer page
	 * @param integer room
	 */
	static int SetAutomapLocationVisited(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DarkInvSrv
 */
class DarkInvService : protected ScriptServices
{
protected:
	/**
	 * I haven't been able to get this to work yet.
	 *
	 * @param string|integer capability
	 * @param string|integer control
	 */
	static int CapabilityControl(luax::Handle);
	/**
	 * Set a speed multiplier on the player.
	 *
	 * @param string name
	 * @param number speed
	 * @param number rotation_speed (optional)
	 */
	static int AddSpeedControl(luax::Handle);
	/**
	 * Cancel a speed multiplier.
	 *
	 * @param string name
	 */
	static int RemoveSpeedControl(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DarkPowerupsSrv
 */
class DarkPowerupsService : protected ScriptServices
{
protected:
	/**
	 * Create a blinding flash from an object.
	 *
	 * @param integer object
	 */
	static int TriggerWorldFlash(luax::Handle);
	/**
	 * Create a new object at the location of another object.
	 *
	 * @return integer|nil newobject
	 * @param integer archetype
	 * @param integer object
	 */
	static int ObjTryDeploy(luax::Handle);
	/**
	 * Slay nearby objects that have the Blood property.
	 *
	 * @param integer object
	 * @param number distance
	 */
	static int CleanseBlood(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DarkUISrv
 */
class DarkUIService : protected ScriptServices
{
protected:
	/**
	 * Display a string on-screen.
	 *
	 * @param string message
	 * @param integer milliseconds (optional)
	 * @param integer|string color (optional)
	 */
	static int TextMessage(luax::Handle);
	/**
	 * Display book text with a background image.
	 *
	 * @param string book
	 * @param string art
	 */
	static int ReadBook(luax::Handle);
	/**
	 * The currently selected inventory item.
	 *
	 * @return integer|nil object
	 */
	static int InvItem(luax::Handle);
	/**
	 * The currently selected weapon.
	 *
	 * @return integer|nil object
	 */
	static int InvWeapon(luax::Handle);
	/**
	 * Set the active inventory object.
	 *
	 * @param boolean success
	 * @param integer object
	 */
	static int InvSelect(luax::Handle);
	/**
	 * Checks if there is a key binding for the command.
	 *
	 * @param boolean isbound
	 * @param string command
	 */
	static int IsCommandBound(luax::Handle);
	/**
	 * Get the keys that are bound to a command.
	 *
	 * @param string description
	 * @param string command
	 */
	static int DescribeKeyBinding(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class DarkUI3Service : protected DarkUIService
{
protected:
	static int TextMessage(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DataSrv
 */
class DataService : protected ScriptServices
{
protected:
	/**
	 * Read a string from a resource file.
	 * You can't use the directory "books". Instead, set the file name to "../books/file"
	 * and use the directory "strings".
	 *
	 * @return string result
	 * @param string file
	 * @param string name
	 * @param string directory (optional)
	 */
	static int GetString(luax::Handle);
	/**
	 * Read a string using the value of a property.
	 * Thief names are "objnames" and "objdescs".
	 * System Shock 2 names are "objname", "objshort", "objlooks",
	 * "modify1", "modify2", "hacktext", "usemsg", "lockmsg", "sett1", "sett2",
	 * "shead1", "shead2", "rsrchtxt", "huduse", "worldcursor", "invcursor", "usecursor".
	 *
	 * @return string result
	 * @param integer object
	 * @param string name
	 */
	static int GetObjString(luax::Handle);
	/**
	 * A random number in the range [0,32767]
	 *
	 * @return number result
	 */
	static int DirectRand(luax::Handle);
	/**
	 * A random number in the range [low,high]
	 *
	 * @return integer result
	 * @param integer low
	 * @param integer high
	 */
	static int RandInt(luax::Handle);
	/**
	 * A random number in the range [0,1]
	 *
	 * @return number result
	 */
	static int RandFlt0to1(luax::Handle);
	/**
	 * A random number in the range (-1,1]
	 *
	 * @return number result
	 */
	static int RandFltNeg1to1(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DebugSrv
 */
class DebugService : protected ScriptServices
{
protected:
	/**
	 * Output to the monolog.
	 * Accepts up to 8 parameters. The total length of all parameters
	 * must not be longer than 1022 characters.
	 *
	 * @param any data1 (optional)
	 * @param any data2 (optional)
	 * @param any data3 (optional)
	 * @param any data4 (optional)
	 * @param any data5 (optional)
	 * @param any data6 (optional)
	 * @param any data7 (optional)
	 * @param any data8 (optional)
	 */
	static int MPrint(luax::Handle);
	/**
	 * Execute a Dromed command.
	 * Accepts up to 7 extra parameters. The total length of all parameters
	 * and the command must not be longer than 1022 characters.
	 *
	 * @param string command
	 * @param any argument1 (optional)
	 * @param any argument2 (optional)
	 * @param any argument3 (optional)
	 * @param any argument4 (optional)
	 * @param any argument5 (optional)
	 * @param any argument6 (optional)
	 * @param any argument7 (optional)
	 */
	static int Command(luax::Handle);
	/**
	 * Causes a software interrupt.
	 * Please do not press this button again.
	 *
	 */
	static int Break(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * DoorSrv
 */
class DoorService : protected ScriptServices
{
protected:
	/**
	 * Close a door.
	 *
	 * @return boolean success
	 * @param integer door
	 */
	static int CloseDoor(luax::Handle);
	/**
	 * Open a door.
	 *
	 * @return boolean success
	 * @param integer door
	 */
	static int OpenDoor(luax::Handle);
	/**
	 * Get the state of a door.
	 *
	 * @return integer state
	 * @param integer door
	 */
	static int GetDoorState(luax::Handle);
	/**
	 * Change the state of a door.
	 *
	 * @return boolean success
	 * @param integer door
	 */
	static int ToggleDoor(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Door2Service : protected DoorService
{
protected:
	/**
	 * Change whether a door should block sound and vision.
	 *
	 * @return boolean success
	 * @param integer door
	 * @param boolean isblocking
	 */
	static int SetBlocking(luax::Handle);
	/**
	 * Test if a door blocks sound.
	 *
	 * @return boolean isblocking
	 * @param integer door
	 */
	static int GetSoundBlocking(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * KeySrv
 */
class KeyService : protected ScriptServices
{
protected:
	/**
	 * Test a key on an object and unlock it if valid.
	 * Usage can be ''open'' (1), ''close'' (2), ''check'' (3) or ''nil'' for the default action.
	 *
	 * @return boolean success
	 * @param integer key
	 * @param integer object
	 * @param string|integer usage (optional)
	 */
	static int TryToUseKey(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * LightSrv
 */
class LightService : protected ScriptServices
{
protected:
	/**
	 * Set the current mode of a light, and the minimum and maximum
	 * brightness values.
	 *
	 * @param integer object
	 * @param string|integer mode
	 * @param number minimum
	 * @param number maximum
	 */
	static int Set(luax::Handle);
	/**
	 * Change the current mode of a light.
	 *
	 * @param integer object
	 * @param string|integer mode
	 */
	static int SetMode(luax::Handle);
	/**
	 * Make a light active. Does not change the mode.
	 *
	 * @param integer object
	 */
	static int Activate(luax::Handle);
	/**
	 * Make a light inactive. Does not change the mode.
	 *
	 * @param integer object
	 */
	static int Deactivate(luax::Handle);
	/**
	 * Begin listening for light mode change messages.
	 *
	 * @param integer object
	 */
	static int Subscribe(luax::Handle);
	/**
	 * Stop listening to light mode change messages.
	 *
	 * @param integer object
	 */
	static int Unsubscribe(luax::Handle);
	/**
	 * Get the current light mode.
	 *
	 * @return integer|nil mode
	 * @param integer object
	 */
	static int GetMode(luax::Handle);
	/**
	 * Return a string that describes a light mode.
	 *
	 * @return string|nil description
	 * @param integer mode
	 */
	static int GetModeName(luax::Handle);
	static const char* const LightMode[];
public:
	static const luax::Registry Methods[];
};
/**
 * LinkSrv
 */
class LinkService : protected ScriptServices
{
protected:
	static short GetRelationNamed(const char *);
protected:
	/**
	 * Add a link.
	 *
	 * @return integer|nil link
	 * @param string|integer flavor
	 * @param integer source
	 * @param integer destination
	 */
	static int Create(luax::Handle);
	/**
	 * Remove a link.
	 *
	 * @return boolean success
	 * @param integer link
	 */
	static int Destroy(luax::Handle);
	/**
	 * Check if there are links between two objects.
	 *
	 * @return boolean exists
	 * @param string|integer flavor
	 * @param integer source (optional)
	 * @param integer destination (optional)
	 */
	static int AnyExist(luax::Handle);
	/**
	 * Get an iterator for links between two objects.
	 *
	 * @return linkset iterator
	 * @param string|integer flavor
	 * @param integer source (optional)
	 * @param integer destination (optional)
	 */
	static int GetAll(luax::Handle);
	/**
	 * Get the first link found between two objects.
	 *
	 * @return integer|nil link
	 * @param string|integer flavor
	 * @param integer source (optional)
	 * @param integer destination (optional)
	 */
	static int GetOne(luax::Handle);
	/**
	 * Send a script message to the destinations of links.
	 * If data is given, and the link flavor supports it, then the
	 * message is only sent to links that match the data.
	 * The fourth parameter can be a function that will be called
	 * for each link. Return ''nil'' to skip the link.
	 *
	 * @param integer source
	 * @param string message
	 * @param string|integer flavor
	 * @param any data or filter (optional)
	 */
	static int BroadcastOnAllLinks(luax::Handle);
	/**
	 * Add many links at once.
	 *
	 * @return boolean success
	 * @param string|integer flavor
	 * @param string sources
	 * @param string destinations
	 */
	static int CreateMany(luax::Handle);
	/**
	 * Remove many links at once.
	 *
	 * @return boolean success
	 * @param string|integer flavor
	 * @param string sources
	 * @param string destinations
	 */
	static int DestroyMany(luax::Handle);
	/**
	 * Get an iterator for links from an object or its parents to another object or its parents.
	 *
	 * @return linkset iterator
	 * @param string|integer flavor
	 * @param integer source
	 * @param integer destination
	 */
	static int GetAllInherited(luax::Handle);
	/**
	 * Get an iterator for links from an object or its parents to a particular or unknown object.
	 *
	 * @return linkset iterator
	 * @param string|integer flavor
	 * @param integer source
	 * @param integer destination (optional)
	 */
	static int GetAllInheritedSingle(luax::Handle);
/**
 * LinkToolsSrv
 */
protected:
	/**
	 * Get the ID of a link flavor.
	 *
	 * @return integer|nil flavor
	 * @param string name
	 */
	static int LinkKindNamed(luax::Handle);
	/**
	 * Get the name of a link flavor.
	 *
	 * @return string|nil name
	 * @param integer flavor
	 */
	static int LinkKindName(luax::Handle);
	/**
	 * Get information about a link
	 *
	 * @return integer|nil destination
	 * @return integer|nil source
	 * @return integer|nil flavor
	 * @param integer link
	 */
	static int LinkGet(luax::Handle);
	/**
	 * Read a field from the link data.
	 *
	 * @return any data
	 * @param integer link
	 * @param string field (optional)
	 */
	static int LinkGetData(luax::Handle);
	/**
	 * Set a field in the link data.
	 *
	 * @param integer link
	 * @param string field (optional)
	 * @param any data
	 */
	static int LinkSetData(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * LockSrv
 */
class LockService : protected ScriptServices
{
protected:
	/**
	 * Check if an object is locked.
	 *
	 * @return boolean islocked
	 * @param integer object
	 */
	static int IsLocked(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * NetworkingSrv
 */
class NetworkingService : protected ScriptServices
{
protected:
	/**
	 * Send a message to an object on other hosts.
	 *
	 * @return boolean success
	 * @param integer destination
	 * @param string message
	 * @param boolean loopback (optional)
	 * @param any data (optional)
	 */
	static int Broadcast(luax::Handle);
	/**
	 * Send a message to a specific host.
	 *
	 * @return boolean success
	 * @param integer host
	 * @param integer destination
	 * @param string message
	 * @param any data (optional)
	 */
	static int SendToProxy(luax::Handle);
	/**
	 * Become the host for an object.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int TakeOver(luax::Handle);
	/**
	 * Change the host for an object.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param integer host
	 */
	static int GiveTo(luax::Handle);
	/**
	 * Check if the object is one of the players.
	 *
	 * @return boolean isplayer
	 * @param integer object
	 */
	static int IsPlayer(luax::Handle);
	/**
	 * Check if the game is in multiplayer mode.
	 *
	 * @return boolean ismultiplayer
	 */
	static int IsMultiplayer(luax::Handle);
	/**
	 * Send a timed message using a proxy.
	 *
	 * @return integer timerid
	 * @param integer destination
	 * @param string name
	 * @param number seconds
	 * @param any data (optional)
	 */
	static int SetProxyOneShotTimer(luax::Handle);
	/**
	 * Begin iterating player objects.
	 *
	 * @return integer|nil object
	 */
	static int FirstPlayer(luax::Handle);
	/**
	 * Get another player object.
	 * Returns ''nil'' when there are no more players.
	 *
	 * @return integer|nil object
	 */
	static int NextPlayer(luax::Handle);
	/**
	 * Disable network communication.
	 *
	 */
	static int Suspend(luax::Handle);
	/**
	 * Restart the network.
	 *
	 */
	static int Resume(luax::Handle);
	/**
	 * Check if this node is the host of an object.
	 *
	 * @return boolean ishost
	 * @param integer object
	 */
	static int HostedHere(luax::Handle);
	/**
	 * Check if an object is hosted by another node.
	 *
	 * @return boolean isproxy
	 * @param integer object
	 */
	static int IsProxy(luax::Handle);
	/**
	 * Check if an object only exists at this node.
	 *
	 * @return boolean islocal
	 * @param integer object
	 */
	static int LocalOnly(luax::Handle);
	/**
	 * Check if the network is active.
	 *
	 * @return boolean isnetwork
	 */
	static int IsNetworking(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Networking2Service : protected NetworkingService
{
protected:
	/**
	 * Get the host of an object.
	 *
	 * @return integer|nil host
	 * @param integer object
	 */
	static int Owner(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * ObjectSrv
 */
class ObjectService : protected ScriptServices
{
protected:
	/**
	 * Create a new object in two steps.
	 *
	 * @return integer|nil object
	 * @param integer archetype
	 */
	static int BeginCreate(luax::Handle);
	/**
	 * Finish creating an object.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int EndCreate(luax::Handle);
	/**
	 * Create a new object.
	 *
	 * @return integer|nil object
	 * @param integer archetype
	 */
	static int Create(luax::Handle);
	/**
	 * Destroy an object.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int Destroy(luax::Handle);
	/**
	 * Check if an object ID is valid.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int Exists(luax::Handle);
	/**
	 * Change the string that identifies an object.
	 *
	 * @param integer object
	 * @param string name
	 */
	static int SetName(luax::Handle);
	/**
	 * Get the name of an object.
	 *
	 * @return string|nil name
	 * @param integer object
	 */
	static int GetName(luax::Handle);
	/**
	 * Get the ID of a named object.
	 *
	 * @return integer|nil object
	 * @param string name
	 */
	static int Named(luax::Handle);
	/**
	 * Link a metaproperty with an object.
	 *
	 * @param integer object
	 * @param integer metaproperty
	 */
	static int AddMetaProperty(luax::Handle);
	/**
	 * Remove one metaproperty link from an object.
	 * If the same metaproperty has been added more than once to the
	 * object, this will only remove one of the copies.
	 *
	 * @param integer object
	 * @param integer metaproperty
	 */
	static int RemoveMetaProperty(luax::Handle);
	/**
	 * Check if a metaproperty has been added to the object.
	 *
	 * @return boolean isset
	 * @param integer object
	 * @param integer metaproperty
	 */
	static int HasMetaProperty(luax::Handle);
	/**
	 * Check if an object inherits the properties from an archetype or metaproperty.
	 *
	 * @return boolean ischild
	 * @param integer object
	 * @param integer archetype
	 */
	static int InheritsFrom(luax::Handle);
	/**
	 * Check if an object is transient.
	 *
	 * @return boolean istemp
	 * @param integer object
	 */
	static int IsTransient(luax::Handle);
	/**
	 * Change the transient state of an object.
	 * Transient objects are not saved in mission files.
	 *
	 * @param integer object
	 * @param boolean istemp
	 */
	static int SetTransience(luax::Handle);
	/**
	 * Get the location of an object.
	 *
	 * @return vector location
	 * @param integer object
	 */
	static int Position(luax::Handle);
	/**
	 * Get the rotation of an object.
	 *
	 * @return vector rotation
	 * @param integer object
	 */
	static int Facing(luax::Handle);
	/**
	 * Move an object to another location.
	 * At least a location or a destination object must be specified.
	 *
	 * @param integer object
	 * @param vector location (optional)
	 * @param vector rotation (optional)
	 * @param integer destination (optional)
	 */
	static int Teleport(luax::Handle);
	static int GetArchetype(luax::Handle);
	static int IsArchetype(luax::Handle);
	static int IsMetaProperty(luax::Handle);
	static int IsDonor(luax::Handle);
//TODO: Query
};
class Object1Service : protected ObjectService
{
protected:
	static int FindClosestObjectNamed(luax::Handle);
	static int AddMetaPropertyToMany(luax::Handle);
	static int RemoveMetaPropertyFromMany(luax::Handle);
	static int RenderedThisFrame(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Object2Service : protected ObjectService
{
protected:
	/**
	 * Check if the location of an object is not in solid space.
	 *
	 * @return boolean isvalid
	 * @param integer object
	 */
	static int IsPositionValid(luax::Handle);
	/**
	 * Search for a nearby object that inherits from an archetype.
	 *
	 * @return integer|nil object
	 * @param integer marker
	 * @param string archetype
	 */
	static int FindClosestObjectNamed(luax::Handle);
	/**
	 * Add a metaproperty to multiple objects.
	 *
	 * @return integer count
	 * @param integer metaproperty
	 * @param string objects
	 */
	static int AddMetaPropertyToMany(luax::Handle);
	/**
	 * Remove a metaproperty from multiple objects.
	 *
	 * @return integer count
	 * @param integer metaproperty
	 * @param string objects
	 */
	static int RemoveMetaPropertyFromMany(luax::Handle);
	/**
	 * Check if an object is being drawn.
	 *
	 * @return boolean isvisible
	 * @param integer object
	 */
	static int RenderedThisFrame(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Object3Service : protected ObjectService
{
protected:
	static int IsPositionValid(luax::Handle);
	static int FindClosestObjectNamed(luax::Handle);
	static int AddMetaPropertyToMany(luax::Handle);
	static int RemoveMetaPropertyFromMany(luax::Handle);
	static int RenderedThisFrame(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * PGroupSrv
 */
class PGroupService : protected ScriptServices
{
protected:
	/**
	 * Enable or disable a particle SFX.
	 *
	 * @param integer object
	 * @param boolean active
	 */
	static int SetActive(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * PhysSrv
 */
class PhysService : protected ScriptServices
{
protected:
	/**
	 * Listen for physics model messages.
	 *
	 * @param integer object
	 * @param string|integer messages
	 */
	static int SubscribeMsg(luax::Handle);
	/**
	 * Stop listening to physics messages.
	 *
	 * @param integer object
	 * @param string|integer messages
	 */
	static int UnsubscribeMsg(luax::Handle);
	/**
	 * Emit an object.
	 *
	 * @return integer|nil object
	 * @param integer source
	 * @param integer archetype
	 * @param number scale (optional)
	 * @param string|integer options (optional)
	 * @param vector velocity (optional)
	 */
	static int LaunchProjectile(luax::Handle);
	/**
	 * Change the velocity of an object.
	 *
	 * @param integer object
	 * @param vector velocity
	 */
	static int SetVelocity(luax::Handle);
	/**
	 * Get the velocity of an object.
	 *
	 * @return vector velocity
	 * @param integer object
	 */
	static int GetVelocity(luax::Handle);

	static const char* const PhysMessages[];
};
class Phys1Service : protected PhysService
{
protected:
	static int SetGravity(luax::Handle);
	static int GetGravity(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Phys2Service : protected PhysService
{
protected:
	/**
	 * Lock the velocity of an object.
	 *
	 * @param integer object
	 * @param vector velocity
	 */
	static int ControlVelocity(luax::Handle);
	/**
	 * Unlock the velocity of an object.
	 *
	 * @param integer object
	 */
	static int StopControlVelocity(luax::Handle);
	/**
	 * Change the gravity of an object.
	 *
	 * @param integer object
	 * @param number gravity
	 */
	static int SetGravity(luax::Handle);
	/**
	 * Get the gravity of an object.
	 *
	 * @return number gravity
	 * @param integer object
	 */
	static int GetGravity(luax::Handle);
	/**
	 * Does the object have a physics model.
	 *
	 * @return boolean isphysical
	 * @param integer object
	 */
	static int HasPhysics(luax::Handle);
	/**
	 * Is the physics model a sphere or sphere hat.
	 *
	 * @return boolean issphere
	 * @param integer object
	 */
	static int IsSphere(luax::Handle);
	/**
	 * Is the physics model an oriented bounding-box.
	 *
	 * @return boolean isobb
	 * @param integer object
	 */
	static int IsOBB(luax::Handle);
	/**
	 * Lock the location of an object to the current position.
	 *
	 * @param integer object
	 */
	static int ControlCurrentLocation(luax::Handle);
	/**
	 * Lock the rotation of an object to the current facing.
	 *
	 * @param integer object
	 */
	static int ControlCurrentRotation(luax::Handle);
	/**
	 * Lock the location and rotation of an object.
	 *
	 * @param integer object
	 */
	static int ControlCurrentPosition(luax::Handle);
	/**
	 * Make an object non-physical.
	 *
	 * @param integer object
	 */
	static int DeregisterModel(luax::Handle);
	/**
	 * Nudge the player.
	 *
	 * @param integer submodel (optional)
	 * @param vector offset
	 */
	static int PlayerMotionSetOffset(luax::Handle);
	/**
	 * Wake up an object at rest.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int Activate(luax::Handle);
	/**
	 * Test that an object is not in solid space.
	 *
	 * @return boolean isvalid
	 * @param integer object
	 */
	static int ValidPos(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Phys3Service : protected Phys1Service
{
protected:
	static int HasPhysics(luax::Handle);
	static int IsSphere(luax::Handle);
	static int IsOBB(luax::Handle);
	static int ControlCurrentLocation(luax::Handle);
	static int ControlCurrentRotation(luax::Handle);
	static int ControlCurrentPosition(luax::Handle);
	static int DeregisterModel(luax::Handle);
	static int PlayerMotionSetOffset(luax::Handle);
	static int Activate(luax::Handle);
	static int ValidPos(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * PickLockSrv
 */
class PickLockService : protected ScriptServices
{
protected:
	/**
	 * Prepare to use a lockpick.
	 * Not used by Thief.
	 *
	 * @return boolean success
	 * @param integer frobber
	 * @param integer pick
	 */
	static int Ready(luax::Handle);
	/**
	 * Finish with a lockpick.
	 * Not used by Thief.
	 *
	 * @return boolean success
	 * @param integer frobber
	 * @param integer pick
	 */
	static int UnReady(luax::Handle);
	/**
	 * Begin using a lockpick.
	 *
	 * @return boolean success
	 * @param integer frobber
	 * @param integer pick
	 * @param integer lock
	 */
	static int StartPicking(luax::Handle);
	/**
	 * Stop using a lockpick.
	 *
	 * @return boolean success
	 * @param integer pick
	 */
	static int FinishPicking(luax::Handle);
	/**
	 * Test a pick on a lock.
	 *
	 * @return integer state
	 * @param integer pick
	 * @param integer lock
	 * @param integer bits (optional)
	 */
	static int CheckPick(luax::Handle);
	/**
	 * Move a pick.
	 * Not used by Thief.
	 *
	 * @return boolean success
	 * @param integer pick
	 */
	static int DirectMotion(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * PlayerLimbsSrv
 */
class PlayerLimbsService : protected ScriptServices
{
protected:
	/**
	 * Show the player arm.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int Equip(luax::Handle);
	/**
	 * Hide the player arm.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int UnEquip(luax::Handle);
	/**
	 * Move the player arm.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int StartUse(luax::Handle);
	/**
	 * Stop moving the player arm.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int FinishUse(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * PropertySrv
 */
class PropertyService : protected ScriptServices
{
protected:
	/**
	 * Get the value of a property field.
	 *
	 * @return any data
	 * @param integer object
	 * @param string property
	 * @param string field (optional)
	 */
	static int Get(luax::Handle);
	/**
	 * Set the value of a property field.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string property
	 * @param string field (optional)
	 * @param any data
	 */
	static int Set(luax::Handle);
	/**
	 * Add a property to an object.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string property
	 */
	static int Add(luax::Handle);
	/**
	 * Remove a property from an object.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string property
	 */
	static int Remove(luax::Handle);
	/**
	 * Set a property to the same as another object.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string property
	 * @param integer source
	 */
	static int CopyFrom(luax::Handle);
	/**
	 * Check if an object has a property, or inherits the property from an archetype.
	 *
	 * @return boolean hasproperty
	 * @param integer object
	 * @param string property
	 */
	static int Possessed(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Property2Service : protected PropertyService
{
protected:
	static int Get(luax::Handle);
	static int Set(luax::Handle);
	static int SetLocal(luax::Handle);
#if 0
	static int Add(luax::Handle);
	static int Remove(luax::Handle);
	static int CopyFrom(luax::Handle);
	static int Possessed(luax::Handle);
#endif
public:
	static const luax::Registry Methods[];
};
/**
 * PuppetSrv
 */
class PuppetService : protected ScriptServices
{
protected:
	/**
	 * Play a motion schema.
	 * Not a tag.
	 *
	 * @return boolean success
	 * @param string name
	 */
	static int PlayMotion(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * QuestSrv
 */
class QuestService : protected ScriptServices
{
protected:
	/**
	 * Listen for changes to a quest variable.
	 *
	 * @param integer object
	 * @param string name
	 * @param string|integer type (optional)
	 */
	static int SubscribeMsg(luax::Handle);
	/**
	 * Stop listening for changes.
	 *
	 * @param integer object
	 * @param string name
	 */
	static int UnsubscribeMsg(luax::Handle);
	/**
	 * Set a quest variable.
	 *
	 * @param string name
	 * @param integer value
	 * @param boolean campaign (optional)
	 */
	static int Set(luax::Handle);
	/**
	 * Get a quest variable.
	 *
	 * @return integer value
	 * @param string name
	 */
	static int Get(luax::Handle);
	/**
	 * Check if a quest variable has been set.
	 *
	 * @return boolean isset
	 * @param string name
	 */
	static int Exists(luax::Handle);
	/**
	 * Delete a quest variable.
	 *
	 * @param string name
	 */
	static int Delete(luax::Handle);
//TODO: Iter
	static const char* const QuestType[];
public:
	static const luax::Registry Methods[];
};
/**
 * ShockGameSrv
 */
class ShockGameService : protected ScriptServices
{
protected:
	/**
	 * Delete the object held by the cursor.
	 *
	 */
	static int DestroyCursorObj(luax::Handle);
	/**
	 * Delete an object that may be in the inventory.
	 *
	 * @param integer object
	 */
	static int DestroyInvObj(luax::Handle);
	/**
	 * Delete multiple items of a type.
	 *
	 * @return boolean success
	 * @param string archetype
	 */
	static int DestroyAllByName(luax::Handle);
	/**
	 * Kill multiple items of a type.
	 *
	 * @return boolean success
	 * @param string archetype
	 */
	static int SlayAllByName(luax::Handle);
	/**
	 * Activate or deactivate the tweq on multiple objects.
	 *
	 * @return boolean success
	 * @param string archetype
	 * @param boolean activate (optional)
	 */
	static int TweqAllByName(luax::Handle);
	/**
	 * Hides an object by removing it from the player's inventory.
	 *
	 * @param integer object
	 */
	static int HideInvObj(luax::Handle);
	/**
	 * Put an object in the player's inventory if there is space for it.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int AddInvObj(luax::Handle);
	/**
	 * Change the psi points of the player.
	 *
	 * @param number amount
	 */
	static int SetPlayerPsiPoints(luax::Handle);
	/**
	 * Get the number of psi points the player has.
	 *
	 * @return integer amount
	 */
	static int GetPlayerPsiPoints(luax::Handle);
	/**
	 * Get the maximum amount of psi points the player can have.
	 *
	 * @return integer amount
	 */
	static int GetPlayerMaxPsiPoints(luax::Handle);
	/**
	 * Give a psi ability to a player.
	 *
	 * @param integer player
	 * @param integer psi
	 */
	static int GrantPsiPower(luax::Handle);
	/**
	 * Check if a psi power is being used.
	 *
	 * @return boolean isactive
	 * @param integer psi
	 */
	static int IsPsiActive(luax::Handle);
	/**
	 * Set the current psi power.
	 *
	 * @param integer psi
	 */
	static int SetSelectedPsiPower(luax::Handle);
	/**
	 * Display the replicator menu for an object.
	 *
	 * @param integer object
	 */
	static int Replicator(luax::Handle);
	/**
	 * Mark a log as being read at this time.
	 *
	 * @param integer deck
	 * @param string|integer logtype
	 * @param integer lognumber
	 */
	static int SetLogTime(luax::Handle);
	/**
	 * Read a log object.
	 * The object is destroyed.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param boolean showalways (optional)
	 */
	static int UseLog(luax::Handle);
	/**
	 * Read a log.
	 *
	 * @return boolean success
	 * @param string|integer logtype
	 * @param integer deck
	 * @param integer lognumber
	 * @param boolean showalways (optional)
	 */
	static int TriggerLog(luax::Handle);
	/**
	 * Get log number from an object.
	 *
	 * @return integer|nil deck
	 * @return integer lognumber
	 * @param integer object
	 * @param string|integer logtype
	 */
	static int FindLogData(luax::Handle);
	/**
	 * Attach the camera to another object.
	 * The object must be uniquely named.
	 *
	 * @param string object
	 */
	static int AttachCamera(luax::Handle);
	/**
	 * Start a cutscene.
	 *
	 * @param string scene
	 */
	static int CutSceneModeOn(luax::Handle);
	/**
	 * End a cutscene.
	 *
	 */
	static int CutSceneModeOff(luax::Handle);
	/**
	 * Create the player object.
	 *
	 * @return integer|nil object
	 * @param string name
	 */
	static int CreatePlayerPuppet(luax::Handle);
	/**
	 * Delete the player object.
	 *
	 */
	static int DestroyPlayerPuppet(luax::Handle);
	/**
	 * Display the container menu for an object.
	 *
	 * @param integer object
	 */
	static int Container(luax::Handle);
	/**
	 * Display a "yes" or "no" prompt.
	 *
	 * @param integer object
	 * @param string prompt
	 */
	static int YorN(luax::Handle);
	/**
	 * Display a keypad.
	 *
	 * @param integer object
	 */
	static int Keypad(luax::Handle);
	/**
	 * Hack, Repair, Modify, Maintain, Research.
	 *
	 * @return boolean success
	 * @param string|integer action
	 * @param integer object
	 * @param boolean psionic (optional)
	 */
	static int HRM(luax::Handle);
	/**
	 * Display the HRM interface for an object.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int TechTool(luax::Handle);
	/**
	 * Subtract nanites from the player.
	 * A negative amount will add nanites.
	 *
	 * @return boolean success
	 * @param number amount
	 */
	static int PayNanites(luax::Handle);
	/**
	 * Show or hide an on-screen display.
	 *
	 * @param integer id
	 * @param string|integer mode
	 */
	static int OverlayChange(luax::Handle);
	/**
	 * Set the object to receive overlay messages.
	 *
	 * @param integer id
	 * @param integer object
	 */
	static int OverlaySetObj(luax::Handle);
	/**
	 * Return the object for an overlay.
	 *
	 * @return integer|nil object
	 */
	static int OverlayGetObj(luax::Handle);
	/**
	 * Show or hide an on-screen display with an object.
	 *
	 * @param integer id
	 * @param string|integer mode
	 * @param integer object
	 */
	static int OverlayChangeObj(luax::Handle);
	/**
	 * Return whether an on-screen display is enabled.
	 *
	 * @return boolean isshown
	 * @param integer id
	 */
	static int OverlayOn(luax::Handle);
	/**
	 * Get an object being used by the player.
	 * Slot numbers are the Contains link type minus 1000.
	 *
	 * @return integer|nil object
	 * @param integer slot
	 */
	static int Equipped(luax::Handle);
	/**
	 * Put an object in one of the player's special slots.
	 *
	 * @param integer slot
	 * @param integer object
	 */
	static int Equip(luax::Handle);
	/**
	 * Use or remove armor.
	 * Without an argument, the armor is removed.
	 *
	 * @param integer object (optional)
	 */
	static int WearArmor(luax::Handle);
	/**
	 * Change the map.
	 * Flags are "RoomObjs" and "NetSynch"
	 *
	 * @return boolean success
	 * @param string name
	 * @param integer location
	 * @param string|integer flags
	 */
	static int LevelTransport(luax::Handle);
	/**
	 * Display an access message for a lock.
	 *
	 * @param integer object
	 * @param boolean unlocked
	 */
	static int SpewLockData(luax::Handle);
	/**
	 * Check if an object is locked for a player or AI.
	 * If using keys, the frobber is required.
	 *
	 * @return boolean islocked
	 * @param integer object
	 * @param boolean usekeys (optional)
	 * @param integer frobber (optional)
	 */
	static int CheckLocked(luax::Handle);
	/**
	 * Display on-screen text.
	 * If the first argument is an object, the string will come from a property.
	 *
	 * @param integer object (optional)
	 * @param string property|text
	 * @param integer player (optional)
	 * @param number milliseconds
	 */
	static int AddText(luax::Handle);
	//static int AddTextObjProp(luax::Handle);
	/**
	 * Display on-screen text read from a string file.
	 * The index is appended to the name. The parameter is substituted in the text.
	 *
	 * @param string name
	 * @param string file
	 * @param integer object
	 * @param number index (optional)
	 * @param number parameter (optional)
	 * @param number milliseconds
	 */
	static int AddTranslatableText(luax::Handle);
	//static int AddTranslatableTextInt(luax::Handle);
	//static int AddTranslatableTextIndexInt(luax::Handle);
	/**
	 * Put ammunition in a gun.
	 *
	 * @param integer gun
	 * @param integer ammo
	 */
	static int AmmoLoad(luax::Handle);
	/**
	 * Get the size of a clip.
	 *
	 * @return integer amount
	 * @param integer ammo
	 */
	static int GetClip(luax::Handle);
	/**
	 * Modify an object.
	 *
	 * @param integer object
	 * @param integer state
	 */
	static int SetModify(luax::Handle);
	/**
	 * Give EXP cookies.
	 *
	 * @param integer player
	 * @param number amount
	 * @param boolean reallyadd (optional)
	 */
	static int AddExp(luax::Handle);
	/**
	 * Check if a player has a trait.
	 *
	 * @return boolean isactive
	 * @param integer player
	 * @param integer trait
	 */
	static int HasTrait(luax::Handle);
	/**
	 * Check if a player is using an implant.
	 *
	 * @return boolean isactive
	 * @param integer player
	 * @param integer implant
	 */
	static int HasImplant(luax::Handle);
	/**
	 * Give hitpoints to an object.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param number amount
	 */
	static int HealObj(luax::Handle);
	/**
	 * Show research.
	 *
	 */
	static int Research(luax::Handle);
	/**
	 * Use a needed item for research.
	 *
	 * @return boolean used
	 * @param integer object
	 */
	static int ResearchConsume(luax::Handle);
	/**
	 * Return the name of an object's parent.
	 *
	 * @return string archetype
	 * @param integer object
	 */
	static int GetArchetypeName(luax::Handle);
	/**
	 * Return a player stat.
	 * Stats are "STR"(0), "END"(1), "PSI"(2), "AGI"(3), "CYB"(4)
	 *
	 * @return integer value
	 * @param integer player
	 * @param string|integer stat
	 */
	static int GetStat(luax::Handle);
	/**
	 * Return the object that the player is focused on.
	 *
	 * @return integer|nil object
	 */
	static int GetSelectedObj(luax::Handle);
	/**
	 * Return the object that the player is looking at.
	 *
	 * @return integer|nil object
	 */
	static int GetDistantSelectedObj(luax::Handle);
	/**
	 * Find a spawn point appropriate for creating a new object.
	 * The search will start at the location of the given object. See the Spawn property for flags.
	 *
	 * @return integer|nil marker
	 * @param integer object
	 * @param integer flags (optional)
	 */
	static int FindSpawnPoint(luax::Handle);
	/**
	 * Count the number of concrete objects with an EcoType property.
	 *
	 * @return number count
	 * @param integer eco_type
	 */
	static int CountEcoMatching(luax::Handle);
	/**
	 * Update player stats.
	 *
	 * @param integer player
	 */
	static int RecalcStats(luax::Handle);
	/**
	 * Show a movie.
	 *
	 * @param string name
	 */
	static int PlayVideo(luax::Handle);
	/**
	 * Stop radiation damage on the player.
	 *
	 */
	static int ClearRadiation(luax::Handle);
	/**
	 * Clear radiation on all objects except the player.
	 *
	 */
	static int RadiationHack(luax::Handle);
	/**
	 * Grab an object with the cursor.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int LoadCursor(luax::Handle);
	/**
	 * Something to do with the player's motion. I'm not sure what.
	 *
	 * @param number volume
	 */
	static int SetPlayerVolume(luax::Handle);
	/**
	 * A random number in the range [low,high]
	 *
	 * @return number result
	 * @param number low
	 * @param number high
	 */
	static int RandRange(luax::Handle);
	/**
	 * Add a multiplier to the player's speed.
	 *
	 * @param string name
	 * @param number speed
	 * @param number turnspeed (optional)
	 */
	static int AddSpeedControl(luax::Handle);
	/**
	 * Cancel a previously set speed control.
	 *
	 * @param string name
	 */
	static int RemoveSpeedControl(luax::Handle);
	/**
	 * Don't pick up the next object that is frobbed.
	 *
	 */
	static int PreventSwap(luax::Handle);
	/**
	 * Change the object state.
	 * Normal, Broken, Destroyed, Unresearched, Locked, Hacked
	 *
	 * @param integer object
	 * @param string|integer state
	 */
	static int SetObjState(luax::Handle);
	/**
	 * Cancel all alarms.
	 *
	 */
	static int DisableAlarmGlobal(luax::Handle);
	/**
	 * Set the alarm expire time.
	 *
	 * @param number milliseconds
	 */
	static int AddAlarm(luax::Handle);
	/**
	 * Cancel an alarm.
	 *
	 */
	static int RemoveAlarm(luax::Handle);
	/**
	 * Check for an alarm.
	 *
	 * @return boolean isactive
	 */
	static int IsAlarmActive(luax::Handle);
	/**
	 * Frob an object.
	 *
	 * @param boolean dontblock
	 */
	static int Frob(luax::Handle);
	/**
	 * Mark a region as visited.
	 *
	 * @param integer location
	 * @param boolean explored (optional)
	 */
	static int SetExplored(luax::Handle);
	/**
	 * Take an object out of a container.
	 *
	 * @param integer object
	 * @param integer container
	 */
	static int RemoveFromContainer(luax::Handle);
	/**
	 * Sets a quest variable using the name of the current map.
	 *
	 */
	static int ActivateMap(luax::Handle);
	/**
	 * Return the current sim time.
	 *
	 * @return number milliseconds
	 */
	static int SimTime(luax::Handle);
	/**
	 * Transition from a solid color to a normal view.
	 * Color is either a name or pixel value, or separate red, green, and blue components.
	 *
	 * @param number milliseconds
	 * @param string|integer color (optional)
	 * @param number red (optional)
	 * @param number green (optional)
	 * @param number blue (optional)
	 */
	static int StartFadeIn(luax::Handle);
	/**
	 * Fade the screen to a solid color.
	 * Color is either a name or pixel value, or separate red, green, and blue components.
	 *
	 * @param number milliseconds
	 * @param string|integer color (optional)
	 * @param number red (optional)
	 * @param number green (optional)
	 * @param number blue (optional)
	 */
	static int StartFadeOut(luax::Handle);
	/**
	 * Quick-and-dirty mode change.
	 *
	 * @param integer mode
	 */
	static int PlayerModeSimple(luax::Handle);
	/**
	 * Change the player mode.
	 * Modes are "Normal"(0), ..., "Dead"(7)
	 *
	 * @param integer mode
	 */
	static int PlayerMode(luax::Handle);
	/**
	 * Stops the game.
	 *
	 */
	static int EndGame(luax::Handle);
	/**
	 * Return whether the player should actually die.
	 * The player mode should not change when this is false.
	 *
	 * @return boolean isfatal
	 */
	static int AllowDeath(luax::Handle);
	/**
	 * Return a factor to multiply damage by.
	 * Argument is the END stat.
	 *
	 * @return number scale
	 * @param integer endurance
	 */
	static int GetHazardResistance(luax::Handle);
	/**
	 * Return amount of damage done by psi overload.
	 *
	 * @return integer damage
	 */
	static int GetBurnDmg(luax::Handle);
	/**
	 * Get the current weapon.
	 *
	 * @return integer|nil object
	 */
	static int PlayerGun(luax::Handle);
	/**
	 * Update the radar.
	 *
	 */
	static int PsiRadarScan(luax::Handle);
	/**
	 * Create an object that comes out of another object.
	 *
	 * @return integer|nil object
	 * @param integer source
	 * @param integer archetype
	 */
	static int PseudoProjectile(luax::Handle);
	/**
	 * Check if gore should be minimized.
	 *
	 * @return boolean noblood
	 */
	static int Censored(luax::Handle);
	/**
	 * Go to the debriefing screen.
	 *
	 * @param integer mission
	 */
	static int DebriefMode(luax::Handle);
	/**
	 * Show a message in a bordered box.
	 * The parameter is appended to the name.
	 *
	 * @param string name
	 * @param string file
	 * @param number index (optional)
	 */
	static int TlucTextAdd(luax::Handle);
	/**
	 * Change the mouse mode.
	 *
	 * @param boolean showcursor
	 * @param boolean drop
	 */
	static int Mouse(luax::Handle);
	/**
	 * Is the mouse in cursor mode?
	 *
	 * @return boolean visible
	 */
	static int MouseCursor(luax::Handle);
	/**
	 * Update the inventory display.
	 * Use when the size of the inventory may have changed.
	 *
	 */
	static int RefreshInv(luax::Handle);
	/**
	 * Randomly create the contents of a container.
	 *
	 * @param integer container
	 */
	static int TreasureTable(luax::Handle);
	/**
	 * Deletes everything in the player's inventory.
	 * Saves nanites and the "Magic Ball".
	 *
	 */
	static int VaporizeInv(luax::Handle);
	/**
	 * Shut-off all psi powers.
	 *
	 */
	static int ShutoffPsi(luax::Handle);
	/**
	 * Set a quest variable.
	 * "Note" variables are handled specially.
	 *
	 * @return boolean success
	 * @param string name
	 * @param number value
	 */
	static int SetQBHacked(luax::Handle);
	/**
	 * Halt all player movement.
	 *
	 * @param integer player
	 * @param boolean resetkeys (optional)
	 */
	static int ZeroControls(luax::Handle);
	/**
	 * Enable or disable player movement.
	 *
	 * @param boolean halt
	 */
	static int NoMove(luax::Handle);
	/**
	 * Check that an object is a gun.
	 *
	 * @return boolean isgun
	 * @param integer object.
	 */
	static int ValidGun(luax::Handle);
	/**
	 * Control the velocity of moving terrain.
	 *
	 * @param integer object
	 * @param integer terrpt
	 * @param number speed
	 */
	static int UpdateMovingTerrainVelocity(luax::Handle);

	static const char* const LogType[];
	static const char* const OverlayChangeMode[];
	static const char* const ObjStates[];
public:
	static const luax::Registry Methods[];
};
/**
 * ShockObjSrv
 */
class ShockObjService : protected ScriptServices
{
protected:
	/**
	 * Get the archetype that a script was inherited from.
	 *
	 * @return integer archetype
	 * @param integer object
	 * @param string script
	 */
	static int FindScriptDonor(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * ShockPsiSrv
 */
class ShockPsiService : protected ScriptServices
{
protected:
	/**
	 * Stop using a psi power.
	 *
	 * @param integer psi
	 */
	static int OnDeactivate(luax::Handle);
	/**
	 * Return how many milliseconds a psi power has been active.
	 *
	 * @return number milliseconds
	 * @param integer psi
	 */
	static int GetActiveTime(luax::Handle);
	/**
	 * Check if a psi power was overloaded.
	 *
	 * @return boolean isover
	 * @param integer psi
	 */
	static int IsOverloaded(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * ShockAISrv
 */
class ShockAIService : protected ScriptServices
{
protected:
	/**
	 * Stop an AI from moving unless it becomes damaged.
	 *
	 * @return boolean success
	 * @param integer ai
	 * @param string tags
	 * @param string tags
	 * @param number seconds
	 */
	static int Stun(luax::Handle);
	/**
	 * Check if an AI is immobile.
	 *
	 * @return boolean isstun
	 * @param integer ai
	 */
	static int IsStunned(luax::Handle);
	/**
	 * Make an AI move again.
	 *
	 * @return boolean success
	 * @param integer ai
	 */
	static int UnStun(luax::Handle);
	/**
	 * Make an AI immobile.
	 *
	 * @return boolean success
	 * @param integer ai
	 * @param number seconds
	 */
	static int Freeze(luax::Handle);
	/**
	 * Make an AI move again.
	 *
	 * @return boolean success
	 * @param integer ai
	 */
	static int UnFreeze(luax::Handle);
	/**
	 * An AI has triggered a door.
	 *
	 * @param integer ai
	 * @param integer door
	 */
	static int NotifyEnterTripwire(luax::Handle);
	/**
	 * An AI has vacated the door trigger.
	 *
	 * @param integer ai
	 * @param integer door
	 */
	static int NotifyExitTripwire(luax::Handle);
	/**
	 * Check if an object is locked.
	 * Ignores keys.
	 *
	 * @return boolean islocked
	 * @param integer object
	 */
	static int ObjectLocked(luax::Handle);
	/**
	 * Associate an object with a spawn point.
	 *
	 * @param integer object
	 * @param integer spawnpoint
	 */
	static int ValidateSpawn(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * ShockWeaponSrv
 */
class ShockWeaponService : protected ScriptServices
{
protected:
	/**
	 * Change the currently displayed gun.
	 *
	 * @param integer object
	 */
	static int SetWeaponModel(luax::Handle);
	/**
	 * Get the current gun.
	 *
	 * @return integer|nil object
	 */
	static int GetWeaponModel(luax::Handle);
	/**
	 * Search for an object that this one is facing.
	 *
	 * @return integer|nil destination
	 * @param integer source
	 */
	static int TargetScan(luax::Handle);
	/**
	 * Rotate an object to face another.
	 *
	 * @param integer source
	 * @param integer destination
	 */
	static int Home(luax::Handle);
	/**
	 * Stop using a melee weapon.
	 *
	 * @param integer object
	 */
	static int DestroyMelee(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * SoundSrv
 */
class SoundService : protected ScriptServices
{
protected:
	static const char* const EnvSoundLoc[];
	static const char* const SoundNetwork[];
};
class Sound1Service : protected SoundService
{
protected:
	static int Play(luax::Handle);
	static int PlayAmbient(luax::Handle);
	static int PlaySchema(luax::Handle);
	static int PlaySchemaAmbient(luax::Handle);
	static int PlayEnvSchema(luax::Handle);
	static int PlayVoiceOver(luax::Handle);
	static int Halt(luax::Handle);
	static int HaltSchema(luax::Handle);
	static int HaltSpeech(luax::Handle);
	static int PreLoad(luax::Handle);
public:
	static const luax::Registry Methods[];
};
class Sound2Service : protected SoundService
{
protected:
	/**
	 * Play schema tags.
	 * Networking option can be "normal", "broadcast", "localonly".
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string tags
	 * @param vector|integer location (optional)
	 * @param boolean loop (optional)
	 * @param string|integer networking (optional)
	 */
	static int Play(luax::Handle);
	/**
	 * Play ambient schema tags.
	 * Networking option can be "normal", "broadcast", "localonly".
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string tags
	 * @param boolean loop (optional)
	 * @param string|integer networking (optional)
	 */
	static int PlayAmbient(luax::Handle);
	/**
	 * Play a schema.
	 * Networking option can be "normal", "broadcast", "localonly".
	 *
	 * @return boolean success
	 * @param integer object
	 * @param integer schema
	 * @param vector|integer location (optional)
	 * @param string|integer networking (optional)
	 */
	static int PlaySchema(luax::Handle);
	/**
	 * Play an ambient schema.
	 * Networking option can be "normal", "broadcast", "localonly".
	 *
	 * @return boolean success
	 * @param integer object
	 * @param integer schema
	 * @param string|integer networking (optional)
	 */
	static int PlaySchemaAmbient(luax::Handle);
	/**
	 * Play environmental schema tags.
	 * Location can be "onobj", "atobjloc", "ambient".
	 * Networking option can be "normal", "broadcast", "localonly".
	 *
	 * @return boolean success
	 * @param integer object
	 * @param string tags
	 * @param integer source1 (optional)
	 * @param integer source2 (optional)
	 * @param string|integer location (optional)
	 * @param string|integer networking (optional)
	 */
	static int PlayEnvSchema(luax::Handle);
	/**
	 * Play a voice-over schema.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param integer schema
	 */
	static int PlayVoiceOver(luax::Handle);
	/**
	 * Stop schemas.
	 *
	 * @return number count
	 * @param integer source
	 * @param string schemas (optional)
	 * @param integer object (optional)
	 */
	static int Halt(luax::Handle);
	/**
	 * Stop schema tags.
	 *
	 * @return boolean success
	 * @param integer source
	 * @param string tags (optional)
	 * @param integer object (optional)
	 */
	static int HaltSchema(luax::Handle);
	/**
	 * Stop AI speech.
	 *
	 * @param integer ai
	 */
	static int HaltSpeech(luax::Handle);
	/**
	 * Prepare a sound so it can be played faster.
	 *
	 * @return boolean success
	 * @param string name
	 */
	static int PreLoad(luax::Handle);
public:
	static const luax::Registry Methods[];
};
/**
 * WeaponSrv
 */
class WeaponService : protected ScriptServices
{
protected:
	/**
	 * Wield a weapon.
	 *
	 * @return boolean success
	 * @param integer object
	 * @param boolean baton
	 */
	static int Equip(luax::Handle);
	/**
	 * Put away a weapon.
	 *
	 * @return boolean success
	 * @param integer object
	 */
	static int UnEquip(luax::Handle);
	/**
	 * Check if a weapon is being wielded.
	 *
	 * @return boolean success
	 * @param integer player
	 * @param integer object
	 */
	static int IsEquipped(luax::Handle);
	/**
	 * Swing a weapon.
	 *
	 * @return boolean success
	 * @param integer player
	 * @param integer object
	 */
	static int StartAttack(luax::Handle);
	/**
	 * Stop swinging a weapon.
	 *
	 * @return boolean success
	 * @param integer player
	 * @param integer object
	 */
	static int FinishAttack(luax::Handle);
public:
	static const luax::Registry Methods[];
};

/**
 * DarkHookSrv
 */
class DarkHookService : protected ScriptServices
{
protected:
	/**
	 * Load the Dark Hook library.
	 *
	 * @return boolean success
	 */
	static int DarkHookInitialize(luax::Handle);
	/**
	 * Listen for changes to a property.
	 *
	 * @return boolean success
	 * @param integer listener
	 * @param boolean post
	 * @param string property
	 * @param integer object
	 */
	static int InstallPropHook(luax::Handle);
	/**
	 * Stop listening to a property.
	 *
	 * @param integer listener
	 * @param string property
	 * @param integer object
	 */
	static int UninstallPropHook(luax::Handle);
	/**
	 * Listen for changes to links.
	 * Responds to links with the object as the source.
	 *
	 * @return boolean success
	 * @param integer listener
	 * @param boolean post
	 * @param string flavor
	 * @param integer object
	 */
	static int InstallRelHook(luax::Handle);
	/**
	 * Stop listening to a link.
	 *
	 * @param integer listener
	 * @param string flavor
	 * @param integer object
	 */
	static int UninstallRelHook(luax::Handle);
	/**
	 * Listen for changes to an object.
	 *
	 * @return boolean success
	 * @param integer listener
	 * @param boolean post
	 * @param integer object
	 */
	static int InstallObjHook(luax::Handle);
	/**
	 * Stop listening to an object.
	 *
	 * @param integer listener
	 * @param integer object
	 */
	static int UninstallObjHook(luax::Handle);
	/**
	 * Listen for changes to an archetype.
	 *
	 * @return boolean success
	 * @param integer listener
	 * @param boolean post
	 * @param integer object
	 */
	static int InstallHierarchyHook(luax::Handle);
	/**
	 * Stop listening to an archetype.
	 *
	 * @param integer listener
	 * @param integer object
	 */
	static int UninstallHierarchyHook(luax::Handle);

	enum DarkHookInitState { kDH_Uninitialized, kDH_Initialized, kDH_Failed };
	static DarkHookInitState sm_DarkHookInitialized;
	static void DHInit(luax::State&);
public:
	static const luax::Registry Methods[];
};

} // namespace Lgs

#endif // LGSERVICES_H
