#include "hive_protection.h"


std::string GLOBAL_ENCRYPTION_KEY = "";

void logHive(std::string logmsg) {
	std::ofstream outfile;
	outfile.open("hive_log.txt", std::ios_base::app);
	outfile << logmsg << "\n";
	outfile.close();
}

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false; 

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true; 

	return false;  
}

bool isKeyChainValid(int type, RC4KeyChain &KeyChain) {
	if (!KeyChain.isGenerated) {
		HiveTroubleshooter::Print("[Encryption] RC4 keychain was not generated, aborting.", ERROR);
		return false;
	}
	switch (type) {
	case ENCRYPTION_KEY:
		if (!KeyChain.isFKeyAvailable) {
			HiveTroubleshooter::Print("[Encryption] RC4 FKey is not available, aborting.", ERROR);
			return false;
		}
		else {
			return true;
		}
		break;
	case DECRYPTION_KEY:
		if (!KeyChain.isSKeyAvailable) {
			HiveTroubleshooter::Print("[Encryption] RC4 SKey is not available, aborting.", ERROR);
			return false;
		}
		else {
			return true;
		}
		break;
	default:
		HiveTroubleshooter::Print("[Encryption] Unknown keychain element.", ERROR);
		break;
	}
}

void RC4GenerateKeyPair(RC4KeyChain &KeyChain, std::string GKey) {
	RC4_set_key(&KeyChain.FKey, GKey.length(), (unsigned char*)GKey.c_str());
	RC4_set_key(&KeyChain.SKey, GKey.length(), (unsigned char*)GKey.c_str());
	KeyChain.isFKeyAvailable = true;
	KeyChain.isSKeyAvailable = true;
	KeyChain.isGenerated = true;
}

void RC4EncryptData(unsigned char *data, int size, unsigned char *buffer, RC4KeyChain &KeyChain) {
	if (!isKeyChainValid(ENCRYPTION_KEY, KeyChain)) {
		HiveTroubleshooter::PrintOpenSSL("[RC4] encryption key needs to be regenerated, aborting encryption.");
		return;
	}
	RC4(&KeyChain.FKey, size, data, buffer);
	buffer[size] = '\0';
	KeyChain.isFKeyAvailable = false;
	HiveTroubleshooter::PrintOpenSSL("[RC4] Successfully encrypted " + std::to_string(size) + " bytes of data.");
}

void RC4DecryptData(unsigned char* encrypted_data, int size, unsigned char *buffer, RC4KeyChain &KeyChain) {
	if (!isKeyChainValid(DECRYPTION_KEY, KeyChain)) {
		HiveTroubleshooter::PrintOpenSSL("[RC4] decryption key needs to be regenerated, aborting decryption.");
		return;
	}
	RC4(&KeyChain.SKey, size, encrypted_data, buffer);
	buffer[size] = '\0';
	KeyChain.isSKeyAvailable = false;
	HiveTroubleshooter::PrintOpenSSL("[RC4] Successfully decrypted " + std::to_string(size) + " bytes of data.");
}
int OpenSSL_GetGlobalEncryptionKey() {
	unsigned long Size = 0;
	HANDLE hFile = CreateFile("C:/HiveOpenSSL", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;
	Size = GetFileSize(hFile, NULL);
	char* GlobalKey = new char[Size];
	ZeroMemory(GlobalKey, Size);
	ReadFile(hFile, GlobalKey, Size, 0, 0);
	GLOBAL_ENCRYPTION_KEY = GlobalKey;
	delete[] GlobalKey;
	return 1;
}

void RC4EncryptFile(std::string path, bool safe) {
	if (safe)
		path = CLuaMenuCallback.GmodWorkspace + path;

	RC4KeyChain KeyChain;
	unsigned long Size;
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		HiveTroubleshooter::Print("Couldn't find " + path + "! Encryption aborted.", ERROR);
		return;
	}
	Size = GetFileSize(hFile, 0);
	unsigned char* unencrypted_data = new unsigned char[Size+1];
	unsigned char* encrypted_data = new unsigned char[Size+1];
	ZeroMemory(unencrypted_data, Size);
	ZeroMemory(encrypted_data, Size);
	ReadFile(hFile, unencrypted_data, Size, 0, 0);
	RC4GenerateKeyPair(KeyChain, GLOBAL_ENCRYPTION_KEY);
	RC4EncryptData(unencrypted_data, Size, encrypted_data, KeyChain);
	CloseHandle(hFile);
	HANDLE hRewrite = CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hRewrite, encrypted_data, Size, 0, 0);
	CloseHandle(hRewrite);
	delete[] encrypted_data;
	delete[] unencrypted_data;
}

