#include "hive_dependencies.h"

namespace HiveMath {
	void CalculateAngle(Vector &src, Vector &dst, QAngle &angles)
	{
		Vector tmp = dst - src;
		VectorAngles(tmp, angles);
	}

	void MakeVector(QAngle angle, QAngle& vector)
	{
		float pitch;
		float yaw;
		float tmp;

		pitch = (float)(angle[0] * M_PI / 180);
		yaw = (float)(angle[1] * M_PI / 180);
		tmp = (float)cos(pitch);

		vector[0] = (float)(-tmp * -cos(yaw));
		vector[1] = (float)(sin(yaw)*tmp);
		vector[2] = (float)-sin(pitch);
	}

	void AngleVectors(const QAngle &angles, Vector &forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	void VectorAngles(Vector &forward, QAngle &angles)
	{
		Assert(s_bMathlibInitialized);
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	void NormalizeAngles(QAngle &angle)
	{
		for (int axis = 0; axis < 3; ++axis)
		{
			while (angle[axis] > 180.f)
			{
				angle[axis] -= 360.f;
			}

			while (angle[axis] < -180.f)
			{
				angle[axis] += 360.f;
			}
		}
	}

	float NormalizeAngleLength(QAngle &Ang)
	{
		float flLength = Ang.Length();
		float flLengthNormal = 1.f / (FLT_EPSILON + flLength);
		Ang.x = Ang.x * flLengthNormal;
		Ang.y = Ang.y * flLengthNormal;
		Ang.z = Ang.z * flLengthNormal;

		return flLength;
	}
}