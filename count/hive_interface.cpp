#include "hive_interface.h"
HiveInterface CHiveInterface;

HiveInterface::HiveInterface() {
	LuaShared = (CLuaShared*)CHiveFactory.LuaFactory(V_CLIENT_LUA_INTERFACE_VERSION, 0);
	Client = (IBaseClientDLL*)CHiveFactory.ClientFactory(V_CLIENT_INTERFACE_VERSION, 0);
	Cvar = (ICvar*)CHiveFactory.VSTD(V_ENGINE_CVAR_INTERFACE_VERSION, 0);
	Engine = (IVEngineClient*)CHiveFactory.EngineFactory(V_ENGLINE_CLIENT_INTERFACE_VERSION, 0);
	EngineTrace = (IEngineTrace*)CHiveFactory.EngineFactory(V_ENGINETRACE_INTERFACE_VERSION, 0);
	EntityList = (IClientEntityList*)CHiveFactory.ClientFactory(V_CLIENTENTITYLIST_INTERFACE_VERSION, 0);
	Render = (IVRenderView*)CHiveFactory.EngineFactory(V_ENGINE_RENDERVIEW_INTERFACE_VERSION, 0);
	IPanel = (vgui::IPanel*)CHiveFactory.VGUI2Factory(V_VGUI_PANEL_INTERFACE_VERSION, 0);
	ISurface = (vgui::ISurface*)CHiveFactory.VGUIFactory(VGUI_SURFACE_INTERFACE_VERSION, 0);
	MaterialSystem = (IMaterialSystem*)CHiveFactory.MaterialFactory(V_MATERIAL_SYSTEM_INTERFACE_VERSION, 0);
	GameConsole = (IGameConsole *)CHiveFactory.GameUIFactory(V_GAMECONSOLE_INTERFACE_VERSION, 0);
	PlayerInfoManager = (CPlayerInfoManager*)CHiveFactory.ServerFactory(V_PLAYER_INFO_MANAGER, 0);
	Globals = PlayerInfoManager->GetGlobalVars();
	CNetworkedVariableManager netvarManager;
	if(ISurface)
	MaterialSurface = (IMatSystemSurface*)ISurface->QueryInterface(MAT_SYSTEM_SURFACE_INTERFACE_VERSION);

	//Prone to crashes
	
	ClientMode = (IClientMode*)((HiveScanner::SigScan((DWORD)GetModuleHandle("client.dll"), 
				 HiveScanner::GetModuleSize("client.dll"), 
				 (PBYTE)CHiveSignatureStorage.ClientModeSig[0], 
				 CHiveSignatureStorage.ClientModeSig[1])) + 2);
	
	DWORD* dwClientVMT = (DWORD*)*(DWORD*)Client;
	Input = (CInput*)*(PDWORD)*(PDWORD)(dwClientVMT[15] + 0x2);
	
}