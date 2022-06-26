#pragma once
#include "hive_dependencies.h"

namespace HiveNetVarOffsets {
	extern DWORD m_iHealth; //DT_BasePlayer
	extern DWORD m_lifeState; //DT_BasePlayer
	extern DWORD m_flMaxSpeed; //DT_BasePlayer
	extern DWORD m_fFlags; //DT_BasePlayer
	extern DWORD m_vecMins; //DT_CollisionProperty
	extern DWORD m_vecMaxs;//DT_CollisionProperty
	extern DWORD m_flSimulationTime; //DT_BaseEntity
	extern DWORD m_iTeamNum; //DT_BaseEntity
	extern DWORD m_Collision; //DT_BaseEntity
	extern DWORD m_flFriction; //DT_LocalPlayerExclusive
	extern DWORD m_vecVelocity; //DT_LocalPlayerExclusive
	extern DWORD m_nTickBase; //DT_LocalPlayerExclusive
	extern DWORD m_hGroundEntity; //DT_LocalPlayerExclusive
	extern DWORD m_vecPunchAngle; //DT_Local
	extern DWORD m_hActiveWeapon; //DT_BaseCombatCharacter
}

