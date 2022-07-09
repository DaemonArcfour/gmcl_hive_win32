#include "hive_cheats.h"
#include "hive_native.h"

CBacktrack CHiveBacktrackInterface;

CBacktrackFrame::CBacktrackFrame()
{
		m_iTickCount = -1;
		m_fSimulationTime = -1;
		m_vHeadPosition = Vector(0, 0, 0);
		m_vOrigin = Vector(0, 0, 0);
}

void CBacktrackEntity::SaveFrame(GMODCUserCmd* cmd)
{
	if (!m_pPlayerEntity)
		return;

	Vector vHeadPosition;
	matrix3x4_t mtBones[128];
	if (NativeClass::GetBonePosition(m_pPlayerEntity, NativeClass::PriorityPoints[0], vHeadPosition, mtBones))
	{
		int iTickCount = cmd->tick_count;
		CBacktrackFrame BTCurrentFrame;
		BTCurrentFrame.m_iTickCount = iTickCount;
		BTCurrentFrame.m_vHeadPosition = vHeadPosition;
		BTCurrentFrame.m_fSimulationTime = ((CBaseEntityNew*)m_pPlayerEntity)->flSimulationTime();
		BTCurrentFrame.m_vOrigin = ((CBaseEntityNew*)m_pPlayerEntity)->GetAbsOrigin();
		memcpy(BTCurrentFrame.m_mtBones, mtBones, sizeof(BTCurrentFrame.m_mtBones));
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

void CBacktrack::ProcessTick(GMODCUserCmd* cmd)
{
	CBaseEntityNew* LocalPlayer = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
	if (!bInit)
	{
		for (int index = MAX_PLAYERS; index >= 0; --index)
		{
			CBacktrackEntity NewPlayer;
			NewPlayer.m_pPlayerEntity = (C_BasePlayerNew*)nullptr;
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