#include "hive_signature.h"

HiveSignatureStorage CHiveSignatureStorage;

namespace HiveSiggedFunctions {
	char* GetServerIP() {
		DWORD ServerIP = HiveScanner::SigScan((DWORD)GetModuleHandle("engine.dll"), HiveScanner::GetModuleSize("engine.dll"), (PBYTE)CHiveSignatureStorage.ServerIP[0], CHiveSignatureStorage.ServerIP[1]) + 1;
		char* ret = *(char**)ServerIP;
		return ret;
	}
}