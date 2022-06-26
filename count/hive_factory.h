#pragma once
#include "hive_dependencies.h"
#define CreateInterface CreateInterfaceFn
class HiveFactory {
public:
	HiveFactory::HiveFactory();
	CreateInterface LuaFactory;
	CreateInterface ClientFactory;
	CreateInterface EngineFactory;
	CreateInterface PhysicFactory;
	CreateInterface FileSystemFactory;
	CreateInterface StudioFactory;
	CreateInterface ServerFactory;
	CreateInterface MaterialFactory;
	CreateInterface VGUIFactory;
	CreateInterface VGUI2Factory;
	CreateInterface GameUIFactory;
	CreateInterface VSTD;
}; extern HiveFactory CHiveFactory;