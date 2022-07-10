#pragma once
#include "hive_dependencies.h"
#include "hive_native.h"

class CBaseEntityNew;

class CBacktrackFrame
{
public:
	int m_iTickCount;
	float m_fSimulationTime;
	Vector m_vAbsOrigin;
	Vector m_vOOBMins;
	Vector m_vOOBMaxs;
	QAngle m_vAbsAngles;
	matrix3x4_t m_mtBones[128];
	std::map<const char*, Vector> m_mBoneMap;
	CBacktrackFrame();
};

class CReconcileFrame : public CBacktrackFrame 
{
public:
	bool bReconciled;
	CReconcileFrame() { bReconciled = false; }
};
class CBacktrackEntity
{
public:
	std::deque<CBacktrackFrame> m_vBacktrackFrames;
	CReconcileFrame m_cReconciledFrame;
	C_BasePlayerNew* m_pPlayerEntity;
	void SaveFrame(GMODCUserCmd* cmd);
	int GetFrameCount();
	CBacktrackFrame GetFrame(int iTick);
};

class CBacktrack
{
public:
	std::map<int, CBacktrackEntity> m_mEntities;
	bool bInit = false;
	void ProcessTick(GMODCUserCmd* cmd);
	void Reconcile(GMODCUserCmd* cmd, CBacktrackEntity BTEntity, int iTick);
	void EndReconciliation(CBacktrackEntity BTEntity);
}; 

namespace HiveCheats 
{
	void Bunnyhop(GMODCUserCmd* ucmd, CBaseEntityNew* LocalPlayer);
	bool CheckFire(C_BaseCombatWeaponNew* currentWeapon);
	void DrawEsp();
	void DrawObservers();
	void Triggerbot(QAngle Angle, GMODCUserCmd* pCmd, CBaseEntityNew* LocalPlayer);
	void Aimbot(GMODCUserCmd *pCmd, CBaseEntityNew* LocalPlayer);
	void RemoveSpread(GMODCUserCmd *pCmd, C_BaseCombatWeaponNew* gun, Vector Spread);
	void RemoveRecoil(QAngle&, CBaseEntityNew* LocalPlayer);
	void CorrectMovement(QAngle vOldAngles, GMODCUserCmd* pCmd, float fOldForward, float fOldSidemove);
	void Chams(IVModelRender*,DrawModelState_t const&, ModelRenderInfo_t const&, matrix3x4_t *);
	void Autostrafe(GMODCUserCmd* cmd);
	void Antiaim(GMODCUserCmd* cmd);
	void FakeLag(GMODCUserCmd* cmd);
	extern CBacktrack cBacktrackInterface;
	extern int autostrafe_angle;
	extern int aimbot_target;
	extern int choked;
	extern bool BreakLuaGameModeHooks;
	extern bool* bSendPacket;
}