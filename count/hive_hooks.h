#pragma once
#include "hive_dependencies.h"

// LUA Shared VMT Offsets
#define VMT_RUNSTRING 92
#define VMT_RUNSTRINGEX 111
#define VMT_COMPILESTRING 117
#define VMT_CREATEINTERFACE 4
#define VMT_CLOSEINTERFACE 5
//-=-=-=-=
//Client DLL VMT Offsets
#define  VMT_CREATEMOVE 21
#define  VMT_PAINTTRAVERSE 41
//-=-=-=-=
//Material VMT Offsets
#define  VMT_READPIXELS 11


namespace HiveHook {
	void CleanUpLuaHooks();
	void GetRunString(void*);
	void GetRunStringEx(void*);
	void GetCompileString(void*);
	void GetCreateLuaInterface();
	void GetCloseLuaInterface();
	void InitHive();
}

extern GarrysMod::Lua::ILuaBase *MENU;