#include "hive_interface.h"
HiveInterface CHiveInterface;

HiveInterface::HiveInterface() {
	LuaShared = (CLuaShared*)CHiveFactory.LuaFactory(V_CLIENT_LUA_INTERFACE_VERSION, 0);
	Client = (IBaseClientDLL*)CHiveFactory.ClientFactory(V_CLIENT_INTERFACE_VERSION, 0);
	Cvar = (ICvar*)CHiveFactory.VSTD(V_ENGINE_CVAR_INTERFACE_VERSION, 0);
	Engine = (IVEngineClient*)CHiveFactory.EngineFactory(V_ENGLINE_CLIENT_INTERFACE_VERSION, 0);
	ModelRender = (IVModelRender*)CHiveFactory.EngineFactory(V_ENGINE_MODEL_RENDER_VERSION, 0);
	ModelInfo = (IVModelInfo*)CHiveFactory.EngineFactory(V_MODEL_INFO_CLIENT, 0);
	EngineTool = (CEngineTool03*)CHiveFactory.EngineFactory(V_ENGINE_TOOL, 0);
	EngineTrace = (IEngineTrace*)CHiveFactory.EngineFactory(V_ENGINETRACE_INTERFACE_VERSION, 0);
	EntityList = (IClientEntityList*)CHiveFactory.ClientFactory(V_CLIENTENTITYLIST_INTERFACE_VERSION, 0);
	Render = (IVRenderView*)CHiveFactory.EngineFactory(V_ENGINE_RENDERVIEW_INTERFACE_VERSION, 0);
	IPanel = (vgui::IPanel*)CHiveFactory.VGUI2Factory(V_VGUI_PANEL_INTERFACE_VERSION, 0);
	ISurface = (vgui::ISurface*)CHiveFactory.VGUIFactory(VGUI_SURFACE_INTERFACE_VERSION, 0);
	MaterialSystem = (IMaterialSystem*)CHiveFactory.MaterialFactory(V_MATERIAL_SYSTEM_INTERFACE_VERSION, 0);
	Prediction = (CPredictionNew*)CHiveFactory.ClientFactory(V_CLIENT_PREDICTION_INTERFACE_VERSION, 0);
	GameMovement = (IGameMovement*)CHiveFactory.ClientFactory(V_GAMEMOVEMENT_INTERFACE_VERSION, 0);
	GameConsole = (IGameConsole *)CHiveFactory.GameUIFactory(V_GAMECONSOLE_INTERFACE_VERSION, 0);
	PlayerInfoManager = (CPlayerInfoManager*)CHiveFactory.ServerFactory(V_PLAYER_INFO_MANAGER, 0);
	Globals = PlayerInfoManager->GetGlobalVars();
//	CNetworkedVariableManager netvarManager;
	if(ISurface)
	MaterialSurface = (IMatSystemSurface*)ISurface->QueryInterface(MAT_SYSTEM_SURFACE_INTERFACE_VERSION);

	//Prone to crashes
	
	ClientMode = (IClientMode*)((HiveScanner::SigScan((DWORD)GetModuleHandle("client.dll"), 
				 HiveScanner::GetModuleSize("client.dll"), 
				 (PBYTE)CHiveSignatureStorage.ClientModeSig[0], 
				 CHiveSignatureStorage.ClientModeSig[1])) + 2);
	
	DWORD* dwClientVMT = (DWORD*)*(DWORD*)Client;
	Input = (CInputNew*)*(PDWORD)*(PDWORD)(dwClientVMT[15] + 0x2);
	//4E3AA0
	MoveHelper = **reinterpret_cast<IMoveHelper***>(HiveScanner::SigScan((DWORD)GetModuleHandle("client.dll"), HiveScanner::GetModuleSize("client.dll"), (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x52\xF3\x0F\x10\x80\x00\x00\x00\x00", "xx????xxxxx????") + 0x2);
	ClientState = *reinterpret_cast<CClientState**>((HiveScanner::SigScan((DWORD)GetModuleHandle("engine.dll"), HiveScanner::GetModuleSize("engine.dll"), (PBYTE)"\xB9\x00\x00\x00\x00\xDD\x5C\x24\x18", "x????xxxx") + 0x1)); //((DWORD)GetModuleHandle("engine.dll") + 0x4E3AA0);
	ClientState += 104;
}