#include "hive_luamenu.h"
#include "hive_native.h"
#include "hive_protocol.h"
#include "inetchannel.h"
#include "hive_cheats.h"

DWORD *sig16 = 0;
LuaMenuCallback CLuaMenuCallback;
std::string ForcedName = "";

#define InitLUAFunction(MENU, name){\
MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);\
MENU->PushString(#name);\
MENU->PushCFunction(##name);\
MENU->SetTable(-3);\
HiveTroubleshooter::PrintLUA("Successfully added \""#name"\" function.");\
}

#define LuaToggleBool(name)\
	int Set##name(lua_State* state) {\
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))\
		{\
			CLuaMenuCallback.##name = MENU->GetBool(1);\
			if (CLuaMenuCallback.##name)\
				HiveTroubleshooter::Print(#name" enabled!", 1);\
			else\
				HiveTroubleshooter::Print(#name" disabled!", 1);\
			return 1;\
		}\
		\
		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);\
		\
		return 0;\
	}\
\
	int Getb##name(lua_State* state) {\
		MENU->PushBool(CLuaMenuCallback.##name);\
		return 1;\
	}\

#define LuaToggleBoolCustom(name, cfg)\
	int Set##name(lua_State* state) {\
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))\
		{\
			CLuaMenuCallback.##cfg = MENU->GetBool(1);\
			if (CLuaMenuCallback.##cfg)\
				HiveTroubleshooter::Print(#name" enabled!", 1);\
			else\
				HiveTroubleshooter::Print(#name" disabled!", 1);\
			return 1;\
		}\
		\
		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);\
		\
		return 0;\
	}\
\
	int Getb##name(lua_State* state) {\
		MENU->PushBool(CLuaMenuCallback.##cfg);\
		return 1;\
	}\


void OnChangeName(IConVar* iconv, const char* pOldValue, float flOldValue)
{
	iconv->SetValue(ForcedName.c_str());
}


void HiveLuaMenuFunctions::SetupLuaMenuCallbacks() {
	InitLUAFunction(MENU, SetLuaExecution);
	InitLUAFunction(MENU, GetbLuaExecution);
	InitLUAFunction(MENU, SetAutostrafe);
	InitLUAFunction(MENU, GetbAutostrafe);
	InitLUAFunction(MENU, SetAntiScreengrab);
	InitLUAFunction(MENU, SetESP);
	InitLUAFunction(MENU, GetbESP);
	InitLUAFunction(MENU, SetChams);
	InitLUAFunction(MENU, GetbChams);
	InitLUAFunction(MENU, SetBoneESP);
	InitLUAFunction(MENU, GetbBoneESP);
	InitLUAFunction(MENU, SetESPDist);
	InitLUAFunction(MENU, GetbESPDist);
	InitLUAFunction(MENU, SetESPName);
	InitLUAFunction(MENU, GetbESPName);
	InitLUAFunction(MENU, SetESPWeapon);
	InitLUAFunction(MENU, GetbESPWeapon);
	InitLUAFunction(MENU, SetESPHealth);
	InitLUAFunction(MENU, GetbESPHealth);
	InitLUAFunction(MENU, SetESPArmor);
	InitLUAFunction(MENU, GetbESPArmor);
	InitLUAFunction(MENU, SetESPBarrel);
	InitLUAFunction(MENU, GetbESPBarrel);
	InitLUAFunction(MENU, SetESPBarrelDist);
	InitLUAFunction(MENU, GetESPBarrelDist);
	InitLUAFunction(MENU, SetESPConnections);
	InitLUAFunction(MENU, GetbESPConnections);
	InitLUAFunction(MENU, SetESPBox);
	InitLUAFunction(MENU, GetbESPBox);
	InitLUAFunction(MENU, SetEntityESP);
	InitLUAFunction(MENU, GetbEntityESP);
	InitLUAFunction(MENU, SetEntityESPBox);
	InitLUAFunction(MENU, GetbEntityESPBox);
	InitLUAFunction(MENU, SetEntityESPConnections);
	InitLUAFunction(MENU, GetbEntityESPConnections);
	InitLUAFunction(MENU, SetEntityESPName);
	InitLUAFunction(MENU, GetbEntityESPName);
	InitLUAFunction(MENU, SetEntityESPList);
	InitLUAFunction(MENU, GetEntityESPList);
	InitLUAFunction(MENU, SetBhop);
	InitLUAFunction(MENU, GetbBhop);
	InitLUAFunction(MENU, SetAimbot);
	InitLUAFunction(MENU, GetbAimbot);
	InitLUAFunction(MENU, SetAimKey);
	InitLUAFunction(MENU, GetAimKey);
	InitLUAFunction(MENU, SetEnginePredict);
	InitLUAFunction(MENU, GetbEnginePredict);
	InitLUAFunction(MENU, SetPSilent);
	InitLUAFunction(MENU, GetbPSilent);
	InitLUAFunction(MENU, SetTriggerbot);
	InitLUAFunction(MENU, GetbTriggerbot);
	InitLUAFunction(MENU, SetNoRecoil);
	InitLUAFunction(MENU, GetbNoRecoil);
	InitLUAFunction(MENU, SetChoke);
	InitLUAFunction(MENU, GetChoke);
	InitLUAFunction(MENU, SetBacktrackMaxTicks);
	InitLUAFunction(MENU, GetBacktrackMaxTicks);
	InitLUAFunction(MENU, SetBacktrack);
	InitLUAFunction(MENU, GetbBacktrack);
	InitLUAFunction(MENU, SetAntiaim);
	InitLUAFunction(MENU, GetbAntiaim);
	InitLUAFunction(MENU, SetAntiaimType);
	InitLUAFunction(MENU, GetAntiaimType);
	InitLUAFunction(MENU, SetNoSpread);
	InitLUAFunction(MENU, GetbNoSpread);
	InitLUAFunction(MENU, SetChamsWireframe);
	InitLUAFunction(MENU, GetbChamsWireframe);
	InitLUAFunction(MENU, SetShowSpectators);
	InitLUAFunction(MENU, GetbShowSpectators);
	InitLUAFunction(MENU, SetLockDownloads);
	InitLUAFunction(MENU, GetbLockDownloads);
	InitLUAFunction(MENU, SetStopLuaGameModeHooks);
	InitLUAFunction(MENU, GetbStopLuaGameModeHooks);
	InitLUAFunction(MENU, LuaRunMenu);
	InitLUAFunction(MENU, LuaRunGame);
	InitLUAFunction(MENU, GetServerIP);
	InitLUAFunction(MENU, WriteFile);
	InitLUAFunction(MENU, ForceSetName);
	InitLUAFunction(MENU, Disconnect);
	InitLUAFunction(MENU, RC4_EncryptFile);
	InitLUAFunction(MENU, RC4_DecryptFile);
	InitLUAFunction(MENU, RunHiveScript);
	InitLUAFunction(MENU, UnlockAchivement);
	InitLUAFunction(MENU, GetOriginalFuncsTable);
	InitLUAFunction(MENU, GetIDs);
	InitLUAFunction(MENU, SetFriendList);
	InitLUAFunction(MENU, GetFriendList);
	InitLUAFunction(MENU, SetbSendPacket);
	InitLUAFunction(MENU, GetbSendPacket);
	InitLUAFunction(MENU, RequestInvalidFile);

	InitLUAFunction(MENU, GetObserversList);
}

namespace HiveLuaMenuFunctions {


	LuaToggleBool(AntiScreengrab);

	// Aimbot
	LuaToggleBool(EnginePredict);
	LuaToggleBool(Aimbot);
	LuaToggleBool(Triggerbot);
	LuaToggleBool(NoRecoil);
	LuaToggleBool(NoSpread);
	LuaToggleBool(PSilent);

	// Chams
	LuaToggleBool(Chams);
	LuaToggleBool(ChamsWireframe);

	// ESP
	LuaToggleBool(ESP);
	LuaToggleBool(ESPBarrel);
	LuaToggleBool(ESPConnections);
	LuaToggleBool(BoneESP);
	LuaToggleBool(ESPName);

	//	Entity ESP
	LuaToggleBool(EntityESP);
	LuaToggleBool(EntityESPBox);
	LuaToggleBool(EntityESPConnections);
	LuaToggleBool(EntityESPName);
	LuaToggleBool(ShowSpectators);
	LuaToggleBool(ESPWeapon);
	LuaToggleBool(ESPDist);
	LuaToggleBool(ESPHealth);
	LuaToggleBool(ESPArmor);

	// Misc

	LuaToggleBool(Bhop);

	LuaToggleBool(Antiaim);
	LuaToggleBool(Backtrack);
	LuaToggleBool(LockDownloads);

	LuaToggleBool(StopLuaGameModeHooks);
	LuaToggleBool(Autostrafe);
	LuaToggleBool(ESPBox);

	int SetLuaExecution(lua_State* state)
	{
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))
		{
			ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
			if (ClientLua) {
				ClientLua->RunStringEx("HiveScript", "", "_pcall = pcall pcall = function(...) end", 1, 1, 1, 1);
				ClientLua->RunStringEx("HiveScript", "", "_xpcall = xpcall xpcall = function(...) end", 1, 1, 1, 1);
				ClientLua->RunStringEx("HiveScript", "", "pcall = _pcall xpcall = _xpcall", 1, 1, 1, 1);
			}
			CLuaMenuCallback.LuaExecution = MENU->GetBool(1);
			if (CLuaMenuCallback.LuaExecution)
				HiveTroubleshooter::Print("LUA execution enabled!", 1);
			else
				HiveTroubleshooter::Print("LUA execution disabled!", 1);
			return 1;
		}
		
		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);

		return 0;
	}

	int GetbLuaExecution(lua_State* state) {
		MENU->PushBool(CLuaMenuCallback.LuaExecution);
		return 1;
	}

	

	int SetESPBarrelDist(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
		{
			CLuaMenuCallback.ESPBarrelDist = MENU->GetNumber(1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts integer variables.", 0);

		return 0;
	}

	int GetESPBarrelDist(lua_State* state) {
		MENU->PushNumber(CLuaMenuCallback.ESPBarrelDist);
		return 1;
	}

	int SetbSendPacket(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))
		{
			*HiveCheats::bSendPacket = MENU->GetBool(1);
			if (*HiveCheats::bSendPacket)
				HiveTroubleshooter::Print("bSendPacket enabled!", 1);
			else
				HiveTroubleshooter::Print("bSendPacket disabled!", 1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);

		return 0;
	}

	int GetbSendPacket(lua_State* state) {
		MENU->PushBool(*HiveCheats::bSendPacket);
		return 1;
	}


	int SetChoke(lua_State* state) 
	{
		if (MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
		{
			CLuaMenuCallback.Choke = MENU->GetNumber(1);
			HiveTroubleshooter::Print("Choke set to " + std::to_string(CLuaMenuCallback.Choke) + "!", 1);
		}

		else
			HiveTroubleshooter::Print("This function only accepts int variables.", 0);

		return 0;
	}

	int GetChoke(lua_State* state) {
		MENU->PushNumber(CLuaMenuCallback.Choke);
		return 1;
	}

	int SetAimKey(lua_State* state)
	{
		if (MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
		{
			CLuaMenuCallback.AimbotKey = MENU->GetNumber(1);
			HiveTroubleshooter::Print("AimKey set to " + std::to_string(CLuaMenuCallback.Choke) + "!", 1);
		}

		else
			HiveTroubleshooter::Print("This function only accepts int variables.", 0);

		return 0;
	}

	int GetAimKey(lua_State* state) {
		MENU->PushNumber(CLuaMenuCallback.AimbotKey);
		return 1;
	}

	int SetBacktrackMaxTicks(lua_State* state)
	{
		if (MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
		{
			CLuaMenuCallback.Backtrack_max_tick = MENU->GetNumber(1);
			HiveTroubleshooter::Print("Backtrack max ticks set to " + std::to_string(CLuaMenuCallback.Backtrack_max_tick) + "!", 1);
		}

		else
			HiveTroubleshooter::Print("This function only accepts int variables.", 0);

		return 0;
	}

	int GetBacktrackMaxTicks(lua_State* state) {
		MENU->PushNumber(CLuaMenuCallback.Backtrack_max_tick);
		return 1;
	}

	int SetAntiaimType(lua_State* state) 
	{
		if (MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
		{
			CLuaMenuCallback.AntiaimType = MENU->GetNumber(1);
			HiveTroubleshooter::Print("Antiaim type set to " + std::to_string(CLuaMenuCallback.AntiaimType) + "!", 1);
		}

		else
			HiveTroubleshooter::Print("This function only accepts int variables.", 0);

		return 0;
	}

	int GetAntiaimType(lua_State* state) {
		MENU->PushNumber(CLuaMenuCallback.AntiaimType);
		return 1;
	}
}

namespace HiveLuaMenuFunctions {
	int LuaRunMenu(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::STRING))
		{
			ILuaInterface* MenuLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_MENU);
			if (!MenuLua) {
				HiveTroubleshooter::Print("Game is not loaded yet.", 0);
				return 0;
			}

			MenuLua->RunStringEx("HiveScript", "", MENU->GetString(1), 1, 1, 1, 1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts string variables.", 0);

		return 0;
	}

	int LuaRunGame(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::STRING))
		{
			ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
			if (!ClientLua) {
				HiveTroubleshooter::Print("Not in game.", 0);
				return 0;
			}
			CLuaMenuCallback.CustomLuaRun = true;
			ClientLua->RunStringEx("HiveScript", "", MENU->GetString(1), 1, 1, 1, 1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts string variables.", 0);

		return 0;
	}

	int GetServerIP(lua_State* state) {
		char* ServerIP = HiveSiggedFunctions::GetServerIP();
		if (!ServerIP) {
			HiveTroubleshooter::Print("Not connected to any server.", 0);
			return 0;
		}
		MENU->PushString(ServerIP);

		return 1;
	}

	int WriteFile(lua_State* state) {
		if (!MENU->IsType(1, GarrysMod::Lua::Type::STRING))
			return 0;
		if (!MENU->IsType(2, GarrysMod::Lua::Type::STRING))
			return 0;


		const char* Path = MENU->GetString(1);
		const char* Cont = MENU->GetString(2);
		if (strstr(Path, "..\\") || strstr(Path, "../")) {
			HiveTroubleshooter::Print("Fuck you.", ERROR);
			return 0;
		}
		std::string fullPath = std::string(CLuaMenuCallback.GmodWorkspace) + std::string(Path);
		std::string toPrint = "Writing file: " + fullPath;
		std::string toPrintError = "Failed writing file: " + fullPath;
		std::fstream pFile;
		pFile.open(fullPath, std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);
		if (pFile.is_open()) {
			HiveTroubleshooter::Print(toPrint, OK);
			pFile << Cont;
			MENU->Push(true);
			return 1;
		}
		else {
			HiveTroubleshooter::Print(toPrintError, ERROR);
			MENU->Push(false);
			return 1;
		}
		pFile.close();
	}

	int RunHiveScript(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::STRING) && MENU->IsType(2, GarrysMod::Lua::Type::NUMBER))
		{
			ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);

			CLuaMenuCallback.CustomLuaRun = true;
			int method = MENU->GetNumber(2);
			if (method != 0 && method != 1)
				return 0;
			if (method == 0)
				if (!ClientLua) {
					HiveTroubleshooter::Print("Not in game.", 0);
					return 0;
				}
			SafeHiveScriptExecution(MENU->GetString(1), method);
			return 1;
		}

		HiveTroubleshooter::Print("RunHiveScript(string path, number method) 0 = Client State, 1 = Menu State.", 0);

		return 0;
	}

	int GetOriginalFuncsTable(lua_State* state) {
		ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);

		if (!ClientLua)
			return 0;
		HiveLUA::RecycleFunctionList(LUA_Functions);
		HiveLUA::GetFunctions(ClientLua, LUA_Functions);
		std::vector<COriginalFunction*> LUA_Overridden;
		HiveLUA::CompareFunctionLists(LUA_OriginalFunctions, LUA_Functions, LUA_Overridden);
		int OTableSize = LUA_OriginalFunctions.size() - 1;
		ConMsg("Overridden functions: \n");
		for (COriginalFunction* i : LUA_Overridden) {
			ConMsg("Function [%s]   Original [%x]   New [%x]\n", i->function_name.c_str(), i->original_address, i->new_address);
		}
		HiveLUA::RecycleFunctionList(LUA_Overridden);
		return 1;
	}

	void SetGmodWorkspace() {
		TCHAR szExeFileName[MAX_PATH];
		ZeroMemory(szExeFileName, MAX_PATH);
		GetModuleFileName(NULL, szExeFileName, MAX_PATH);
		CLuaMenuCallback.GmodWorkspace = szExeFileName;
		int cut = CLuaMenuCallback.GmodWorkspace.find("hl2.exe", 0);
		CLuaMenuCallback.GmodWorkspace = CLuaMenuCallback.GmodWorkspace.substr(0, cut);
		CLuaMenuCallback.GmodWorkspace = CLuaMenuCallback.GmodWorkspace.append("garrysmod\\");
	}
}

