#pragma once
#include "hive_dependencies.h"
typedef HANDLE(WINAPI *oCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef HANDLE(WINAPI *oCreateFileA)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

namespace HiveProtection {
	extern oCreateFileW orgCreateFileW;
	extern oCreateFileA orgCreateFileA;
	extern WCHAR HiveMultichar[];
	extern WCHAR HiveLuaMultichar[];
	extern WCHAR DownloadHiveMultichar[];
	void SetupProtection();
}