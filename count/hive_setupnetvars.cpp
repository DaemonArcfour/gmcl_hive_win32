#include "hive_setupnetvars.h"

namespace HiveNetVarOffsets {
	DWORD m_iHealth = netvarManager.GetOffset("DT_BasePlayer", "m_iHealth");
	DWORD m_lifeState = netvarManager.GetOffset("DT_BasePlayer", "m_lifeState");
	DWORD m_flMaxSpeed = netvarManager.GetOffset("DT_BasePlayer", "m_flMaxSpeed");
	DWORD m_fFlags = netvarManager.GetOffset("DT_BasePlayer", "m_fFlags");
	DWORD m_vecMins = netvarManager.GetOffset("DT_BasePlayer", "m_vecMins");
	DWORD m_vecMaxs = netvarManager.GetOffset("DT_BasePlayer", "m_vecMaxs");
	DWORD m_vecPunchAngle = netvarManager.GetOffset("DT_BasePlayer", "m_vecPunchAngle");
	DWORD m_vecVelocity = netvarManager.GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
	DWORD m_flFriction = netvarManager.GetOffset("DT_BasePlayer", "m_flFriction");
	
	DWORD m_flSimulationTime = netvarManager.GetOffset("DT_BaseEntity", "m_flSimulationTime");
	DWORD m_iTeamNum = netvarManager.GetOffset("DT_BaseEntity", "m_iTeamNum");
	DWORD m_Collision = netvarManager.GetOffset("DT_BaseEntity", "m_Collision");
	DWORD m_hGroundEntity = netvarManager.GetOffset("DT_BasePlayer", "m_hGroundEntity");
	DWORD m_nTickBase = netvarManager.GetOffset("DT_BasePlayer", "m_nTickBase");
	
	DWORD m_hActiveWeapon = netvarManager.GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
} 
