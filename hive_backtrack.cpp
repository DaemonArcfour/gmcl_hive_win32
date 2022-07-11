#include "hive_cheats.h"
#include "hive_native.h"

CBacktrack CHiveBacktrackInterface;

CBacktrackFrame::CBacktrackFrame()
{
		m_iTickCount = -1;
		m_fSimulationTime = -1;
		m_vAbsOrigin = Vector(0, 0, 0);
}

void CBacktrackEntity::SaveFrame(GMODCUserCmd* cmd)
{
	if (!m_pPlayerEntity)
		return;

	matrix3x4_t mtBones[128];
	CBacktrackFrame BTCurrentFrame;
	if (NativeClass::GetBoneMap(m_pPlayerEntity, BTCurrentFrame.m_mBoneMap, BTCurrentFrame.m_mtBones))
	{
		int iTickCount = cmd->tick_count;
		BTCurrentFrame.m_iTickCount = iTickCount;
		BTCurrentFrame.m_fSimulationTime = ((CBaseEntityNew*)m_pPlayerEntity)->flSimulationTime();
		BTCurrentFrame.m_vAbsOrigin = m_pPlayerEntity->GetAbsOrigin();
		BTCurrentFrame.m_vAbsAngles = m_pPlayerEntity->GetAbsAngles();
		BTCurrentFrame.m_vOOBMins = ((CBaseEntityNew*)m_pPlayerEntity)->OBBMins();
		BTCurrentFrame.m_vOOBMaxs = ((CBaseEntityNew*)m_pPlayerEntity)->OBBMaxs();
		BTCurrentFrame.m_vOrigin = ((CBaseEntityNew*)m_pPlayerEntity)->GetOrigin();
		m_vBacktrackFrames.push_front(BTCurrentFrame);
		if (m_vBacktrackFrames.size() > CLuaMenuCallback.Backtrack_max_tick)
			m_vBacktrackFrames.pop_back();
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

void CBacktrack::Reconcile(GMODCUserCmd* cmd, CBacktrackEntity BTEntity, int iTick)
{
	if (/*!BTEntity.m_cReconciledFrame->bReconciled &&*/ BTEntity.m_pPlayerEntity)
	{

		BTEntity.m_cReconciledFrame->bReconciled = true;
		//uintptr_t* BaseAnimating = (uintptr_t*)(*(int(__thiscall**)(void*))(*(DWORD*)BTEntity.m_pPlayerEntity + 156))(BTEntity.m_pPlayerEntity);
		CHiveSourceNative.InvalidateBoneCache(BTEntity.m_pPlayerEntity);
		int BoneCacheSize = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrixSize;
		matrix3x4_t* BoneMatrixCache = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrix;

		CBacktrackFrame ReconciliationFrame = BTEntity.GetFrame(iTick);
		BTEntity.m_cReconciledFrame->m_vAbsOrigin = BTEntity.m_pPlayerEntity->GetAbsOrigin();
		BTEntity.m_cReconciledFrame->m_vAbsAngles = BTEntity.m_pPlayerEntity->GetAbsAngles();
		BTEntity.m_cReconciledFrame->m_vOOBMins = ((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->OBBMins();
		BTEntity.m_cReconciledFrame->m_vOOBMaxs = ((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->OBBMaxs();
		BTEntity.m_cReconciledFrame->m_vOrigin = ((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->GetOrigin();
		memcpy(BTEntity.m_cReconciledFrame->m_mtBones, BoneMatrixCache, BoneCacheSize * sizeof(matrix3x4_t));

		BTEntity.m_pPlayerEntity->GetAbsOrigin() = ReconciliationFrame.m_vAbsOrigin;
		BTEntity.m_pPlayerEntity->GetAbsAngles() = ReconciliationFrame.m_vAbsAngles;
		((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->GetOrigin() = ReconciliationFrame.m_vOrigin;
		memcpy(BoneMatrixCache, ReconciliationFrame.m_mtBones, BoneCacheSize * sizeof(matrix3x4_t));
		cmd->tick_count = ReconciliationFrame.m_iTickCount;
		//CHiveSourceNative.SetCollisionBounds(BTEntity.m_pPlayerEntity->GetCollideable(), ReconciliationFrame.m_vOOBMins, ReconciliationFrame.m_vOOBMaxs);
	}
}

void CBacktrack::EndReconciliation(CBacktrackEntity BTEntity)
{
	if (BTEntity.m_cReconciledFrame->bReconciled && BTEntity.m_pPlayerEntity)
	{
		BTEntity.m_cReconciledFrame->bReconciled = false;
		int BoneCacheSize = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrixSize;
		matrix3x4_t* BoneMatrixCache = ((CBoneAccessorNew*)BTEntity.m_pPlayerEntity)->BoneMatrix;

		BTEntity.m_pPlayerEntity->GetAbsOrigin() = BTEntity.m_cReconciledFrame->m_vAbsOrigin;
		BTEntity.m_pPlayerEntity->GetAbsAngles() = BTEntity.m_cReconciledFrame->m_vAbsAngles;
		((CBaseEntityNew*)BTEntity.m_pPlayerEntity)->GetOrigin() = BTEntity.m_cReconciledFrame->m_vOrigin;
		memcpy(BoneMatrixCache, BTEntity.m_cReconciledFrame->m_mtBones, BoneCacheSize * sizeof(matrix3x4_t));
		//CHiveSourceNative.SetCollisionBounds(BTEntity.m_pPlayerEntity->GetCollideable(), BTEntity.m_cReconciledFrame.m_vOOBMins, BTEntity.m_cReconciledFrame.m_vOOBMaxs);
	}
}

void CBacktrack::ProcessTick(GMODCUserCmd* cmd)
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

						it->second.SaveFrame(cmd);
					}
				}
			}
		}
	}

	return;
}
namespace HiveCheats
{
	CBacktrack cBacktrackInterface;
}