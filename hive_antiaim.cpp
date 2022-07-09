#include "hive_cheats.h"
#include "hive_math.h"

namespace HiveCheats {
	bool* bSendPacket;
	void Antiaim(GMODCUserCmd* cmd)
	{
		if (!cmd->command_number)
			return;

		int iAntiaim_Type = CLuaMenuCallback.AntiaimType;
		switch (iAntiaim_Type)
		{
			case 0:
				cmd->viewangles.x = -180.000005f;
				break;

			case 1:
				cmd->viewangles.x = 180.0000076294f;
				break;

			case 2:
				cmd->viewangles.x = 180.00000762939f;
				break;

			default:
				break;
		}
	}

	void CorrectMovement(QAngle vOldAngles, GMODCUserCmd* pCmd, float fOldForward, float fOldSidemove) {
		QAngle NormalizedViewAngles = pCmd->viewangles;
		NormalizedViewAngles.z = 1;
		if (NormalizedViewAngles.x > 89.f || NormalizedViewAngles.x < -89.f)
		{
			NormalizedViewAngles.y += 180;
			NormalizedViewAngles.z = -1;
		}

		HiveMath::NormalizeAngles(NormalizedViewAngles);
		
		float deltaView = NormalizedViewAngles.y - vOldAngles.y;
		float f1;
		float f2;

		if (pCmd->viewangles == vOldAngles)
			return;

		if (vOldAngles.y < 0.f)
			f1 = 360.0f + vOldAngles.y;
		else
			f1 = vOldAngles.y;

		if (NormalizedViewAngles.y < 0.0f)
			f2 = 360.0f + NormalizedViewAngles.y;
		else
			f2 = NormalizedViewAngles.y;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);
		deltaView = 360.0f - deltaView;

		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = (sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove) * NormalizedViewAngles.z;
	}
}