void RC4DecryptFile(std::string path, bool safe) {
	if(safe)
	path = CLuaMenuCallback.GmodWorkspace + path;
	RC4KeyChain KeyChain;
	unsigned long Size;
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		HiveTroubleshooter::Print("Couldn't find " + path + "! Decryption aborted.", ERROR);
		return;
	}
	Size = GetFileSize(hFile, 0);
	unsigned char* encrypted_data = new unsigned char[Size+1];
	unsigned char* decrypted_data = new unsigned char[Size+1];
	ZeroMemory(encrypted_data, Size);
	ZeroMemory(decrypted_data, Size);
	ReadFile(hFile, encrypted_data, Size, 0, 0);
	RC4GenerateKeyPair(KeyChain, GLOBAL_ENCRYPTION_KEY);
	RC4DecryptData(encrypted_data, Size, decrypted_data, KeyChain);
	CloseHandle(hFile);
	HANDLE hRewrite = CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hRewrite, decrypted_data, Size, 0, 0);
	CloseHandle(hRewrite);
	delete[] encrypted_data;
	delete[] decrypted_data;
}

void SafeHiveScriptExecution(std::string path, int method) {
	ILuaInterface* MenuLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_MENU);
	ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
	path = CLuaMenuCallback.GmodWorkspace + path;
	RC4KeyChain KeyChain;
	unsigned long Size;
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		HiveTroubleshooter::Print("Couldn't find " + path + "! Safe execution aborted.", ERROR);
		return;
	}
	Size = GetFileSize(hFile, 0);
	unsigned char* encrypted_data = new unsigned char[Size + 1];
	unsigned char* decrypted_data = new unsigned char[Size + 1];
	ZeroMemory(encrypted_data, Size);
	ZeroMemory(decrypted_data, Size);
	ReadFile(hFile, encrypted_data, Size, 0, 0);
	CloseHandle(hFile);
	RC4GenerateKeyPair(KeyChain, GLOBAL_ENCRYPTION_KEY);
	RC4DecryptData(encrypted_data, Size, decrypted_data, KeyChain);
	std::string LuaRun = std::string((char*)decrypted_data);
	if (method == 1)
		MenuLua->RunStringEx("HiveScript", "", LuaRun.c_str(), 1, 1, 1, 1);
	if (method == 0)
		ClientLua->RunStringEx("HiveScript", "", LuaRun.c_str(), 1, 1, 1, 1);
	delete[] encrypted_data;
	delete[] decrypted_data;
}

void ProtectScripts() {
	HiveTroubleshooter::PrintOpenSSL("Getting a list of unprotected scripts.");
	std::string path = CLuaMenuCallback.GmodWorkspace + "lua/unprotected_scripts/";
	std::string protectedScript = CLuaMenuCallback.GmodWorkspace + "lua/protected_scripts/";
	for (auto & p : std::filesystem::directory_iterator(path)) {
		std::string toProtect = p.path().string();
		std::string extRem = p.path().string();
		HiveTroubleshooter::PrintOpenSSL(toProtect.c_str());
		RC4EncryptFile(toProtect, false);
		char* CuttedExtension = PathFindFileName(extRem.c_str());
		PathRemoveExtension(CuttedExtension);
		std::string newFile = CuttedExtension; 
		CopyFile(toProtect.c_str(), std::string(protectedScript + newFile +".hivescript").c_str(), 0);
		DeleteFile(toProtect.c_str());
	}
}

