#pragma once
#include "hive_dependencies.h"
#include "hive_native.h"

namespace HiveMath {
	void CalculateAngle(Vector &src, Vector &dst, QAngle &angles);
	void MakeVector(QAngle angle, QAngle& vector);
	void VectorAngles(Vector &forward, QAngle &angles);
	void AngleVectors(const QAngle &angles, Vector &forward);
	void NormalizeAngles(QAngle &angle);
	float NormalizeAngleLength(QAngle &Ang);
}