// Lua Lists

namespace HiveLuaMenuFunctions {
	int GetIDs(lua_State* state)
	{
		MENU->CreateTable();
		int i = 1;
		for (int index = CHiveInterface.Engine->GetMaxClients(); index >= 0; --index)
		{
			if (!CHiveInterface.Engine->IsInGame()) break;

			player_info_t info;
			if (index == CHiveInterface.Engine->GetLocalPlayer())
				continue;

			C_BasePlayerNew* Player = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(index);

			if (!Player || !CHiveInterface.Engine->GetPlayerInfo(((CBaseEntityNew*)Player)->Index(), &info))
				continue;

			CSteamID SteamID;
			Player->GetSteamID(&SteamID);
			MENU->PushNumber(i);
			MENU->CreateTable();

			MENU->PushString("steamid3");
			MENU->PushString(SteamID.Render());
			MENU->SetTable(-3);

			MENU->PushString("steamid64");
			MENU->PushString(std::to_string(SteamID.ConvertToUint64()).c_str());
			MENU->SetTable(-3);

			MENU->PushString("name");
			MENU->PushString(info.name);
			MENU->SetTable(-3);

			MENU->PushString("userid");
			MENU->PushNumber(info.userID);
			MENU->SetTable(-3);


			MENU->SetTable(-3);
			i++;
		}

		return 1;
	}

