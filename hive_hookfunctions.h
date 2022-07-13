#pragma once
#include "hive_dependencies.h"

namespace HiveHookedFunctions 
{
	void*	__fastcall RunString(void *, void *, const char*, const char*, const char*, bool, bool);
	void*	__fastcall RunStringEx(void *, void*, char const*, char const*, char const*, bool, bool, bool, bool);
	void*	__fastcall CompileString(void *, void*, void*, char const&);
	void*	__fastcall CloseLuaInterface(void *, void* , void *);
	void*	__fastcall CreateLuaInterface(void *, void *, uchar, bool);
	void*	__fastcall CalcView(void*, void*, Vector&, QAngle&, float&, float&, float&);
	void*	__fastcall RenderCapture(void*, void*);
	void*	__fastcall SwepPrimaryAttack(void*, void*);
	bool	__fastcall ClientModeCreateMove(void*, void*, float, GMODCUserCmd*);
	void	__fastcall CreateMove(void*, void *, int , float, bool);
	void	__fastcall PaintTraverse(void*, void*, VPANEL, bool, bool);
	void	__fastcall DrawModelExecute(void*, void*, DrawModelState_t const&, ModelRenderInfo_t const&, matrix3x4_t *);
	void	__fastcall FireBullets(void*, void*, void*);
	void	__fastcall FrameStageNotify(void *, void*, ClientFrameStage_t);
	bool	__fastcall SetupBones(void*, void*, matrix3x4_t*, int, int, float);
	bool	__fastcall LuaGameModeCallWithArgs(void*, void*, int);
	void	__fastcall RunCommand(void*, void*, C_BasePlayerNew*, GMODCUserCmd*, void*);
	bool	__fastcall WriteUsercmdDeltaToBuffer(void* _this, void* ecx, bf_write* buf, int from, int to, int isnewcommand);
}

extern void* clientState;
extern VPANEL GlobalPanel;
extern int MaxClients;

namespace HiveOriginalFunctions {
	extern CHook* RunStringHook;
	extern CHook* RunStringExHook;
	extern CHook* CompileStringHook;
	extern CHook* CreateLuaInterfaceHook;
	extern CHook* CloseLuaInterfaceHook;
	extern CHook* CreateMoveHook;
	extern CHook* PaintTraverseHook;
	extern CHook* DrawModelExecuteHook;
	extern CHook* CInputCreateMoveHook;
	extern CHook* FireBulletsHooks;
	extern CHook* FrameStageNotifyHook;
	extern CHook* SetupBonesHook;
	extern CHook* RunCommandHook;
	extern hive_func_WriteUsercmdDeltaToBuffer WriteUsercmdDeltaToBuffer;
	extern hive_func_RunCommand RunCommand;
	extern hive_func_CompileString CompileString;
	extern hive_func_RunString RunString;
	extern hive_func_RunStringEx RunStringEx;
	extern hive_func_CreateLuaInterface CreateLuaInterface;
	extern hive_func_CloseLuaInterface CloseLuaInterface;
	extern hive_func_ClientModeCreateMove ClientModeCreateMove;
	extern hive_func_PaintTraverse PaintTraverse;
	extern hive_func_DrawModelExecute DrawModelExecute;
	extern hive_func_FrameStageNotify FrameStageNotify;
	extern hive_func_RenderCapture RenderCapture;
	extern hive_func_SwepPrimaryAttack SwepPrimaryAttack;
	extern hive_func_CalcView CalcView;
	extern hive_func_CreateMove CreateMove;
	extern hive_func_FireBullets FireBullets;
	extern hive_func_SetupBones SetupBones;
	extern hive_func_CLuaGameModeCallWithArgs LuaGameModeCallWithArgs;
	extern hive_func_CLuaGameModeCallFinish LuaGameModeCallFinish;
}