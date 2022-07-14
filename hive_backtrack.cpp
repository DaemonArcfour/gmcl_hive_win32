#include "hive_cheats.h"
#include "hive_native.h"

CBacktrack CHiveBacktrackInterface;

CBacktrackFrame::CBacktrackFrame()
{
		m_iTickCount = -1;
		m_fSimulationTime = -1;
		m_vAbsOrigin = Vector(0, 0, 0);
}

void CBacktrackEntity::SaveFrame(int iSequenceNumber)
{
	if (!m_pPlayerEntity)
		return;

	matrix3x4_t mtBones[128];
	CBacktrackFrame BTCurrentFrame;
	if (NativeClass::GetBoneMap(m_pPlayerEntity, BTCurrentFrame.m_mBoneMap, BTCurrentFrame.m_mtBones, false))
	{
		BTCurrentFrame.m_fSimulationTime = ((CBaseEntityNew*)m_pPlayerEntity)->flSimulationTime();
		BTCurrentFrame.m_iTickCount = TIME_TO_TICKS(BTCurrentFrame.m_fSimulationTime);
		BTCurrentFrame.m_vAbsOrigin = m_pPlayerEntity->GetAbsOrigin();
		BTCurrentFrame.m_vAbsAngles = m_pPlayerEntity->GetAbsAngles();
		BTCurrentFrame.m_vOOBMins = ((CBaseEntityNew*)m_pPlayerEntity)->OBBMins();
		BTCurrentFrame.m_vOOBMaxs = ((CBaseEntityNew*)m_pPlayerEntity)->OBBMaxs();
		BTCurrentFrame.m_vOrigin = ((CBaseEntityNew*)m_pPlayerEntity)->GetOrigin();
		m_vBacktrackFrames.push_front(BTCurrentFrame);
		GMODCUserCmd* cmd = (GMODCUserCmd*)CHiveInterface.Input->GetUserCmd(iSequenceNumber);
		if (m_vBacktrackFrames.size() > 0)
		{
			while (TICKS_TO_TIME((cmd->tick_count + 1) - TIME_TO_TICKS(m_vBacktrackFrames.back().m_fSimulationTime)) > 0.5f)
			{
				m_vBacktrackFrames.pop_back();

				if (m_vBacktrackFrames.size() == 0)
					break;
			}
		}

		if (m_vBacktrackFrames.size() > CLuaMenuCallback.Backtrack_max_tick)
		{
			
			m_vBacktrackFrames.pop_back();
		}
	}
}

int CBacktrackEntity::GetFrameCount()
{
	return m_vBacktrackFrames.size();
}

CBacktrackFrame CBacktrackEntity::GetFrame(int iTick)
{
	if (iTick > CLuaMenuCallback.Backtrack_max_tick || iTick >= GetFrameCount())
		return CBacktrackFrame();

	return m_vBacktrackFrames.at(iTick);
}

