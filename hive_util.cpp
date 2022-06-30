#include "hive_util.h"

IMaterial* HiveUTIL::CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert)
{
	std::stringstream materialData;
	materialData << "\"" + type + "\"\n"
		"{\n"
		"\t\"$basetexture\" \"" + texture + "\"\n"
		"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
		"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
		"\t\"$model\" \"" + std::to_string(model) + "\"\n"
		"\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
		"\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
		"}\n" << std::flush;

	std::string materialName = "hive_" + std::to_string(RandomInt(10, 100000));
	KeyValues* keyValues = new KeyValues(materialName.c_str());
	CHiveSourceNative.KeyValues_Init(keyValues, type.c_str());
	CHiveSourceNative.KeyValues_LoadFromBuffer(keyValues, materialName.c_str(), materialData.str().c_str(), nullptr, nullptr);
	IMaterial* CreatedMaterial = CHiveInterface.MaterialSystem->CreateMaterial(materialName.c_str(), keyValues);
	CreatedMaterial->IncrementReferenceCount();
	return CreatedMaterial;
}

bool HiveUTIL::IsFriend(const char* steamid) {
	return CLuaMenuCallback.FriendList.find(steamid) != CLuaMenuCallback.FriendList.end();
}