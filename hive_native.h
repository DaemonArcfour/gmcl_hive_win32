#pragma once
#include "hive_dependencies.h"
typedef C_BaseCombatWeapon*(*native_GetCombatWeapon)(void);
typedef C_BasePlayer*(*native_UTIL_PlayerByIndex)(int);
typedef C_BaseCombatWeapon*(*native_GetPlayerCombatWeapon)(void*);
typedef CRC32_t(__thiscall* native_GetChecksum)(void *_this);
typedef unsigned int(*native_MD5_Random)(unsigned int);
typedef int(__thiscall* native_LookUpBone)(void* ppp_entAnim, const char *attachment);
typedef void(__thiscall* native_GetBonePosition)(void *ppp_entAnim, int  boneNum, Vector &origin/*, QAngle& angles*/);
typedef char*(__thiscall* native_GetClassName)(void *_this);
typedef CBaseEntityNew*(*native_GetObserverTarget)(C_BasePlayer *_this);
typedef void*(__thiscall* native_initkeyvalues)(void *_this, const char*);
typedef void*(__thiscall* native_loadfrombuffer)(void *_this, const char*, const char*, void*, void*);

class SourceNative {
private:
	DWORD CL = (DWORD)GetModuleHandle("client.dll");
	DWORD CLSize = HiveScanner::GetModuleSize("client.dll");
	DWORD Engine = (DWORD)GetModuleHandle("engine.dll");
	DWORD EngineSize = HiveScanner::GetModuleSize("engine.dll");
	DWORD MaterialSystem = (DWORD)GetModuleHandle("materialsystem.dll");
	DWORD MaterialSystemSize = HiveScanner::GetModuleSize("materialsystem.dll");
	DWORD offset_GetActiveWeapon = HiveScanner::SigScan(	CL,		CLSize,		(PBYTE)"\x56\x8B\xF1\x3B\x35", "xxxxx");
	DWORD offset_MD5_Random = HiveScanner::SigScan(			CL,		CLSize,		(PBYTE)"\x55\x8B\xEC\x83\xEC\x68\x6A", "xxxxxxx");
	DWORD offset_UTIL_PlayerByIndex = HiveScanner::SigScan(	CL,		CLSize,		(PBYTE)"\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x56\xFF\x75\x08\xE8", "xxxxx????xxxxx");
	DWORD offset_LookUpBone = HiveScanner::SigScan(			CL,		CLSize,		(PBYTE)"\x55\x8B\xEC\x56\x8B\xF1\x80\xBE\x00\x00\x00\x00\x00\x74\x13\xFF\x75\x08", "xxxxxxxx?????xxxxx");
	DWORD offset_GetBonePosition = HiveScanner::SigScan(	CL,		CLSize,		(PBYTE)"\x55\x8B\xEC\x83\xEC\x30\x56\x6A", "xxxxxxxx");
	DWORD offset_GetObserverTarget = HiveScanner::SigScan(	CL,		CLSize,		(PBYTE)"\x56\x8B\xF1\x39\x35\x00\x00\x00\x00\x75\x21", "xxxxx????xx");
	DWORD offset_GetChecksum = HiveScanner::SigScan(		CL,		CLSize,		(PBYTE)"\x55\x8B\xEC\x51\x56\x8D\x45\xFC\x8B\xF1\x50\xE8", "xxxxxxxxxxxx");
	DWORD offset_GetClassName = HiveScanner::SigScan(		CL,		CLSize,		(PBYTE)"\x53\x8B\xD9\x8B\x83\x00\x00\x00\x00\x85\xC0\x74", "xxxxx????xxx");
	DWORD offset_LoadFromBuffer = HiveScanner::SigScan(		CL,		CLSize,		(PBYTE)"\x55\x8B\xEC\x83\xEC\x34\x53\x8B\x5D\x0C", "xxxxxxxxxx");
	DWORD offset_InitKeyvalues = HiveScanner::SigScan(		MaterialSystem, MaterialSystemSize, (PBYTE)"\x55\x8B\xEC\x56\x8B\xF1\x6A\x01", "xxxxxxxx");
public:
	native_GetPlayerCombatWeapon	GetPlayerActiveWeapon = (native_GetPlayerCombatWeapon)(offset_GetActiveWeapon);								//near STR: "Player.AmbientUnderWater" IN VTABLE!
	native_MD5_Random				MD5_PseudoRandom =		(native_MD5_Random)(offset_MD5_Random);												// CInput::CreateMove  0x7FFFFFFF
	native_UTIL_PlayerByIndex		UTIL_GetPlayerByIndex = (native_UTIL_PlayerByIndex)(offset_UTIL_PlayerByIndex); // (Updated 04.02.2019)		// STR: "cvarname" or ""Unhandled GameEvent in ClientModeShared::FireGameEvent - %s\n"
	native_LookUpBone				LookUpBone =			(native_LookUpBone)(offset_LookUpBone);												//found near client.dll "placementOrigin"
	native_GetBonePosition			GetBonePosition =		(native_GetBonePosition)(offset_GetBonePosition);									//found near client.dll "placementOrigin"
	native_GetObserverTarget		GetObserverTarget =		(native_GetObserverTarget)(offset_GetObserverTarget);								// Found in a vtable near "Player.AmbientUnderWater"
	native_GetChecksum				GetChecksum =			(native_GetChecksum)(offset_GetChecksum);											// End of CInput::CreateMove
	native_GetClassName				GetClassName =			(native_GetClassName)(offset_GetClassName);											// POTENTIALLY could be found in FireBullets, but the placement differs from macOS dylibs
	native_loadfrombuffer			KeyValues_LoadFromBuffer = (native_loadfrombuffer)(offset_LoadFromBuffer);									// "tmp_resource" client.dll
	native_initkeyvalues			KeyValues_Init =		(native_initkeyvalues)(offset_InitKeyvalues);										// "BufferClearObeyStencil" materialsystem.dll

