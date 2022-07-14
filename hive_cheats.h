#pragma once
#include "hive_dependencies.h"
#include "hive_native.h"

class CBaseEntityNew;

class CBacktrackBase
{
public:
	Vector m_vAbsOrigin;
	Vector m_vOrigin;
	Vector m_vOOBMins;
	Vector m_vOOBMaxs;
	QAngle m_vAbsAngles;
	matrix3x4_t m_mtBones[128];
};

class CBacktrackFrame : public CBacktrackBase
{
public:
	int m_iTickCount;
	float m_fSimulationTime;
	std::map<const char*, Vector> m_mBoneMap;
	CBacktrackFrame();
};

class CReconcileFrame : public CBacktrackBase
{
public:
	bool bReconciled;
	CReconcileFrame() { bReconciled = false; }
};

class CBacktrackEntity
{
public:
	std::deque<CBacktrackFrame> m_vBacktrackFrames;
	CReconcileFrame* m_cReconciledFrame;
	C_BasePlayerNew* m_pPlayerEntity;
	void SaveFrame(int iSequenceNumber);
	int GetFrameCount();
	CBacktrackFrame GetFrame(int iTick);
};

class CBacktrack
{
public:
	std::map<int, CBacktrackEntity> m_mEntities;
	bool bInit = false;
	bool bDisableBoneInterpolation = false;
	float m_flInterp;
	float m_flRatio;
	bool m_bInterpolate;
	bool m_bHasToChange;
	bool m_bHasToReset;
	int m_iResetTicks;
	int m_iSequenceNumber;

	void ProcessTick();
	void Reconcile(CBacktrackEntity& BTEntity, int iTick);
	float GetLowestPossibleLerpTime(int* nUpdateRate);
	bool IsDeltaTooBig(Vector& vPos1, Vector& vPos2);
	int EstimateServerArriveTick(GMODCUserCmd* cmd);
	bool CanRestoreToSimulationTime(float flSimulationTime, bool* bNeedToAdjustInterp);
	void EndReconciliation(CBacktrackEntity& BTEntity);
	float GetLerpTime();
	void SetSequenceNumber(int iSequenceNumber);
	void Interp_WriteUserCmdDeltaToBuffer();
	void Interp_UpdateInterpolation(float flInterp);
}; 

namespace HiveCheats 
{
	void Bunnyhop(GMODCUserCmd* ucmd, CBaseEntityNew* LocalPlayer);
	bool CheckFire(C_BaseCombatWeaponNew* currentWeapon);
	void DrawEsp();
	void DrawObservers();
	bool TestCollision(Vector ColPos);
	void Triggerbot(QAngle Angle, GMODCUserCmd* pCmd, CBaseEntityNew* LocalPlayer);
	void Aimbot(GMODCUserCmd *pCmd, CBaseEntityNew* LocalPlayer);
	void RemoveSpread(GMODCUserCmd *pCmd, C_BaseCombatWeaponNew* gun, Vector Spread);
	void RemoveRecoil(QAngle&, CBaseEntityNew* LocalPlayer);
	void CorrectMovement(QAngle vOldAngles, GMODCUserCmd* pCmd, float fOldForward, float fOldSidemove);
	void Chams(IVModelRender*,DrawModelState_t const&, ModelRenderInfo_t const&, matrix3x4_t *);
	void Autostrafe(GMODCUserCmd* cmd);
	void Antiaim(GMODCUserCmd* cmd, CBaseEntityNew* LocalPlayer);
	void FakeLag(GMODCUserCmd* cmd);
	extern CBacktrack cBacktrackInterface;
	extern int autostrafe_angle;
	extern int aimbot_target;
	extern int choked;
	extern bool BreakLuaGameModeHooks;
	extern bool* bSendPacket;
}