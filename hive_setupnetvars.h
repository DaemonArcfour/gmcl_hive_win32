#pragma once
#include "hive_dependencies.h"
#define NET_VAR(entity, offset, type) (*(reinterpret_cast<type *>(reinterpret_cast<uint64_t>(entity) + (offset))))
#define NET_INT(entity, offset) NET_VAR(entity, offset, int)
#define NET_FLOAT(entity, offset) NET_VAR(entity, offset, float)
#define NET_BYTE(entity, offset) NET_VAR(entity, offset, unsigned char)
#define NET_VECTOR(entity, offset) NET_VAR(entity, offset, Vector)

namespace HiveNetVarOffsets {
	void GetOffsets();
	extern DWORD m_MoveType;
	extern DWORD m_iHealth; //DT_BasePlayer
	extern DWORD m_lifeState; //DT_BasePlayer
	extern DWORD m_flMaxSpeed; //DT_BasePlayer
	extern DWORD m_fFlags; //DT_BasePlayer
	extern DWORD m_fEffects; //DT_BaseEntity
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
	extern DWORD m_vecPunchAngleVel;
	extern DWORD m_angEyeAngles;
	extern DWORD m_Local;
	extern DWORD m_hActiveWeapon; //DT_BaseCombatCharacter
	extern DWORD m_bDormant; // DT_BaseEntity
	extern DWORD m_iPrimaryAmmoType;
	extern DWORD m_flNextPrimaryAttack;
	extern DWORD m_iObserverMode;
	extern DWORD m_hObserverTarget;
}