HANDLE WINAPI Hooked_CreateFileW(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
) {
	std::wstring wstr_prt = std::wstring(lpFileName);
	std::string  prt = std::string(wstr_prt.begin(), wstr_prt.end());
	if (wcsstr(lpFileName, HiveProtection::HiveMultichar) || wcsstr(lpFileName, HiveProtection::HiveLuaMultichar) || wcsstr(lpFileName, HiveProtection::PDB)) {
		//PlaySoundW(L"HiveSounds\\kernelblock.wav", NULL, SND_ASYNC);
		prt = "Kernel32->CreateFile: Handle faked. (" + prt + ")";
		logHive(prt);
		HiveTroubleshooter::PrintBlock(prt.c_str());
		return HiveProtection::orgCreateFileW(L"", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	
	if (CLuaMenuCallback.LockDownloads && wcsstr(lpFileName, HiveProtection::DownloadHiveMultichar)) {
		//PlaySoundW(L"HiveSounds\\kernelblock.wav", NULL, SND_ASYNC);
		prt = "Kernel32->CreateFile: Handle faked. (" + prt + ")";
		logHive(prt);
		HiveTroubleshooter::PrintBlock(prt.c_str());
		return HiveProtection::orgCreateFileW(L"", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
		
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
	std::string access = "";
	if (strstr(lpFileName, "gmsv_hive_win32.dll")) {
		//PlaySoundW(L"HiveSounds\\kernelblock.wav", NULL, SND_ASYNC);
		access = "Kernel32->CreateFile: Handle faked. (" + std::string(lpFileName) + ")";
		logHive(access);
		HiveTroubleshooter::PrintBlock(access.c_str());
		return HiveProtection::orgCreateFileA("D:\\gethive.txt", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	if (strstr(lpFileName, "_win32.dll") || strstr(lpFileName, "h4ck.lua") || strstr(lpFileName, "_win32.pdb")) {
		//PlaySoundW(L"HiveSounds\\kernelblock.wav", NULL, SND_ASYNC);
		access = "Kernel32->CreateFile: Handle blocked. (" + std::string(lpFileName) + ")";
		logHive(access);
		HiveTroubleshooter::PrintBlock(access.c_str());
		return HiveProtection::orgCreateFileA("", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	if (CLuaMenuCallback.LockDownloads && strstr(lpFileName, "downloaded_hive")) {
		//PlaySoundW(L"HiveSounds\\kernelblock.wav", NULL, SND_ASYNC);
		access = "Kernel32->CreateFile: Handle blocked. (" + std::string(lpFileName) + ")";
		logHive(access);
		HiveTroubleshooter::PrintBlock(access.c_str());
		return HiveProtection::orgCreateFileA("", dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
		
		return HiveProtection::orgCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WINAPI Hooked_FindFirstFileA(_In_ LPCSTR lpFileName, _Out_ LPWIN32_FIND_DATAA lpFindFileData) {
	std::string access = "";
	if (strstr(lpFileName, "garrysmod\\lua\\bin") || strstr(lpFileName, "downloaded_hive")) {
		//PlaySoundW(L"HiveSounds\\kernelblock.wav", NULL, SND_ASYNC);
		access = "Kernel32 -> FindFirstFileA: Handle blocked. (" + std::string(lpFileName) + ")";
		logHive(access);
		HiveTroubleshooter::PrintBlock(access.c_str());
		return INVALID_HANDLE_VALUE;
	}

	return HiveProtection::orgFindFirstFileA(lpFileName, lpFindFileData);
}

namespace HiveProtection {
	//FindFirstFileA
	oCreateFileW orgCreateFileW;
	oCreateFileA orgCreateFileA;
	oFindFirstFileA orgFindFirstFileA;
	WCHAR HiveMultichar[] = L"_win32.dll";
	WCHAR HiveLuaMultichar[] = L"h4ck.lua";
	WCHAR DownloadHiveMultichar[] = L"downloaded_hive";
	WCHAR PDB[] = L"_win32.pdb";
	void SetupProtection() {
		auto aCreateFileW = reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileW"));
		auto aCreateFileA = reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA"));
		auto aFindFirstFileA = reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "FindFirstFileA"));
		orgCreateFileA = (oCreateFileA)DetourFunction((PBYTE)aCreateFileA, (PBYTE)Hooked_CreateFileA);
		orgCreateFileW = (oCreateFileW)DetourFunction((PBYTE)aCreateFileW, (PBYTE)Hooked_CreateFileW);
		orgFindFirstFileA = (oFindFirstFileA)DetourFunction((PBYTE)aFindFirstFileA, (PBYTE)Hooked_FindFirstFileA);
	}
}