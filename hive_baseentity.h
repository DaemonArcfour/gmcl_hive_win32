#pragma once
#include "hive_setupnetvars.h"
#if PROXY_ENTITY == true
#define RAW_ENT(ce) ce->InternalEntity()
#else
#define RAW_ENT(ce) ce->m_pEntity
#endif
class CBaseEntityNew
{
public:
	float Friction();
	Vector& Velocity() const noexcept;
	QAngle GetPunchAngles();
	QAngle GetEyeAngles();
	int GetPrimaryAmmoType();
	int GetMoveType();
	QAngle GetPunchAnglesVel();
	int GetGroundEntityHandle();
	void SetPunchAngle(QAngle);
	void SetPunchAngleVel(QAngle);
	void* ActiveWeap();
	void* GetObserverTarget();
	int Health();
	int& GetTickBase() const noexcept;
	float flSimulationTime();
	int TeamNum();
	int GroundEntity();
	float MaxSpeed();
	int& GetFlags() const noexcept;
	Vector OBBMins();
	Vector OBBMaxs();
	bool isAlive();
	bool IsDormant();

	int Index()
	{
		void* networkable = (void*)(this + 0x8);
		typedef int(__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(networkable, 9))(networkable);
	}

	Vector GetEyePosition()
	{
		Vector vecViewOffset = *reinterpret_cast<Vector*>((DWORD)this + 0xE8);
		return GetAbsOrigin() + vecViewOffset;
	}

	Vector& GetAbsOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(this, 9))(this);
	}

	Vector& GetAbsAngles()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(this, 10))(this);
	}

	void* GetClientClass()
	{
		void* Networkable = (void*)(this + 0x8);
		typedef void* (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(Networkable, 2))(Networkable);
	}

	int GetIndex()
	{

		typedef int(__thiscall* OriginalFn)(PVOID);
		return ((OriginalFn)VMT.GetFunction(this, 70))(this);
	}

	bool SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, VMatrix*, int, int, float);
		return ((OriginalFn)VMT.GetFunction(pRenderable, 16))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	bool SetupBonesMatrix3_4(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4_t*, int, int, float);
		return ((OriginalFn)VMT.GetFunction(pRenderable, 16))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	const model_t* GetModel()
	{

		return (model_t*)*((DWORD *)this + 19);
	}

	void prtThis() {
		ConMsg("[0x%.8X]\n", (DWORD)this);
	}

};