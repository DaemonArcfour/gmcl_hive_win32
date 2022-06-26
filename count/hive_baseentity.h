#pragma once
#include "hive_setupnetvars.h"
class CBaseEntityNew
{
public:
	float Friction();
	Vector Velocity();
	QAngle GetPunchAngles();
	void* ActiveWeap();
	int Health();
	int GetTickBase();
	float flSimulationTime();
	int TeamNum();
	int GroundEntity();
	float MaxSpeed();
	int GetFlags();
	Vector OBBMins();
	Vector OBBMaxs();
	bool isAlive();
	//EntIndex
	int Index()
	{
		void* networkable = (void*)(this + 0x8);
		typedef int(__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(networkable, 9))(networkable);
		//return *(int*)(this + 64);
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

	bool IsDormant()
	{
		typedef bool(__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(this, 76))(this);
	}

	//ValidateModelIndex
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
		//typedef bool(__thiscall* OriginalFn)(PVOID, VMatrix*, int, int, float);
		//return ((OriginalFn)VMT.GetFunction(this, 94))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	bool SetupBonesMatrix3_4(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4_t*, int, int, float);
		return ((OriginalFn)VMT.GetFunction(pRenderable, 16))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
		//typedef bool(__thiscall* OriginalFn)(PVOID, VMatrix*, int, int, float);
		//return ((OriginalFn)VMT.GetFunction(this, 94))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	const model_t* GetModel()
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef const model_t*(__thiscall* OriginalFn)(PVOID);
		return VMT.getvfunc<OriginalFn>(pRenderable, 9)(pRenderable);
	}
	void prtThis() {
		ConMsg("[0x%.8X]\n", (DWORD)this);
	}

};