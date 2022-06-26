#pragma once
#include "hive_dependencies.h"
typedef HANDLE(WINAPI *oCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef HANDLE(WINAPI *oCreateFileA)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef HANDLE(WINAPI *oFindFirstFileA)(LPCSTR, LPWIN32_FIND_DATAA);
extern std::string GLOBAL_ENCRYPTION_KEY;
enum {
ENCRYPTION_KEY,
DECRYPTION_KEY
};

struct RC4KeyChain {
	RC4_KEY FKey, SKey;
	bool isGenerated = false;
	bool isFKeyAvailable = false, isSKeyAvailable = false;
};
int OpenSSL_GetGlobalEncryptionKey();
bool dirExists(const std::string& dirName_in);
void ProtectScripts();
void RC4GenerateKeyPair(RC4KeyChain &KeyChain, std::string GKey);
void SafeHiveScriptExecution(std::string path, int method);
void RC4EncryptData(unsigned char *data, int size, unsigned char *buffer, RC4KeyChain &KeyChain);
void RC4DecryptData(unsigned char* encrypted_data, int size, unsigned char *buffer, RC4KeyChain &KeyChain);
void RC4EncryptFile(std::string path,bool);
void RC4DecryptFile(std::string path,bool);

namespace HiveProtection {
	extern oFindFirstFileA orgFindFirstFileA;
	extern oCreateFileW orgCreateFileW;
	extern oCreateFileA orgCreateFileA;
	extern WCHAR HiveMultichar[];
	extern WCHAR HiveLuaMultichar[];
	extern WCHAR DownloadHiveMultichar[];
	extern WCHAR PDB[];
	void SetupProtection();
}