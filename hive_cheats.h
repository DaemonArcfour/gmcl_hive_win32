#pragma once
#include "hive_dependencies.h"
#include "hive_native.h"

class CBaseEntityNew;
namespace HiveCheats {
	void Bunnyhop(GMODCUserCmd* ucmd, CBaseEntityNew* LocalPlayer);
	bool CheckFire();
	void DrawEsp();
	void DrawObservers();
	void Triggerbot(QAngle Angle, GMODCUserCmd* pCmd, CBaseEntityNew* LocalPlayer);
	void Aimbot(GMODCUserCmd *pCmd, CBaseEntityNew* LocalPlayer);
	void RemoveSpread(GMODCUserCmd *pCmd, C_BaseCombatWeaponNew* gun, Vector Spread);
	void RemoveRecoil(QAngle&, CBaseEntityNew* LocalPlayer);
	void CorrectMovement(QAngle vOldAngles, GMODCUserCmd* pCmd, float fOldForward, float fOldSidemove);
	void Chams(IVModelRender*,DrawModelState_t const&, ModelRenderInfo_t const&, matrix3x4_t *);
	void Autostrafe(GMODCUserCmd* cmd);
	extern int autostrafe_angle;
	extern bool BreakLuaGameModeHooks;
}