#include "hive_cheats.h"
int HiveCheats::autostrafe_angle = 450;
void HiveCheats::Autostrafe(GMODCUserCmd* cmd)
{
	if (!(cmd->buttons & IN_BACK) && !(cmd->buttons & IN_FORWARD))
	{
		CBaseEntityNew * Local = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		int iFlags = Local->GetFlags();
		if (!(iFlags & FL_ONGROUND) && !(iFlags & FL_PARTIALGROUND) && !(iFlags & FL_INWATER)) {
			Vector vVelocity = Local->Velocity();
			float Speed = sqrt(vVelocity.x * vVelocity.x + vVelocity.y * vVelocity.y);

			float Tickrate;

			if (CHiveInterface.Globals->interval_per_tick * 100 > 1)
				Tickrate = 1.1;
			else
				Tickrate = 1;

			static float OldY;
			float Difference = AngleNormalize(cmd->viewangles.y - OldY);


			float Value = (8.15 - Tickrate) - (Speed / 340);

			if (Speed > 160 && Speed < 420)
			{
				Value = (4.6 - Tickrate) - (Speed / 340);
			}
			if (Speed > 420)
			{
				Value = (3 - Tickrate) - (Speed / 1000);
			}
			if (Value <= 0.275)
			{
				Value = 0.275;
			}

			if (abs(Difference) < Value)
			{
				static bool Flip = false;
				if (Flip)
				{
					cmd->viewangles.y -= Value;
					cmd->sidemove = -autostrafe_angle;
					Flip = false;
				}
				else
				{
					cmd->viewangles.y += Value;
					cmd->sidemove = autostrafe_angle;
					Flip = true;
				}
			}
			else
			{
				if (Difference > 0.0f)
				{
					cmd->sidemove = -autostrafe_angle;
				}
				if (Difference < 0.0f)
				{
					cmd->sidemove = autostrafe_angle;
				}
			}


			OldY = cmd->viewangles.y;
		}
	}
}