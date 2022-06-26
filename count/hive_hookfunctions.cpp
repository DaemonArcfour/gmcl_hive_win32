#include "hive_hookfunctions.h"
void* clientState = 0;

namespace HiveHookedFunctions {
	void * __fastcall RunString(void *edx, void *ecx, const char* ccName, const char* ccPath, const char* ccString, bool bRun, bool bErrors) {
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->GetField(-1, "hook");
		MENU->GetField(-1, "Call");
		MENU->PushString("hiveRunString");
		MENU->PushNil();
		MENU->PushString(ccName);
		MENU->PushString(ccPath);
		MENU->PushString(ccString);
		MENU->Call(5, 0);
		MENU->Pop(2);
		if (!CLuaMenuCallback.LuaExecution && !CLuaMenuCallback.CustomLuaRun)
			ccString = "print('Script stopped, lua execution disabled.')";
		CLuaMenuCallback.CustomLuaRun = false;
		return HiveOriginalFunctions::RunString(edx, ccName, ccPath, ccString, bRun, bErrors);
	}

	void * __fastcall RunStringEx(void *_this, void* edx, char const* ccName, char const* ccPath, char const* ccString, bool bRun, bool bErrors, bool UNK_1, bool UNK_2) {
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->GetField(-1, "hook");
		MENU->GetField(-1, "Call");
		MENU->PushString("hiveRunStringEx");
		MENU->PushNil();
		MENU->PushString(ccName);
		MENU->PushString(ccPath);
		MENU->PushString(ccString);
		MENU->Call(5, 0);
		MENU->Pop(2);
		if (!CLuaMenuCallback.LuaExecution && !CLuaMenuCallback.CustomLuaRun)
			ccString = "print('Script stopped, lua execution disabled.')";
		CLuaMenuCallback.CustomLuaRun = false;
		return HiveOriginalFunctions::RunStringEx(_this, ccName, ccPath, ccString, bRun, bErrors, UNK_1, UNK_2);
	}

	void * __fastcall CompileString(void *_this, void* edx, void* UNK_1, char const& ccString) {
		ConMsg("Compile string called.\n");
		return HiveOriginalFunctions::CompileString(_this, UNK_1, ccString);
	}

	void *__fastcall CloseLuaInterface(void *_this, void *edx, void* luaInterface) {
		void *LuaState = HiveOriginalFunctions::CloseLuaInterface(_this, luaInterface);

		if (luaInterface == clientState)
			clientState = 0;

		HiveTroubleshooter::PrintInitizalization("Lua interface destroyed, initializing garbage collector...");
		HiveHook::CleanUpLuaHooks();

		return LuaState;
	}

	void * __fastcall CreateLuaInterface(void *_this, void *, uchar stateType, bool renew) {
		void *LuaState = HiveOriginalFunctions::CreateLuaInterface(_this, stateType, renew);
		
		if (stateType != 0)
			return LuaState;

		HiveTroubleshooter::PrintInitizalization("Lua interface created, initializing hooks...");
		clientState = LuaState;

		HiveHook::GetRunString(clientState);
		HiveHook::GetRunStringEx(clientState);
		HiveHook::GetCompileString(clientState);

		return clientState;
	}

	bool __fastcall CreateMove(void* CClientModeShared, void* edx, float sample_input_frametime, GMODCUserCmd* ucmd) {
		bool bReturn = HiveOriginalFunctions::CreateMove(CClientModeShared, sample_input_frametime, ucmd);
		if (!ucmd->command_number)
				return bReturn;

		CHiveOptimize.InGame = CHiveInterface.Engine->IsInGame();
		CHiveOptimize.IsConnected = CHiveInterface.Engine->IsConnected();

		if (CHiveOptimize.InGame && CHiveOptimize.IsConnected) {
			if(CLuaMenuCallback.Bhop)
				HiveCheats::Bunnyhop(ucmd);
		}

		return bReturn;
	}

}

namespace HiveOriginalFunctions {
	CHook* RunStringHook = nullptr;
	CHook* RunStringExHook = nullptr;
	CHook* CompileStringHook = nullptr;
	CHook* CreateLuaInterfaceHook = nullptr;
	CHook* CloseLuaInterfaceHook = nullptr;
	CHook* CreateMoveHook = nullptr;
	hive_func_CompileString CompileString = 0;
	hive_func_RunString RunString = 0;
	hive_func_RunStringEx RunStringEx = 0;
	hive_func_CreateLuaInterface CreateLuaInterface = 0;
	hive_func_CloseLuaInterface CloseLuaInterface = 0;
	hive_func_CreateMove CreateMove = 0;
}
