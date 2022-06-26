#include "hive_hooks.h"

namespace HiveHook {
	void CleanUpLuaHooks() {
		HiveOriginalFunctions::RunStringExHook = 0;
		HiveOriginalFunctions::RunStringHook = 0;
		HiveOriginalFunctions::CompileStringHook = 0;
		delete HiveOriginalFunctions::RunStringExHook;
		HiveTroubleshooter::Print("Removed \"RunStringEx\" hook...", 1);
		delete HiveOriginalFunctions::RunStringHook;
		HiveTroubleshooter::Print("Removed \"RunString\" hook...", 1);
		delete HiveOriginalFunctions::CompileStringHook;
		HiveTroubleshooter::Print("Removed \"CompileString\" hook...", 1);
		HiveTroubleshooter::PrintInitizalization("Gracefully deallocated memory used by hooks!");
	}

	void GetRunString(void* state) {
		HiveOriginalFunctions::RunStringHook = new CHook((PDWORD*)state);
		HiveOriginalFunctions::RunString = (hive_func_RunString)HiveOriginalFunctions::RunStringHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::RunString,
			VMT_RUNSTRING);
	}
	
	void GetRunStringEx(void* state) {
		HiveOriginalFunctions::RunStringExHook = new CHook((PDWORD*)state);
		HiveOriginalFunctions::RunStringEx = (hive_func_RunStringEx)HiveOriginalFunctions::RunStringExHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::RunStringEx,
			VMT_RUNSTRINGEX);
	}

	void GetCompileString(void* state) {
		HiveOriginalFunctions::CompileStringHook = new CHook((PDWORD*)state);
		HiveOriginalFunctions::CompileString = (hive_func_CompileString)HiveOriginalFunctions::CompileStringHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::CompileString,
			VMT_COMPILESTRING);
	}

	void GetCreateLuaInterface() {
		HiveOriginalFunctions::CreateLuaInterfaceHook = new CHook((PDWORD*)CHiveInterface.LuaShared);
		HiveOriginalFunctions::CreateLuaInterface = (hive_func_CreateLuaInterface)HiveOriginalFunctions::CreateLuaInterfaceHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::CreateLuaInterface, 
			VMT_CREATEINTERFACE);
	}

	void GetCloseLuaInterface() {
		HiveOriginalFunctions::CloseLuaInterfaceHook = new CHook((PDWORD*)CHiveInterface.LuaShared);
		HiveOriginalFunctions::CloseLuaInterface = (hive_func_CloseLuaInterface)HiveOriginalFunctions::CloseLuaInterfaceHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::CloseLuaInterface,
			VMT_CLOSEINTERFACE);
	}

	void GetCreateMove() {
		HiveOriginalFunctions::CreateMoveHook = new CHook(**(PDWORD***)CHiveInterface.ClientMode);
		HiveOriginalFunctions::CreateMove = (hive_func_CreateMove)HiveOriginalFunctions::CreateMoveHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::CreateMove,
			VMT_CREATEMOVE);
	}

	void InitHive() {
		GetCreateLuaInterface();
		GetCloseLuaInterface();
		GetCreateMove();
	}
}