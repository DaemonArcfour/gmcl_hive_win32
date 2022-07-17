#pragma once
#include "hive_dependencies.h"
// Global drawing panel
#define VGUI_PANEL "staticClientDLLToolsPanel" //"HudGMOD"
//#define VGUI_PANEL "FocusOverlayPanel"
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
//-=-=-=-=
//Model Render
#define VMT_DRAWMODELEXECUTE 20
//-=-=-=-=
//Prediction
#define VMT_RUNCOMMAND 17
// niggers?
#define VMT_CVAR_GET_BOOL 13

namespace HiveHook {
	void CleanUpLuaHooks();
	void GetRunString(void*);
	void GetRunStringEx(void*);
	void GetCompileString(void*);
	void GetFrameStageNotify();
	void GetRenderCapture();
	void GetCreateLuaInterface();
	void GetCloseLuaInterface();
	void GetSetupBones();
	void GetLuaGameModeCallWithArgs();
	void GetLuaGameModeCallFinish();
	void GetRunCommand();
	void GetWriteUsercmdDeltaToBuffer();
	void InitHive();
	void GetCheatsConVar();
}

extern GarrysMod::Lua::ILuaBase *MENU;