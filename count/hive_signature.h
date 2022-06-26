#pragma once
#include "hive_dependencies.h"

namespace HiveSiggedFunctions {
	char* GetServerIP();
}

class HiveSignatureStorage {
public:
	char* ClientModeSig[2] = { "\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x50\x08\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x50\x20", "xx????xxxxxxx????xxxxx" };
	char* CloseLuaInterfaceSig[2] = { "\x55\x8B\xEC\x56\x57\x8B\x7D\x08\x8B\xF1\x57", "xxxxxxxxxxx" };
	char* CreateLuaInterfaceSig[2] = { "\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\xF1", "xxxxxxxxx" };
	char* ServerIP[2] = { "\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8A\xD8", "x????x????xx" };
}; HiveSignatureStorage extern CHiveSignatureStorage;
