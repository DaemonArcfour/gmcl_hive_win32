#pragma once
#include "hive_netvar.h"

class CHiveEnginePrediction
{
public:
	int unpredictedFlags;
	int predictedFlags;

	CMoveData moveData;

public:
	void Start(GMODCUserCmd* cmd, C_BasePlayerNew* localPlayer);
	float GetServerTime(GMODCUserCmd* cmd = nullptr, int tickOffset = 0) const;
	void Finish(C_BasePlayerNew* localPlayer);

private:
	float _unpredictedCurtime;
	float _unpredictedFrametime;

	Vector _unpredictedVelocity;

	float _unsimulatedCurtime;
	float _unsimulatedFrametime;

	int _unsimulatedFlags;

	int _ticksToSimulate;

	int* _predictionRandomSeed;
}; extern CHiveEnginePrediction CHiveEnginePredict;