	int GetEntityESPList(lua_State* state) {
		MENU->CreateTable();

		int i = 1;
		for (auto itr = CLuaMenuCallback.EntityESPList.begin(); itr != CLuaMenuCallback.EntityESPList.end(); ++itr) {
			MENU->PushNumber(i);
			MENU->PushString(itr->c_str());
			MENU->SetTable(-3);
			i++;
		}

		return 1;
	}

	int SetEntityESPList(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::TABLE)) {
			MENU->Push(1);
			MENU->PushNil();
			CLuaMenuCallback.EntityESPList.clear();


			while (MENU->Next(-2)) {
				//-1 == value
				//-2 == key

				if (MENU->IsType(-1, GarrysMod::Lua::Type::STRING)) {
					CLuaMenuCallback.EntityESPList.insert(MENU->GetString(-1));
				}

				MENU->Pop();
			}

			MENU->Pop();

			return 0;
		}

		HiveTroubleshooter::Print("This function only accepts table.", 0);

		return 0;
	}

	int GetFriendList(lua_State* state) {
		MENU->CreateTable();

		int i = 1;
		for (auto itr = CLuaMenuCallback.FriendList.begin(); itr != CLuaMenuCallback.FriendList.end(); ++itr) {
			MENU->PushNumber(i);
			MENU->PushString(std::to_string(*itr).c_str());
			MENU->SetTable(-3);
			i++;
		}

		return 1;
	}

	int SetFriendList(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::TABLE)) {
			MENU->Push(1);
			MENU->PushNil();
			CLuaMenuCallback.FriendList.clear();


			while (MENU->Next(-2)) {
				//-1 == value
				//-2 == key

				if (MENU->IsType(-1, GarrysMod::Lua::Type::STRING)) {
					CLuaMenuCallback.FriendList.insert(atoll(MENU->GetString(-1)));
				}

				MENU->Pop();
			}

			MENU->Pop();

			return 0;
		}

		HiveTroubleshooter::Print("This function only accepts table.", 0);

		return 0;
	}

	int GetObserversList(lua_State* state) {
		MENU->CreateTable();

		if (!CHiveInterface.Engine->IsInGame()) return 1;
		
		C_BasePlayerNew* LocalPlayer = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		
		int cnt = 0;
		for (int i = 0; i < MaxClients; i++)
		{
			C_BasePlayerNew* target = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(i);
			player_info_t info;
			if (target) {
				if (CHiveInterface.Engine->GetPlayerInfo(i, &info)) {
					C_BasePlayerNew* Observer = (C_BasePlayerNew*)CHiveSourceNative.UTIL_GetPlayerByIndex(i);
					if (Observer) {
						if ((C_BasePlayerNew*)Observer->GetObserverTarget() == LocalPlayer) {
							cnt++;
							MENU->PushNumber(cnt);
							MENU->CreateTable();

							player_info_t info;
							CSteamID steamid;

							if (!CHiveInterface.Engine->GetPlayerInfo(((CBaseEntityNew*)Observer)->Index(), &info)) continue;

							Observer->GetSteamID(&steamid);

							MENU->PushString("name");
							MENU->PushString(info.name);
							MENU->SetTable(-3);

							MENU->PushString("steamid64");
							MENU->PushString(std::to_string(steamid.ConvertToUint64()).c_str());
							MENU->SetTable(-3);

							MENU->SetTable(-3);
						}
					}
				}
			}

		}

		return 1;
	}
}

