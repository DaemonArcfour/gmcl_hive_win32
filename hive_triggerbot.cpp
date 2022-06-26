#include "hive_cheats.h"
namespace HiveCheats {
	void Triggerbot(QAngle qCurAng, GMODCUserCmd* pCmd, CBaseEntityNew* LocalPlayer)
	{

		trace_t tr;
		Ray_t ray;
		QAngle qFix;
		Vector vEyePos = LocalPlayer->GetEyePosition();
		Vector vEnd;
		CTraceFilter filter;
		filter.pSkip = LocalPlayer;
		AngleVectors(qCurAng, &vEnd);
		vEnd = vEnd * 8192 + vEyePos;
		ray.Init(vEyePos, vEnd);
		CHiveInterface.EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);
		CBaseEntityNew *pBaseEntity = (CBaseEntityNew*)tr.m_pEnt;

		if (!pBaseEntity || !pBaseEntity->isAlive())
			return;

		if (tr.allsolid)
			return;

		pCmd->buttons |= IN_ATTACK;
	}
}