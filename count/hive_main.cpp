#include "hive_dependencies.h"


using namespace GarrysMod::Lua;
ILuaBase *MENU;

GMOD_MODULE_OPEN()
{
	MENU = LUA;
	HiveTroubleshooter::Print("gmcl_hive_win32.dll (BUILD: [8/18/2017])", OK);
	HiveTroubleshooter::PrintInitizalization("Initializing factories troubleshooter...");
	if (HiveTroubleshooter::factoryErrors()) {
		HiveTroubleshooter::Warning();
		return 0;
	}

	HiveTroubleshooter::PrintInitizalization("Initializing interfaces troubleshooter...");
	if (HiveTroubleshooter::interfaceErrors()) {
		HiveTroubleshooter::Warning();
		return 0;
	}

	HiveTroubleshooter::PrintInitizalization("Initializing hooks troubleshooter...");
	HiveHook::InitHive();
	if (HiveTroubleshooter::hookErrors()) {
		HiveTroubleshooter::Warning();
		return 0;
	}
	HiveTroubleshooter::PrintInitizalization("Initializing netvars troubleshooter...");
	if (HiveTroubleshooter::netvarErrors()) {
		HiveTroubleshooter::Warning();
		return 0;
	}

	HiveTroubleshooter::PrintInitizalization("Setting GMod workspace directory...");
	HiveLuaMenuFunctions::SetGmodWorkspace();
	
	if (!CLuaMenuCallback.GmodWorkspace.size()) {
		HiveTroubleshooter::Warning();
		return 0;
	}
	else {
		HiveTroubleshooter::Print(CLuaMenuCallback.GmodWorkspace, OK);
	}

	HiveTroubleshooter::PrintInitizalization("Initializing lua menu callbacks...");
	HiveLuaMenuFunctions::SetupLuaMenuCallbacks();
	HiveTroubleshooter::PrintInitizalization("Initializing module protector troubleshooter...");
	HiveProtection::SetupProtection();

	if (HiveTroubleshooter::protectorErrors()) {
		HiveTroubleshooter::Warning();
		return 0;
	}

	HiveTroubleshooter::PrintInitizalization("Done loading!");
	HiveTroubleshooter::PrintInitizalization("[C++] Technical support: https://steamcommunity.com/id/unsigned__int32");
	HiveTroubleshooter::PrintInitizalization("[LUA] Technical support: http://steamcommunity.com/id/el_leet/");
	return 0;
}


GMOD_MODULE_CLOSE()
{
	return 0;
}