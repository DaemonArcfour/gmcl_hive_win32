#include "hive_hooks.h"
#include "hive_native.h"
CVMT VMT;
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

	void GetCModeCreateMove() {
		HiveOriginalFunctions::CreateMoveHook = new CHook(**(PDWORD***)CHiveInterface.ClientMode);
		HiveOriginalFunctions::ClientModeCreateMove = (hive_func_ClientModeCreateMove)HiveOriginalFunctions::CreateMoveHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::CreateMove,
			VMT_CREATEMOVE);
	}

	void GetPaintTraverse() {
		HiveOriginalFunctions::PaintTraverseHook = new CHook((PDWORD*)CHiveInterface.IPanel);
		HiveOriginalFunctions::PaintTraverse = (hive_func_PaintTraverse)HiveOriginalFunctions::PaintTraverseHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::PaintTraverse,
			VMT_PAINTTRAVERSE);
	}

	void GetDrawModelExecute() {
		HiveOriginalFunctions::DrawModelExecuteHook = new CHook((PDWORD*)CHiveInterface.ModelRender);
		HiveOriginalFunctions::DrawModelExecute = (hive_func_DrawModelExecute)HiveOriginalFunctions::DrawModelExecuteHook->dwHookMethod(
			(DWORD)HiveHookedFunctions::DrawModelExecute,
			VMT_DRAWMODELEXECUTE);
	}

	void GetCalcView() {
		HiveOriginalFunctions::CalcView = (hive_func_CalcView)DetourFunction((PBYTE)CHiveSourceNative.offset_CalcView, (PBYTE)HiveHookedFunctions::CalcView);
	}

	void GetCreateMove() {
		HiveOriginalFunctions::CreateMove = (hive_func_CreateMove)DetourFunction((PBYTE)CHiveSourceNative.offset_CreateMove, (PBYTE)HiveHookedFunctions::CreateMove);
	}

	void GetFireBullets() {
		HiveOriginalFunctions::FireBullets = (hive_func_FireBullets)DetourFunction((PBYTE)CHiveSourceNative.offset_FireBullets, (PBYTE)HiveHookedFunctions::FireBullets);
	}

	void GetFrameStageNotify() {
		HiveOriginalFunctions::FrameStageNotify = (hive_func_FrameStageNotify)DetourFunction((PBYTE)CHiveSourceNative.offset_FrameStageNotify, (PBYTE)HiveHookedFunctions::FrameStageNotify);
	}

	void GetRenderCapture() {
		HiveOriginalFunctions::RenderCapture = (hive_func_RenderCapture)DetourFunction((PBYTE)CHiveSourceNative.offset_RenderCapture, (PBYTE)HiveHookedFunctions::RenderCapture);
	}

	void GetSwepPrimaryAttack() {
		HiveOriginalFunctions::SwepPrimaryAttack = (hive_func_SwepPrimaryAttack)DetourFunction((PBYTE)CHiveSourceNative.offset_SwepPrimaryAttack, (PBYTE)HiveHookedFunctions::SwepPrimaryAttack);
	}

	void InitHive() {
		GetCreateLuaInterface(); // index
		GetCloseLuaInterface();  // index
		GetPaintTraverse();		 // index
		GetDrawModelExecute();	 // index
		GetCreateMove();		 // sig
		GetCalcView();			 // sig
		GetFireBullets();		 // sig
		GetFrameStageNotify();	 // sig
		GetRenderCapture();		 // sig
		GetSwepPrimaryAttack();	 // sig
	}
}