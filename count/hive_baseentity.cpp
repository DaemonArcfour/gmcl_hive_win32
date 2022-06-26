#include "hive_baseentity.h"

float CBaseEntityNew::Friction()
{
	return *reinterpret_cast<float*>((DWORD)this + HiveNetVarOffsets::m_flFriction);
}

Vector CBaseEntityNew::Velocity()
{
	return *reinterpret_cast<Vector*>((DWORD)this + HiveNetVarOffsets::m_vecVelocity);
}

QAngle CBaseEntityNew::GetPunchAngles() {
	return *reinterpret_cast<QAngle*>((DWORD)this + HiveNetVarOffsets::m_vecPunchAngle);
}

void* CBaseEntityNew::ActiveWeap() {
	return reinterpret_cast<void*>((DWORD)this + HiveNetVarOffsets::m_hActiveWeapon);
}

int CBaseEntityNew::Health()
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_iHealth);
}
int CBaseEntityNew::GetTickBase()
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_nTickBase);
}
float CBaseEntityNew::flSimulationTime()
{
	return *reinterpret_cast<float*>((DWORD)this + HiveNetVarOffsets::m_flSimulationTime);
}
int CBaseEntityNew::TeamNum()
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_iTeamNum);
}
int CBaseEntityNew::GroundEntity()
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_hGroundEntity);
}
float CBaseEntityNew::MaxSpeed()
{
	return *reinterpret_cast<float*>((DWORD)this + HiveNetVarOffsets::m_flMaxSpeed);
}

int CBaseEntityNew::GetFlags()
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_fFlags);
}

Vector CBaseEntityNew::OBBMins()
{
	return *reinterpret_cast<Vector*>((DWORD)this + HiveNetVarOffsets::m_Collision + HiveNetVarOffsets::m_vecMins);
}
Vector CBaseEntityNew::OBBMaxs()
{
	return *reinterpret_cast<Vector*>((DWORD)this + HiveNetVarOffsets::m_Collision + HiveNetVarOffsets::m_vecMaxs);
}
bool CBaseEntityNew::isAlive()
{
	BYTE lifestate = *(BYTE*)((DWORD)this + HiveNetVarOffsets::m_lifeState);
	return (lifestate == 0);
}