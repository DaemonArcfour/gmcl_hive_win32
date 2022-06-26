#include "hive_factory.h"

HiveFactory CHiveFactory;

HiveFactory::HiveFactory() {
	LuaFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("lua_shared.dll"), "CreateInterface");
	ClientFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("client.dll"), "CreateInterface");
	EngineFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("engine.dll"), "CreateInterface");
	PhysicFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vphysics.dll"), "CreateInterface");
	//FileSystemFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("FileSystem_Steam.dll"), "CreateInterface"); - Not used
	StudioFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("StudioRender.dll"), "CreateInterface");
	ServerFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("server.dll"), "CreateInterface");
	MaterialFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("MaterialSystem.dll"), "CreateInterface");
	VGUIFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vguimatsurface.dll"), "CreateInterface");
	VGUI2Factory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vgui2.dll"), "CreateInterface");
	GameUIFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("gameui.dll"), "CreateInterface");
	VSTD = (CreateInterfaceFn)GetProcAddress((HMODULE)GetModuleHandleA("vstdlib.dll"), "CreateInterface");
}