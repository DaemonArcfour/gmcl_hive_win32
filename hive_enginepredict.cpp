#include "hive_enginepredict.h"



void CHiveEnginePrediction::Start(GMODCUserCmd* cmd, C_BasePlayerNew* localPlayer)
{
	CBaseEntityNew* localEntity = (CBaseEntityNew*)localPlayer;
	localPlayer->GetCurrentCommand() = cmd;
	//*_predictionRandomSeed = cmd->random_seed;

	unpredictedFlags = localPlayer->getFlags();

	_unpredictedCurtime = CHiveInterface.Globals->curtime;
	_unpredictedFrametime = CHiveInterface.Globals->frametime;
	_unpredictedVelocity = localEntity->Velocity();

	CHiveInterface.Globals->curtime = CHiveInterface.Globals->interval_per_tick * localEntity->GetTickBase();
	CHiveInterface.Globals->frametime = CHiveInterface.Globals->interval_per_tick;
	
	if (CHiveInterface.ClientState->m_nDeltaTick > 0) {
		CHiveInterface.Prediction->Update(CHiveInterface.ClientState->m_nDeltaTick,
			CHiveInterface.ClientState->m_nDeltaTick > 0,
			CHiveInterface.ClientState->last_command_ack,
			CHiveInterface.ClientState->lastoutgoingcommand + CHiveInterface.ClientState->chokedcommands);
	}
	

	CHiveInterface.Prediction->SetLocalViewAngles(cmd->viewangles);

	CHiveInterface.GameMovement->StartTrackPredictionErrors((C_BasePlayer*)localPlayer);

	memset(&moveData, 0, sizeof(moveData));

	CHiveInterface.Prediction->SetupMove(localEntity, (CUserCmd*)cmd, CHiveInterface.MoveHelper, &moveData);
	CHiveInterface.GameMovement->ProcessMovement((C_BasePlayer*)localPlayer, &moveData);
	CHiveInterface.Prediction->FinishMove(localEntity, (CUserCmd*)cmd, &moveData);

	CHiveInterface.GameMovement->FinishTrackPredictionErrors((C_BasePlayer*)localPlayer);

	if (CHiveInterface.Globals->frametime > 0.f) 
	{
		localEntity->GetTickBase()++;
	}

	predictedFlags = localPlayer->getFlags();
}

void CHiveEnginePrediction::Finish(C_BasePlayerNew* localPlayer)
{
	CBaseEntityNew* localEntity = (CBaseEntityNew*)localPlayer;
	CHiveInterface.Globals->curtime = _unpredictedCurtime;
	CHiveInterface.Globals->frametime = _unpredictedFrametime;

	localEntity->Velocity() = _unpredictedVelocity;
	localEntity->GetFlags() = unpredictedFlags;

	//*_predictionRandomSeed = -1;
	localPlayer->GetCurrentCommand() = nullptr;
}