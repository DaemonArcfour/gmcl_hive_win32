#pragma once
#include "hive_dependencies.h"
namespace HiveScanner {
	DWORD GetModuleSize(const char* mod);
	DWORD SigScan(DWORD dwAddress, DWORD dwSize, PBYTE pbMask, char *szMask);
}