void CBacktrack::Reconcile(CBacktrackEntity& BTEntity, int iTick)
{
	if (!BTEntity.m_cReconciledFrame->bReconciled && BTEntity.m_pPlayerEntity)
	{

		BTEntity.m_cReconciledFrame->bReconciled = true;
		//uintptr_t* BaseAnimating = (uintptr_t*)(*(int(__thiscall**)(void*))(*(DWORD*)BTEntity.m_pPlayerEntity + 156))(BTEntity.m_pPlayerEntity);
		CHiveSourceNative.InvalidateBoneCache(BTEntity.m_pPlayerEntity);
		int BoneCacheSize = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrixSize;
		matrix3x4_t* BoneMatrixCache = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrix;
		
		CBacktrackFrame* ReconciliationFrame = &BTEntity.GetFrame(iTick);
	
		BTEntity.m_cReconciledFrame->m_vAbsOrigin = BTEntity.m_pPlayerEntity->GetAbsOrigin();
		BTEntity.m_cReconciledFrame->m_vAbsAngles = BTEntity.m_pPlayerEntity->GetAbsAngles();
		BTEntity.m_cReconciledFrame->m_vOOBMins = ((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->OBBMins();
		BTEntity.m_cReconciledFrame->m_vOOBMaxs = ((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->OBBMaxs();
		BTEntity.m_cReconciledFrame->m_vOrigin = ((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->GetOrigin();
		
		//memcpy(BTEntity.m_cReconciledFrame->m_mtBones, BoneMatrixCache, BoneCacheSize * sizeof(matrix3x4_t));
		//BTEntity.m_pPlayerEntity->GetAbsOrigin() = ReconciliationFrame.m_vAbsOrigin;
		//BTEntity.m_pPlayerEntity->GetAbsAngles() = ReconciliationFrame.m_vAbsAngles;
		//((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->GetOrigin() = ReconciliationFrame.m_vOrigin;
		CHiveSourceNative.SetAbsOrigin(BTEntity.m_pPlayerEntity, ReconciliationFrame->m_vOrigin);
		
		//CHiveSourceNative.SetCollisionBounds(BTEntity.m_pPlayerEntity->GetCollideable(), ReconciliationFrame.m_vOOBMins, ReconciliationFrame.m_vOOBMaxs);
		//memcpy(BoneMatrixCache, ReconciliationFrame.m_mtBones, BoneCacheSize * sizeof(matrix3x4_t));
		//BTEntity.m_pPlayerEntity->UpdateClientSideAnimation();
		//BTEntity.m_pPlayerEntity->GetClientNetworkable()->PostDataUpdate(DATA_UPDATE_DATATABLE_CHANGED);
		//BTEntity.m_pPlayerEntity->GetClientRenderable()->SetupBones(NULL, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, ReconciliationFrame.m_fSimulationTime);
		
		
		//BTEntity.m_pPlayerEntity->GetClientRenderable()->SetupBones(ReconciliationFrame->m_mtBones, 128, BONE_USED_BY_HITBOX, ReconciliationFrame->m_fSimulationTime);
		//cmd->tick_count = ReconciliationFrame.m_iTickCount;
	}
}

void CBacktrack::EndReconciliation(CBacktrackEntity& BTEntity)
{
	if (BTEntity.m_cReconciledFrame->bReconciled && BTEntity.m_pPlayerEntity)
	{
		//return;
		BTEntity.m_cReconciledFrame->bReconciled = false;
		int BoneCacheSize = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrixSize;
		matrix3x4_t* BoneMatrixCache = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrix;
		
		//BTEntity.m_pPlayerEntity->GetAbsOrigin() = BTEntity.m_cReconciledFrame->m_vAbsOrigin;
		//BTEntity.m_pPlayerEntity->GetAbsAngles() = BTEntity.m_cReconciledFrame->m_vAbsAngles;
		CHiveSourceNative.SetAbsOrigin(BTEntity.m_pPlayerEntity, BTEntity.m_cReconciledFrame->m_vAbsOrigin);
		//CHiveSourceNative.SetCollisionBounds(BTEntity.m_pPlayerEntity->GetCollideable(), BTEntity.m_cReconciledFrame->m_vOOBMins, BTEntity.m_cReconciledFrame->m_vOOBMaxs);
		//((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->GetOrigin() = BTEntity.m_cReconciledFrame->m_vOrigin;
		
		//memcpy(BoneMatrixCache, BTEntity.m_cReconciledFrame->m_mtBones, BoneCacheSize * sizeof(matrix3x4_t));
		
		//BTEntity.m_pPlayerEntity->GetClientNetworkable()->PostDataUpdate(DATA_UPDATE_DATATABLE_CHANGED);
		
	}
}

float CBacktrack::GetLerpTime()
{
	static auto sv_minupdaterate = CHiveInterface.Cvar->FindVar("sv_minupdaterate");
	static auto sv_maxupdaterate = CHiveInterface.Cvar->FindVar("sv_maxupdaterate");
	static auto cl_updaterate = CHiveInterface.Cvar->FindVar("cl_updaterate");
	static auto cl_interp_ratio = CHiveInterface.Cvar->FindVar("cl_interp_ratio");
	static auto cl_interp = CHiveInterface.Cvar->FindVar("cl_interp");
	static auto sv_client_min_interp_ratio = CHiveInterface.Cvar->FindVar("sv_client_min_interp_ratio");
	static auto sv_client_max_interp_ratio = CHiveInterface.Cvar->FindVar("sv_client_max_interp_ratio");

	int nUpdateRate = clamp(cl_updaterate->GetInt(), (int)sv_minupdaterate->GetFloat(), (int)sv_maxupdaterate->GetFloat());
	float flLerpTime = 0.f;

	float flLerpRatio = cl_interp_ratio->GetFloat();
	if (flLerpRatio == 0)
	{
		flLerpRatio = 1.f;
	}

	float flLerpAmount = cl_interp->GetFloat();

	if (sv_client_min_interp_ratio && sv_client_max_interp_ratio && sv_client_min_interp_ratio->GetFloat() != -1.f)
	{
		flLerpRatio = clamp(flLerpRatio, sv_client_min_interp_ratio->GetFloat(), sv_client_max_interp_ratio->GetFloat());
	}
	else
	{
		if (flLerpRatio == 0.f)
		{
			flLerpRatio = 1.f;
		}
	}

	flLerpTime = max(flLerpAmount, flLerpRatio / nUpdateRate);

	return flLerpTime;
}

float CBacktrack::GetLowestPossibleLerpTime(int* nUpdateRate)
{
	static auto sv_maxupdaterate = CHiveInterface.Cvar->FindVar("sv_maxupdaterate");

	if (nUpdateRate)
	{
		*nUpdateRate = sv_maxupdaterate->GetInt();
	}

	return 1.f/*cl_interp_ratio*/ / sv_maxupdaterate->GetInt();
}

int CBacktrack::EstimateServerArriveTick(GMODCUserCmd* cmd)
{
	int nTick = cmd->tick_count + 1;

	auto pNetChan = CHiveInterface.Engine->GetNetChannelInfo();
	if (pNetChan)
	{
		float time = pNetChan->GetLatency(FLOW_INCOMING) + pNetChan->GetLatency(FLOW_OUTGOING);
		nTick += TIME_TO_TICKS(time);
	}

	return nTick;
}
/*
bool CBacktrack::CanRestoreToSimulationTime(float flSimulationTime, bool* bNeedToAdjustInterp)
{
	float correct = 0.f;

	auto pNetChan = CHiveInterface.Engine->GetNetChannelInfo();
	if (pNetChan)
	{
		correct += pNetChan->GetLatency(FLOW_OUTGOING);
	}

	float lerpTicks = GetLerpTime();

	// not needed, we either adjust cl_interp or disable cl_interpolate[smart], except on newer src games where we cant but i add that later

	correct += lerpTicks;

	if (correct >= 1.f)
	{
		return false;
	}
	else if (correct < 0.f)
	{
		correct = 0.f;
	}

	float flDiff = TICKS_TO_TIME(EstimateServerArriveTick() - TIME_TO_TICKS(flSimulationTime));
	if (flDiff >= 1.f)
	{
		return false;
	}

	int targettick = TIME_TO_TICKS(flSimulationTime);
	float deltaTime = correct - TICKS_TO_TIME(EstimateServerArriveTick() - targettick);

	if (fabsf(deltaTime) >= TICKS_TO_TIME(7))
	{
		if (flDiff >= 0.f)
		{
			if (bNeedToAdjustInterp != nullptr)
			{
				*bNeedToAdjustInterp = true;
			}
			return true;
		}

		return false;
	}

	if (bNeedToAdjustInterp != nullptr)
	{
		*bNeedToAdjustInterp = false;
	}

	return true;
}
*/
bool CBacktrack::IsDeltaTooBig(Vector& vPos1, Vector& vPos2)
{
	return (vPos1 - vPos2).LengthSqr() > 1600000.f; // temp until lagfix
}

void CBacktrack::SetSequenceNumber(int iSequenceNumber)
{
	m_iSequenceNumber = iSequenceNumber;
}

void CBacktrack::ProcessTick()
{
	CBaseEntityNew* LocalPlayer = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
	if (!bInit)
	{
		m_mEntities.clear();
		for (int index = 128; index >= 0; --index)
		{
			CBacktrackEntity NewPlayer;
			NewPlayer.m_pPlayerEntity = (C_BasePlayerNew*)nullptr;
			NewPlayer.m_cReconciledFrame = new CReconcileFrame();
			m_mEntities.insert(std::pair(index, NewPlayer));
		}
		bInit = true;
	}

	for (int index = CHiveInterface.Engine->GetMaxClients(); index >= 0; --index)
	{
		if (index == LocalPlayer->Index())
			continue;

		C_BasePlayerNew* Player = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(index);

		if (Player && Player->IsAlive())
		{
			if (m_mEntities.size() >= index)
			{
				for (std::map<int, CBacktrackEntity>::iterator it = m_mEntities.begin(); it != m_mEntities.end(); ++it)
				{
					if (it->first == index)
					{
						if (it->second.m_pPlayerEntity != Player)
						{
							it->second.m_pPlayerEntity = Player;
							it->second.m_vBacktrackFrames.clear();
						}
						bDisableBoneInterpolation = true;
						it->second.SaveFrame(m_iSequenceNumber);
						bDisableBoneInterpolation = false;
					}
				}
			}
		}
	}

	return;
}

void CBacktrack::Interp_WriteUserCmdDeltaToBuffer()
{
	{
		if (m_bHasToReset)
		{
			if (m_iResetTicks == 3)
			{
				float m_flInterp = CHiveInterface.Cvar->FindVar("cl_interp")->GetFloat();
				char buf[32];
				sprintf_s(buf, "%f", m_flInterp);
				NativeClass::SendNetMsg("cl_interp", buf);
				m_bHasToReset = false;
				m_iResetTicks = 0;
			}
			m_iResetTicks++;
		}

		if (m_bHasToChange)
		{
			char buf[32];
			sprintf_s(buf, "%f", m_flInterp);
			NativeClass::SendNetMsg("cl_interp", buf);
			m_bHasToChange = false;
			m_iResetTicks = 0;
			m_bHasToReset = true;
		}
	}
}

void CBacktrack::Interp_UpdateInterpolation(float flInterp)
{
	m_flInterp = flInterp;
	m_bHasToChange = true;
}

namespace HiveCheats
{
	CBacktrack cBacktrackInterface;
}