// Open SSL

namespace HiveLuaMenuFunctions {
	int RC4_EncryptFile(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::STRING))
		{
			RC4EncryptFile(MENU->GetString(1), true);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts string variables.", 0);

		return 0;
	}

	int RC4_DecryptFile(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::STRING))
		{
			RC4DecryptFile(MENU->GetString(1), true);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts string variables.", 0);

		return 0;
	}
}

// Exploits

namespace HiveLuaMenuFunctions {
	int RequestInvalidFile(lua_State* state) {
		if (!CHiveInterface.Engine->IsConnected())
		{
			HiveTroubleshooter::Print("Not connected to any server.", 0);
			return 0;
		}

		if (!MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
			return 0;

		int power = MENU->GetNumber(1);
		INetChannel* ch = (INetChannel*)CHiveInterface.Engine->GetNetChannelInfo();
		char pckBuf[5024];
		ZeroMemory(pckBuf, sizeof(pckBuf));
		bf_write pck(pckBuf, sizeof(pckBuf));
		for (int i = 1; i < power; i++)
		{
			if (pck.GetNumBytesLeft() < 100)
				break;
			pck.WriteUBitLong(net_File, NETMSG_TYPE_BITS);
			pck.WriteUBitLong(i, 32); // transfer id
			pck.WriteOneBit(1);	// we are requesting the file
			pck.WriteOneBit(DOWNLOADABLE); // downloadable
			pck.WriteUBitLong(i, 32);	// file id

		}
		for (int i = 0; i < power; i++)
		{
			ch->SendData(pck, false);
		}

		return 1;
	}

	int ForceSetName(lua_State* state) {
		if (!MENU->IsType(1, GarrysMod::Lua::Type::STRING))
			return 0;
		ForcedName = MENU->GetString(1);
		ConVar* LockedVar = CHiveInterface.Cvar->FindVar("name");
		LockedVar->InstallChangeCallback(&OnChangeName);
		LockedVar->SetValue(ForcedName.c_str());
		return 1;
	}

	int UnlockAchivement(lua_State* state) {
		if (!MENU->IsType(1, GarrysMod::Lua::Type::NUMBER))
			return 0;
		int id = MENU->GetNumber(1);
		KeyValues* keyval = new KeyValues("AchievementEarned");
		keyval->SetInt("achievementID", id);
		CHiveInterface.Engine->ServerCmdKeyValues(keyval);
	}

	int Disconnect(lua_State* state) {
		if (!CHiveInterface.Engine->IsConnected()) {
			HiveTroubleshooter::Print("Not connected to any server.", 0);
			return 0;
		}

		if (!MENU->IsType(1, GarrysMod::Lua::Type::STRING))
			return 0;
		INetChannel* ch = (INetChannel*)CHiveInterface.Engine->GetNetChannelInfo();
		char pckBuf[5024];
		bf_write pck(pckBuf, sizeof(pckBuf));
		std::string reason = MENU->GetString(1);;
		pck.WriteUBitLong(1, 6);
		pck.WriteString(reason.c_str());
		ch->SendData(pck, false);
		return 1;
	}
}