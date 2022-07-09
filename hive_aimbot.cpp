#include "hive_cheats.h"
#include "hive_math.h"
#include "hive_util.h"
float	flBestDist;
int		m_nTarget;

void DropTarget()
{
	flBestDist = 1e+7;
	m_nTarget = -1;
}

float GetFov(QAngle angle, Vector src, Vector dst)
{
	QAngle ang, aim;
	float fov;

	HiveMath::CalculateAngle(src, dst, ang);
	HiveMath::MakeVector(angle, aim);
	HiveMath::MakeVector(ang, ang);

	float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));

	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

	fov = acos(u_dot_v / (mag_s*mag_d)) * (360.0 / M_PI);

	return fov;
}

void EntBestTarget(CBaseEntityNew* me, CBaseEntityNew* pBaseEntity, int index) {
	QAngle AbsAng = QAngle(me->GetAbsAngles().x, me->GetAbsAngles().y, me->GetAbsAngles().z);
	if (GetFov(AbsAng, me->GetEyePosition(), pBaseEntity->GetEyePosition()) <= 360
		&& GetFov(AbsAng, me->GetEyePosition(), pBaseEntity->GetEyePosition()) < flBestDist)
	{

		flBestDist = GetFov(AbsAng, me->GetEyePosition(), pBaseEntity->GetEyePosition());
		m_nTarget = index;
	}
}


bool GetVisible(Vector& vecAbsStart, Vector& vecAbsEnd, CBaseEntityNew* pBaseEnt, CBaseEntityNew* LocalPlayer)
{
	CTraceFilter filter;
	trace_t tr;
	Ray_t ray;
	filter.pSkip = LocalPlayer;
	ray.Init(vecAbsStart, vecAbsEnd);
	//0x46004003
	CHiveInterface.EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

	if (tr.fraction > 0.97f) {
		return true;
	}

	if (tr.m_pEnt && pBaseEnt)
	{
		if (tr.m_pEnt->index == 0 || tr.allsolid) {
			return false;
		}

		player_info_t* pinfo = new player_info_t;
		if ((CHiveInterface.Engine->GetPlayerInfo(tr.m_pEnt->index, pinfo)
			|| pBaseEnt->Index() == tr.m_pEnt->index) && tr.fraction > 0.92) {
			delete pinfo;
			return true;
		}
		delete pinfo;
	}
	return false;
}
Vector PredictedPosition;
Vector EnemyPosition;
Vector BonePosition;
QAngle dummy;
QAngle AimbotAngle;
CBaseEntityNew* pBaseEntity;
CBaseEntityNew* Target;
int VisibleBoneIndex = -1;
int SavedBoneIndex = -1;

namespace HiveCheats {
	int aimbot_target = -1;
	void Aimbot(GMODCUserCmd *pCmd, CBaseEntityNew* LocalPlayer) {
		DropTarget();
		aimbot_target = -1;
		for (int index = CHiveInterface.Engine->GetMaxClients(); index >= 0; --index)
		{

			if (index == CHiveInterface.Engine->GetLocalPlayer())
				continue;

			pBaseEntity = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(index);


			if (!pBaseEntity  || !pBaseEntity->isAlive() || NativeClass::IsDormant(pBaseEntity) )
				continue;

			if(HiveUTIL::IsFriend((C_BasePlayerNew*)pBaseEntity))
				continue;

			for (int i = 0; i < 21; i++) {
				NativeClass::GetBonePosition(CHiveInterface.EntityList->GetClientEntity(index), NativeClass::PriorityPoints[i], BonePosition);
				
				if (!GetVisible(LocalPlayer->GetEyePosition(), BonePosition, pBaseEntity, LocalPlayer))
					continue;
				
				else {
					VisibleBoneIndex = i;
					break;
				}
				
			}

			if (VisibleBoneIndex == -1)
				continue;

			else {
				
				SavedBoneIndex = VisibleBoneIndex;
				Target = pBaseEntity;
				EntBestTarget(LocalPlayer, pBaseEntity, index);
				VisibleBoneIndex = -1;
				
			}
		}

		if (m_nTarget == -1 || SavedBoneIndex == -1)
			return;


		aimbot_target = m_nTarget;
		NativeClass::GetBonePosition(CHiveInterface.EntityList->GetClientEntity(m_nTarget), NativeClass::PriorityPoints[SavedBoneIndex], EnemyPosition);
		if (!CLuaMenuCallback.EnginePredict)
		{
			PredictedPosition = EnemyPosition - (LocalPlayer->Velocity() * CHiveInterface.Globals->interval_per_tick);
			VectorAngles(PredictedPosition - LocalPlayer->GetEyePosition(), AimbotAngle);
		}

		else
			VectorAngles(EnemyPosition - LocalPlayer->GetEyePosition(), AimbotAngle);

		pCmd->mousedx = 1.0;
		pCmd->mousedy = 1.0;
		if(pCmd->buttons & IN_ATTACK)
			pCmd->viewangles = AimbotAngle;
		SavedBoneIndex = -1;

	}
}