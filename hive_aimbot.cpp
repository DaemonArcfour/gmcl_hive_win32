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
int SavedBacktrackTick = -1;
bool bSavedBacktrackState = false;

namespace HiveCheats {
	int aimbot_target = -1;

	bool IsAimKeyDown()
	{
		if (CLuaMenuCallback.AimbotKey == 0)
			return true;

		if (CHiveInterface.ISurface->IsCursorVisible())
			return false;

		auto aimKey = (vgui::KeyCode)CLuaMenuCallback.AimbotKey;

		if (aimKey >= MOUSE_FIRST && aimKey <= MOUSE_LAST)
			return CHiveInterface.VInput->IsMouseDown(aimKey);

		return CHiveInterface.VInput->IsKeyDown(aimKey);
	}

	void Aimbot(GMODCUserCmd *pCmd, CBaseEntityNew* LocalPlayer) {
		DropTarget();
		aimbot_target = -1;
		bool bBacktrackEntity = false;
		int iBacktrackTick = 0;

		for (int index = CHiveInterface.Engine->GetMaxClients(); index >= 0; --index)
		{

			if (index == CHiveInterface.Engine->GetLocalPlayer())
				continue;

			pBaseEntity = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(index);


			if (!pBaseEntity  || !pBaseEntity->isAlive() || NativeClass::IsDormant(pBaseEntity) )
				continue;

			if(HiveUTIL::IsFriend((C_BasePlayerNew*)pBaseEntity))
				continue;

			for (int i = 0; i < 21; i++) 
			{
				NativeClass::GetBonePosition(CHiveInterface.EntityList->GetClientEntity(index), NativeClass::PriorityPoints[i], BonePosition);
				
				if (!GetVisible(LocalPlayer->GetEyePosition(), BonePosition, pBaseEntity, LocalPlayer))
					continue;
				
				else 
				{
					VisibleBoneIndex = i;
					break;
				}
				
			}

			if (VisibleBoneIndex == -1 && CLuaMenuCallback.Backtrack)
			{
				// try backtracking
				if (HiveCheats::cBacktrackInterface.m_mEntities[index].m_pPlayerEntity)
				{
					CBacktrackEntity* BTPlayer = &HiveCheats::cBacktrackInterface.m_mEntities[index];
					for (int i = 0; i < BTPlayer->GetFrameCount(); i++)
					{
						std::map<const char*, Vector> BacktrackedBones = BTPlayer->GetFrame(i).m_mBoneMap;
						HiveCheats::cBacktrackInterface.Reconcile(*BTPlayer, i);

						for (int z = 0; z < 5; z++)
						{
							
							if (!GetVisible(LocalPlayer->GetEyePosition(), BacktrackedBones[NativeClass::PriorityPoints[z]], pBaseEntity, LocalPlayer))
								continue;

							else
							{
								VisibleBoneIndex = z;
								bBacktrackEntity = true;
								iBacktrackTick = i;
								break;
							}
						}

						if (bBacktrackEntity)
							break;
					}

					HiveCheats::cBacktrackInterface.EndReconciliation(HiveCheats::cBacktrackInterface.m_mEntities[index]);
				}
			}

			if (VisibleBoneIndex == -1)
				continue;

			else 
			{
				SavedBoneIndex = VisibleBoneIndex;
				SavedBacktrackTick = iBacktrackTick;
				bSavedBacktrackState = bBacktrackEntity;
				Target = pBaseEntity;
				EntBestTarget(LocalPlayer, pBaseEntity, index);
				VisibleBoneIndex = -1;
				bBacktrackEntity = false;
				iBacktrackTick = -1;
			}
		}

		if (m_nTarget == -1 || SavedBoneIndex == -1)
			return;

		aimbot_target = m_nTarget;
		if (!bSavedBacktrackState)
		{
			NativeClass::GetBonePosition(CHiveInterface.EntityList->GetClientEntity(m_nTarget), NativeClass::PriorityPoints[SavedBoneIndex], EnemyPosition);
			if (!CLuaMenuCallback.EnginePredict)
			{
				PredictedPosition = EnemyPosition - (LocalPlayer->Velocity() * CHiveInterface.Globals->interval_per_tick);
				VectorAngles(PredictedPosition - LocalPlayer->GetEyePosition(), AimbotAngle);
			}

			else
				VectorAngles(EnemyPosition - LocalPlayer->GetEyePosition(), AimbotAngle);
		}

		pCmd->mousedx = 10;
		pCmd->mousedy = 10;
		if (IsAimKeyDown() && pCmd->buttons & IN_ATTACK)
		{
			if (bSavedBacktrackState)
			{
				Vector BacktrackedPos = HiveCheats::cBacktrackInterface.m_mEntities[m_nTarget].GetFrame(SavedBacktrackTick).m_mBoneMap[NativeClass::PriorityPoints[SavedBoneIndex]];
				VectorAngles(BacktrackedPos - LocalPlayer->GetEyePosition(), AimbotAngle);
				float simtime = HiveCheats::cBacktrackInterface.m_mEntities[m_nTarget].GetFrame(SavedBacktrackTick).m_fSimulationTime;
				int EstimatedArriveTicks = HiveCheats::cBacktrackInterface.EstimateServerArriveTick(pCmd) - pCmd->tick_count;
				//EstimatedArriveTicks -= 1;
				EstimatedArriveTicks = 0;
				HiveCheats::cBacktrackInterface.Interp_UpdateInterpolation(TICKS_TO_TIME((pCmd->tick_count - EstimatedArriveTicks) - TIME_TO_TICKS(simtime)));
			}

			pCmd->viewangles = AimbotAngle;
		
		}

		SavedBoneIndex = -1;
		SavedBacktrackTick = -1;
		bSavedBacktrackState = false;

	}
}