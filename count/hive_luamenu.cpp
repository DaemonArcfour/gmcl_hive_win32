#include "hive_luamenu.h"

LuaMenuCallback CLuaMenuCallback;
namespace HiveLuaMenuFunctions {
	void SetupLuaMenuCallbacks() {
		//SetLuaExecution
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("SetLuaExecution");
		MENU->PushCFunction(SetLuaExecution);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"SetLuaExecution\" function.");

		//GetbLuaExecution
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("GetbLuaExecution");
		MENU->PushCFunction(GetbLuaExecution);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"GetbLuaExecution\" function.");

		//SetESP
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("SetESP");
		MENU->PushCFunction(SetESP);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"SetESP\" function.");

		//GetESP
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("GetbESP");
		MENU->PushCFunction(GetbESP);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"GetbESP\" function.");

		//SetBhop
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("SetBhop");
		MENU->PushCFunction(SetBhop);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"SetBhop\" function.");

		//GetBhop
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("GetbBhop");
		MENU->PushCFunction(GetbBhop);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"GetbBhop\" function.");

		//SetLockDownloads
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("SetLockDownloads");
		MENU->PushCFunction(SetLockDownloads);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"SetLockDownloads\" function.");

		//GetbLockDownloads
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("GetbLockDownloads");
		MENU->PushCFunction(GetbLockDownloads);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"GetbLockDownloads\" function.");

		//LuaRunMenu
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("LuaRunMenu");
		MENU->PushCFunction(LuaRunMenu);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"LuaRunMenu\" function.");

		//LuaRunGame
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("LuaRunGame");
		MENU->PushCFunction(LuaRunGame);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"LuaRunGame\" function.");

		//GetServerIP
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("GetServerIP");
		MENU->PushCFunction(GetServerIP);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"GetServerIP\" function.");

		//WriteFile
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->PushString("WriteFile");
		MENU->PushCFunction(WriteFile);
		MENU->SetTable(-3);
		HiveTroubleshooter::PrintLUA("Successfully added \"WriteFile\" function.");

	}
	int SetLuaExecution(lua_State* state)
	{
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))
		{
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

	int SetESP(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))
		{
			CLuaMenuCallback.ESP = MENU->GetBool(1);
			if (CLuaMenuCallback.ESP)
				HiveTroubleshooter::Print("ESP enabled!", 1);
			else
				HiveTroubleshooter::Print("ESP disabled!", 1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);

		return 0;
	}

	int GetbESP(lua_State* state) {
		MENU->PushBool(CLuaMenuCallback.ESP);
		return 1;
	}

	int SetBhop(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))
		{
			CLuaMenuCallback.Bhop = MENU->GetBool(1);
			if (CLuaMenuCallback.Bhop)
				HiveTroubleshooter::Print("Bhop enabled!", 1);
			else
				HiveTroubleshooter::Print("Bhop disabled!", 1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);

		return 0;
	}

	int GetbBhop(lua_State* state) {
		MENU->PushBool(CLuaMenuCallback.Bhop);
		return 1;
	}


	int SetLockDownloads(lua_State* state) {
		if (MENU->IsType(1, GarrysMod::Lua::Type::BOOL))
		{
			CLuaMenuCallback.LockDownloads = MENU->GetBool(1);
			if (CLuaMenuCallback.LockDownloads)
				HiveTroubleshooter::Print("hive_downloaded is protected!", 1);
			else
				HiveTroubleshooter::Print("hive_downloaded is unprotected!", 1);
			return 1;
		}

		HiveTroubleshooter::Print("This function only accepts bool variables.", 0);

		return 0;
	}

	int GetbLockDownloads(lua_State* state) {
		MENU->PushBool(CLuaMenuCallback.LockDownloads);
		return 1;
	}

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

	int WriteFile(lua_State* state){
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