#include "hive_dependencies.h"

using namespace GarrysMod::Lua;
ILuaBase *MENU;

GMOD_MODULE_OPEN()
{
	MENU = LUA;
	
	HiveTroubleshooter::Print("gmcl_hive_win32.dll ( Revision: [" + std::string(__DATE__) + "] [" + std::string(__TIME__) + "] )", OK);
	HiveTroubleshooter::PrintOpenSSL("Grabbing OpenSSL GLOBAL_ENCRYPTION_KEY...");
	if (OpenSSL_GetGlobalEncryptionKey()) {
		HiveTroubleshooter::Print("GLOBAL_ENCRYPTION_KEY was set!", OK);
	}
	else {
		HiveTroubleshooter::PrintOpenSSL("Couldn't find key file, aborting execution.");
		return 0;
	}
	HiveTroubleshooter::PrintOpenSSL("RC4 loaded.");
	
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
	HiveNetVarOffsets::GetOffsets();
	if (HiveTroubleshooter::netvarErrors()) {
		HiveTroubleshooter::Warning();
		return 0;
	}
	
	HiveTroubleshooter::PrintInitizalization("Initializing native functions troubleshooter...");
	if (HiveTroubleshooter::nativeErrors()) {
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

	HiveTroubleshooter::PrintOpenSSL("Checking buffer folders existance...");
	
	std::string CheckDir1 = CLuaMenuCallback.GmodWorkspace + std::string("lua/protected_scripts/");
	if (!dirExists(CheckDir1)) {
		HiveTroubleshooter::PrintOpenSSL("\"unprotected_scripts\" folder was not found, creating one.");
		CreateDirectory(CheckDir1.c_str(), 0);
	}
	std::string CheckDir2 = CLuaMenuCallback.GmodWorkspace + std::string("lua/protected_scripts/");
	if (!dirExists(CheckDir2)) {
		HiveTroubleshooter::PrintOpenSSL("\"protected_scripts\" folder was not found, creating one.");
		CreateDirectory(CheckDir2.c_str(), 0);
	}
	/*
	ProtectScripts();
	*/

	HiveTroubleshooter::PrintInitizalization("Done loading!");
	HiveTroubleshooter::PrintInitizalization("[C++] Technical support: https://steamcommunity.com/id/unsigned__int32");
	HiveTroubleshooter::PrintInitizalization("[LUA] Technical support: https://steamcommunity.com/id/AppleJeb/");
	HiveTroubleshooter::PrintInitizalization("Protector powered by OpenSSL [https://www.openssl.org/]");
	//PlaySoundW(L"HiveSounds\\loaded.wav", NULL, SND_ASYNC);
	
	return 0;
}


GMOD_MODULE_CLOSE()
{
	exit(1);
	return 0;
}