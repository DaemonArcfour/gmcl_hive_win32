#pragma once
#include "hive_dependencies.h"

namespace HiveHookedFunctions {
	void * __fastcall RunString(void *ecx, void *edx, const char* ccName, const char* ccPath, const char* ccString, bool bRun, bool bErrors);
	void * __fastcall RunStringEx(void *_this, void* edx, char const* ccName, char const* ccPath, char const* ccString, bool bRun, bool bErrors, bool UNK_1, bool UNK_2);
	void * __fastcall CompileString(void *_this, void* edx, void* UNK_1, char const& ccString);
	void * __fastcall CloseLuaInterface(void *_this, void* edx, void *luaInterface);
	void * __fastcall CreateLuaInterface(void *_this, void *, uchar stateType, bool renew);
	bool   __fastcall CreateMove(void* CClientModeShared, void* edx, float sample_input_frametime, GMODCUserCmd* cmd);
}
extern void* clientState;

namespace HiveOriginalFunctions {
	extern CHook* RunStringHook;
	extern CHook* RunStringExHook;
	extern CHook* CompileStringHook;
	extern CHook* CreateLuaInterfaceHook;
	extern CHook* CloseLuaInterfaceHook;
	extern CHook* CreateMoveHook;
	extern hive_func_CompileString CompileString;
	extern hive_func_RunString RunString;
	extern hive_func_RunStringEx RunStringEx;
	extern hive_func_CreateLuaInterface CreateLuaInterface;
	extern hive_func_CloseLuaInterface CloseLuaInterface;
	extern hive_func_CreateMove CreateMove;
}