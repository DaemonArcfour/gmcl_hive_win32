#include "hive_setupnetvars.h"
#include "hive_netvar.h"

namespace HiveNetVarOffsets {
	DWORD m_lifeState = 0;
	DWORD m_flMaxSpeed = 0;
	DWORD m_fFlags = 0;
	DWORD m_fEffects = 0;
	DWORD m_vecPunchAngle = 0;
	DWORD m_vecPunchAngleVel = 0;
	DWORD m_vecVelocity = 0;
	DWORD m_flFriction = 0;
	DWORD m_hGroundEntity = 0;
	DWORD m_nTickBase = 0;
	DWORD m_flSimulationTime = 0;
	DWORD m_iTeamNum = 0;
	DWORD m_Collision = 0;
	DWORD m_vecMins = 0;
	DWORD m_vecMaxs = 0;
	DWORD m_iHealth = 0;
	DWORD m_iArmor = 0;
	DWORD m_bDormant = 0;
	DWORD m_angEyeAngles = 0;
	DWORD m_Local = 0;
	DWORD m_vecViewOffset = 0;
	DWORD m_hActiveWeapon = 0;
	DWORD m_iObserverMode = 0;
	DWORD m_hObserverTarget = 0;
	DWORD m_iPrimaryAmmoType = 0;
	DWORD m_MoveType = 0;
	DWORD m_flNextPrimaryAttack = 0;

	void GetOffsets() {
		g_pNetvars.init();
		m_MoveType = g_pNetvars.get_offset("DT_BasePlayer", "m_MoveType");
		m_lifeState = g_pNetvars.get_offset("DT_BasePlayer", "m_lifeState");
		m_flMaxSpeed = g_pNetvars.get_offset("DT_BasePlayer", "m_flMaxspeed");
		m_fFlags = g_pNetvars.get_offset("DT_BasePlayer", "m_fFlags");
		m_vecPunchAngle = g_pNetvars.get_offset("DT_BasePlayer", "localdata", "m_Local", "m_vecPunchAngle");
		m_vecPunchAngleVel = g_pNetvars.get_offset("DT_BasePlayer", "localdata", "m_Local", "m_vecPunchAngleVel");
		m_iObserverMode = g_pNetvars.get_offset("DT_BasePlayer", "m_iObserverMode");
		m_hObserverTarget = g_pNetvars.get_offset("DT_BasePlayer", "m_hObserverTarget");
		m_vecVelocity = g_pNetvars.get_offset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		m_flFriction = g_pNetvars.get_offset("DT_BasePlayer", "localdata", "m_flFriction");
		m_hGroundEntity = g_pNetvars.get_offset("DT_BasePlayer", "localdata", "m_hGroundEntity");
		m_nTickBase = g_pNetvars.get_offset("DT_BasePlayer", "localdata", "m_nTickBase");
		m_flSimulationTime = g_pNetvars.get_offset("DT_BaseEntity", "m_flSimulationTime");
		m_iTeamNum = g_pNetvars.get_offset("DT_BaseEntity", "m_iTeamNum");
		m_Collision = g_pNetvars.get_offset("DT_BaseEntity", "m_Collision");
		m_vecMins = g_pNetvars.get_offset("DT_BaseEntity", "m_Collision", "m_vecMins");
		m_vecMaxs = g_pNetvars.get_offset("DT_BaseEntity", "m_Collision", "m_vecMaxs");
		m_iHealth = g_pNetvars.get_offset("DT_BaseEntity", "m_iHealth");
		m_iArmor = g_pNetvars.get_offset("DT_PlayerResource", "m_iArmor");
		m_fEffects = g_pNetvars.get_offset("DT_BaseEntity", "m_fEffects");
		m_angEyeAngles = g_pNetvars.get_offset("DT_HL2MP_Player", "hl2mplocaldata", "m_angEyeAngles[0]");
		m_hActiveWeapon = g_pNetvars.get_offset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		//LocalWeaponData 
		m_iPrimaryAmmoType = g_pNetvars.get_offset("DT_BaseCombatWeapon", "LocalWeaponData", "m_iPrimaryAmmoType");
		m_flNextPrimaryAttack = g_pNetvars.get_offset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
	}
} 
