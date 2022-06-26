#include "hive_protection.h"

HANDLE WINAPI Hooked_CreateFileW(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
) {
	if (wcsstr(lpFileName, HiveProtection::HiveMultichar) || wcsstr(lpFileName, HiveProtection::HiveLuaMultichar))
		return HiveProtection::orgCreateFileW(L"", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	
	if(CLuaMenuCallback.LockDownloads && wcsstr(lpFileName, HiveProtection::DownloadHiveMultichar))
		return HiveProtection::orgCreateFileW(L"", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	return HiveProtection::orgCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WINAPI Hooked_CreateFileA(
	_In_ LPCSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
) {
	if (strstr(lpFileName, "hive_win32.dll") || strstr(lpFileName, "h4ck.lua"))
		return HiveProtection::orgCreateFileA("", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	if (CLuaMenuCallback.LockDownloads && strstr(lpFileName, "downloaded_hive"))
		return HiveProtection::orgCreateFileA("", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

		return HiveProtection::orgCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}



namespace HiveProtection {
	oCreateFileW orgCreateFileW;
	oCreateFileA orgCreateFileA;
	WCHAR HiveMultichar[] = L"hive_win32.dll";
	WCHAR HiveLuaMultichar[] = L"h4ck.lua";
	WCHAR DownloadHiveMultichar[] = L"downloaded_hive";
	void SetupProtection() {
		auto aCreateFileW = reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileW"));
		auto aCreateFileA = reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA"));
		orgCreateFileA = (oCreateFileA)DetourFunction((PBYTE)aCreateFileA, (PBYTE)Hooked_CreateFileA);
		orgCreateFileW = (oCreateFileW)DetourFunction((PBYTE)aCreateFileW, (PBYTE)Hooked_CreateFileW);
	}
}