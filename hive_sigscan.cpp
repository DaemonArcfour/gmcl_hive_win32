#include "hive_sigscan.h"

namespace HiveScanner {
	DWORD GetModuleSize(const char* mod) {
		MODULEINFO modinfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(mod), &modinfo, sizeof(MODULEINFO));
		return modinfo.SizeOfImage;
	}

	inline bool QuickCheck(PBYTE pData, PBYTE pbMask, char *szMask)
	{
		for (; *szMask; ++szMask, ++pbMask, ++pData)
			if (*szMask == 'x' && *pData != *pbMask)
				return false;
		return true;
	}
	DWORD SigScan(DWORD dwAddress, DWORD dwSize, PBYTE pbMask, char *szMask)
	{
		for (int i = 0; i < dwSize; i++)
			if (QuickCheck((PBYTE)(dwAddress + i), pbMask, szMask))
				return (dwAddress + i);
		return -1;
	}

	DWORD SigScanRelative(DWORD dwAddress, DWORD dwSize, PBYTE pbMask, char* szMask)
	{
		DWORD dwCallAddress = SigScan(dwAddress, dwSize, pbMask, szMask);
		DWORD dwRelativeAddress = *(DWORD*)(dwCallAddress + 0x1);
		DWORD dwRealFunctionAddress = dwCallAddress + 0x1 + sizeof(DWORD) + dwRelativeAddress;
		return dwRealFunctionAddress;
	}
}