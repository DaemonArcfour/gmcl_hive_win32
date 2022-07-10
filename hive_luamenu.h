#pragma once
#include "hive_dependencies.h"
extern DWORD *sig16;
namespace HiveLuaMenuFunctions {
	void SetGmodWorkspace();
	void SetupLuaMenuCallbacks();

	int SetLuaExecution(lua_State* state);
	int GetbLuaExecution(lua_State* state);
	int SetChoke(lua_State* state);
	int GetChoke(lua_State* state);
	int SetAntiScreengrab(lua_State* state);
	int SetESP(lua_State* state);
	int SetBoneESP(lua_State * state);
	int GetbBoneESP(lua_State * state);
	int GetbESP(lua_State* state);
	int SetESPWeapon(lua_State* state);
	int SetESPName(lua_State* state);
	int SetESPDist(lua_State* state);
	int SetESPBarrel(lua_State* state);
	int SetESPBarrelDist(lua_State* state);
	int SetESPConnections(lua_State* state);
	int SetESPHealth(lua_State* state);
	int SetESPBox(lua_State* state);
	int SetESPArmor(lua_State* state);
	int SetChams(lua_State* state);
	int GetbChams(lua_State* state);
	int SetPSilent(lua_State* state);
	int GetbPSilent(lua_State* state);
	int GetbESPWeapon(lua_State* state);
	int GetbESPName(lua_State* state);
	int GetbESPDist(lua_State* state);
	int GetbESPHealth(lua_State* state);
	int GetbESPArmor(lua_State* state);
	int SetStopLuaGameModeHooks(lua_State* state);
	int GetbStopLuaGameModeHooks(lua_State* state);
	int GetbESPBox(lua_State* state);
	int GetbESPConnections(lua_State* state);
	int GetbESPBarrel(lua_State* state);
	int GetESPBarrelDist(lua_State* state);
	
	// Entity ESP
	int SetEntityESP(lua_State* state);
	int GetbEntityESP(lua_State* state);
	int SetEntityESPBox(lua_State* state);
	int GetbEntityESPBox(lua_State* state);
	int SetEntityESPConnections(lua_State* state);
	int GetbEntityESPConnections(lua_State* state);
	int SetEntityESPName(lua_State* state);
	int GetbEntityESPName(lua_State* state);
	int SetEntityESPList(lua_State*);
	int GetEntityESPList(lua_State*);

	// Misc
	int SetEnginePredict(lua_State* state);
	int GetbEnginePredict(lua_State* state);
	int SetAimbot(lua_State* state);
	int GetbAimbot(lua_State* state);
	int SetAutostrafe(lua_State* state);
	int GetbAutostrafe(lua_State* state);
	int SetTriggerbot(lua_State* state);
	int GetbTriggerbot(lua_State* state);
	int SetBhop(lua_State* state);
	int GetbBhop(lua_State* state);
	int SetLockDownloads(lua_State* state);
	int GetbLockDownloads(lua_State* state);
	int SetNoRecoil(lua_State* state);
	int GetbNoRecoil(lua_State* state);
	int SetNoSpread(lua_State* state);
	int GetbNoSpread(lua_State* state);
	int SetChamsWireframe(lua_State* state);
	int GetbChamsWireframe(lua_State* state);
	int SetShowSpectators(lua_State* state);
	int GetbShowSpectators(lua_State* state);
	int LuaRunMenu(lua_State* state);
	int LuaRunGame(lua_State* state);
	int SetAimKey(lua_State* state);
	int GetAimKey(lua_State* state);

	int SetFriendList(lua_State*);
	int GetFriendList(lua_State*);

	int SetbSendPacket(lua_State*);
	int GetbSendPacket(lua_State*);

	int WriteFile(lua_State* state);

	int GetServerIP(lua_State* state);

	int Disconnect(lua_State* state);
	int ForceSetName(lua_State* state);

	int RC4_EncryptFile(lua_State* state);
	int RC4_DecryptFile(lua_State* state);
	int GetOriginalFuncsTable(lua_State* state);
	int RunHiveScript(lua_State* state);
	int GetIDs(lua_State* state);
	int UnlockAchivement(lua_State* state);
	int RequestInvalidFile(lua_State* state);

	int SetBacktrack(lua_State* state);
	int GetbBacktrack(lua_State* state);
	int SetBacktrackMaxTicks(lua_State* state);
	int GetBacktrackMaxTicks(lua_State* state);

	int SetAntiaim(lua_State* state);
	int GetbAntiaim(lua_State* state);
	int SetAntiaimType(lua_State* state);
	int GetAntiaimType(lua_State* state);
}
enum
{
	LUA_CLIENT = 0,
	LUA_SERVER = 1,
	LUA_MENU = 2
};

class LuaMenuCallback {
public:
	std::string GmodWorkspace = "\0";
	bool LuaExecution = true;
	bool Autostrafe = false;
	bool Backtrack = false;
	bool StopLuaGameModeHooks = false;
	bool Antiaim = false;
	int	AntiaimType = 0;
	int AimbotKey = 0;
	bool ESP = false;
	bool BoneESP = false;
	bool Bhop = false;
	bool EnginePredict = true;
	bool Aimbot = false;
	bool Triggerbot = false;
	bool NoRecoil = false;
	bool NoSpread = false;
	int Choke = 0;
	int Backtrack_max_tick = 8;
	bool PSilent = false;
	bool ESPName = true;
	bool ESPArmor = true;
	bool ESPHealth = true;
	bool ESPBox = true;
	bool ESPWeapon = true;
	bool ESPConnections = false;
	bool ESPBarrel = false;
	int  ESPBarrelDist = 300;
	bool ESPDist = false;
	bool ESPDormant = true;

	// Entity ESP
	bool EntityESP = false;
	bool EntityESPBox = true;
	bool EntityESPConnections = false;
	bool EntityESPName = true;
	std::unordered_set<std::string> EntityESPList;

	bool AntiScreengrab = true;
	bool Chams = false;
	bool ChamsWireframe = false;
	bool ShowSpectators = true;
	bool LockDownloads = true;
	bool CustomLuaRun = false;

	// Friends List
	std::unordered_set<unsigned long long> FriendList;
}; LuaMenuCallback extern CLuaMenuCallback;