	DWORD offset_bSendPacket = HiveScanner::SigScan(Engine, EngineSize, (PBYTE)"\xC6\x45\xFF\x01\x8B\x01\x8B", "xxxxxxx") + 3;				// (Updated 04.02.2019) (CL_MOVE)
	DWORD offset_CalcView = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x55\x8B\xEC\x53\x56\x8B\xF1\x57\x8B\x8E", "xxxxxxxxxx");
	DWORD offset_CreateMove = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x55\x8B\xEC\x83\xEC\x58\x53\x8B\x5D\x08\xB8", "xxxxxxxxxxx");		// (Updated 16.11.2019)
	DWORD offset_FrameStageNotify = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x55\x8B\xEC\x51\x8B\x45\x08\xA3", "xxxxxxxx");
	DWORD offset_FireBullets = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x56\x8B\x73\x08\x57\x89\x4D\xF0", "xxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxx"); // C_BaseEntity::FireBullets; "ERROR: Undefined ammo type!\n"
	DWORD offset_RenderCapture = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x55\x8B\xEC\x83\xEC\x60\x8B\x0D\x00\x00\x00\x00\x8B\x81\x00\x00\x00\x00", "xxxxxxxx????xx????");
	DWORD offset_SwepPrimaryAttack = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x56\x8B\xF1\xE8\x00\x00\x00\x00\x85\xC0\x74\x48", "xxxx????xxxx");
	DWORD offset_SetupBones = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x55\x8B\xEC\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x53\x56\x57\x8B\xF9\x8B\x0D\x00\x00\x00\x00", "xxxx????x????xxxxxxx????");
	DWORD offset_LuaGameModeCallWithArgs = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\x8B\x46\x3C\x8D\x4E\x3C\xFF\x90\x84\x00\x00\x00\x8B\x00\x00\x00\x00\x00\xFF", "xxxxxxxxxxxxx?????x") - 0x7C; // "CLuaGamemode::CallWithArgs" client.dll
	DWORD offset_LuaGameModeCallFinish = HiveScanner::SigScan(CL, CLSize, (PBYTE)"\xE8\x00\x00\x00\x00\x83\x7D\xCC\x00", "x????xxxx"); //"CLuaGamemode::CallFinish" client.dll -- This sig is wrong, have to calculate negative offset
}; extern SourceNative CHiveSourceNative;
class cPaintTraverseConfig {
public:
	void *ecx = 0;
	void *edx = 0;
	VPANEL panel = 0;
	bool frp = 0;
	bool allwfr = 0;
}; extern cPaintTraverseConfig PaintTraverseConfig;
namespace NativeClass {
	extern char* BonesTable[20];
	extern bool *bSendPacket;
	extern char* PriorityPoints[21];
	char* GetLuaWeaponName(C_BasePlayer*);
	QAngle GetEntEyeAng(CBaseEntityNew*);
	bool IsDormant(CBaseEntityNew *Target);
	bool GetBonePosition(void* ply, const char* attachment, Vector& origin);
	bool GetBonePosition(void* _this, const char* attachment, Vector& origin, matrix3x4_t* bones);
}

class WeaponName;
class GetIsDormant;
class EntEyeAng;

/* Native GMod Classes*/
class EntEyeAng
{
public:
	char _0x0000[11260];
	QAngle EyeAngles; //0x2BF8 

};//Size=0x2C04

class WeaponName
{
public:
	char _0x0000[16];
	char* Name; //0x0010 

};//Size=0x0018

class GetIsDormant
{
public:
	char pad_0x0000[0x7C]; //0x0000
	unsigned char N00000140; //0x007C 
	unsigned char N00000333; //0x007D 
	unsigned char N00000336; //0x007E 
	BYTE isDormant; //0x007F 
};//Size=0x0440
//IsDormant fix


class FireBullets_info
{
public:
	int m_iShots = 0; //0x0000 
	Vector m_vecSrc = Vector(0, 0, 0); //0x0004 
	Vector m_vecDirShooting = Vector(0, 0, 0); //0x0010 
	Vector m_vecSpread = Vector(0, 0, 0); //0x001C 
	float m_flDistance = 0.0f; //0x0028 
	int m_iAmmoType = 0; //0x002C 
	int m_iTracerFreq = 0; //0x0030 
	float m_flDamage = 0; //0x0034 
	int m_nFlags = 0; //0x0038 
	int m_flDamageForceScale = 0; //0x003C 
	CBaseEntityNew* m_pAttacker = 0; //0x0040 
	CBaseEntityNew* m_pAdditionalIgnoreEnt = 0; //0x0044 
	__int32 unk1 = 0; //0x0048 
	__int32 unk2 = 0; //0x004C 
	__int32 unk3 = 0; //0x0050 
	char pad_0x0054[0x6C]; //0x0054

}; //Size=0x00C0


class CVerifiedUserCmd
{
public:
	GMODCUserCmd	m_cmd;
	CRC32_t		m_crc;
};