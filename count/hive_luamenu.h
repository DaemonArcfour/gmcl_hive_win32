#pragma once
#include "hive_dependencies.h"

namespace HiveLuaMenuFunctions {
	void SetGmodWorkspace();
	void SetupLuaMenuCallbacks();

	int SetLuaExecution(lua_State* state);
	int GetbLuaExecution(lua_State* state);

	int SetESP(lua_State* state);
	int GetbESP(lua_State* state);

	int SetBhop(lua_State* state);
	int GetbBhop(lua_State* state);

	int SetLockDownloads(lua_State* state);
	int GetbLockDownloads(lua_State* state);

	int LuaRunMenu(lua_State* state);
	int LuaRunGame(lua_State* state);

	int WriteFile(lua_State* state);

	int GetServerIP(lua_State* state);

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
	bool ESP = false;
	bool Bhop = false;
	bool LockDownloads = true;
	bool CustomLuaRun = false;
}; LuaMenuCallback extern CLuaMenuCallback;