#include "hive_baseentity.h"
#include "hive_native.h"
float CBaseEntityNew::Friction()
{
	return *reinterpret_cast<float*>((DWORD)this + HiveNetVarOffsets::m_flFriction);
}

Vector& CBaseEntityNew::Velocity() const noexcept
{
	return *reinterpret_cast<Vector*>((DWORD)this + HiveNetVarOffsets::m_vecVelocity);
}

QAngle CBaseEntityNew::GetPunchAngles() {
	return *reinterpret_cast<QAngle*>((DWORD)this + HiveNetVarOffsets::m_vecPunchAngle);
}

QAngle CBaseEntityNew::GetPunchAnglesVel() {
	return *reinterpret_cast<QAngle*>((DWORD)this + HiveNetVarOffsets::m_vecPunchAngleVel);
}

void CBaseEntityNew::SetPunchAngle(QAngle y) {
	QAngle* x = reinterpret_cast<QAngle*>((DWORD)this + HiveNetVarOffsets::m_vecPunchAngle);
	*x = y;
}

void CBaseEntityNew::SetPunchAngleVel(QAngle y) {
	QAngle* x = reinterpret_cast<QAngle*>((DWORD)this + HiveNetVarOffsets::m_vecPunchAngleVel);
	*x = y;
}

QAngle CBaseEntityNew::GetEyeAngles() {
	return *reinterpret_cast<QAngle*>((DWORD)this + HiveNetVarOffsets::m_angEyeAngles);
}

int CBaseEntityNew::GetGroundEntityHandle() {
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_hGroundEntity);
}

int CBaseEntityNew::GetPrimaryAmmoType() {
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_iPrimaryAmmoType);
}

int CBaseEntityNew::GetMoveType() {
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_MoveType);
}

void* CBaseEntityNew::ActiveWeap() {
	return reinterpret_cast<void*>((DWORD)this + HiveNetVarOffsets::m_hActiveWeapon);
}
void* CBaseEntityNew::GetObserverTarget() {
	return reinterpret_cast<void*>((DWORD)this + HiveNetVarOffsets::m_hObserverTarget);
}
int CBaseEntityNew::Health()
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_iHealth);
}
int& CBaseEntityNew::GetTickBase() const noexcept
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_nTickBase);
}
int& CBaseEntityNew::GetEffects() const noexcept
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_fEffects);
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

bool CBaseEntityNew::IsDormant(){
	return *reinterpret_cast<bool*>((DWORD)this + HiveNetVarOffsets::m_bDormant);
}

int& CBaseEntityNew::GetFlags() const noexcept
{
	return *reinterpret_cast<int*>((DWORD)this + HiveNetVarOffsets::m_fFlags);
}

Vector CBaseEntityNew::OBBMins()
{
	return *reinterpret_cast<Vector*>((DWORD)this /*+ HiveNetVarOffsets::m_Collision*/  + HiveNetVarOffsets::m_vecMins);
}
Vector CBaseEntityNew::OBBMaxs()
{
	return *reinterpret_cast<Vector*>((DWORD)this /*+ HiveNetVarOffsets::m_Collision*/ + HiveNetVarOffsets::m_vecMaxs);
}
bool CBaseEntityNew::isAlive()
{
	BYTE lifestate = *(BYTE*)((DWORD)this + HiveNetVarOffsets::m_lifeState);
	return (lifestate == 0);
}