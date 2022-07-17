#pragma once
#include "hive_dependencies.h"
#include "hive_native.h"
namespace HiveUTIL {
	IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert);
	bool IsFriend(unsigned long long);
	bool IsFriend(C_BasePlayerNew* Player);
	bool IsKeyDown(int key, bool default_state);
	bool IsKeyPressed(int key, bool default_state);
}