#include "hive_cheats.h"

void HiveCheats::Bunnyhop(GMODCUserCmd* ucmd, CBaseEntityNew* LocalPlayer) {
	//int iFlags = LocalPlayer->GetFlags();
	int gEntity = LocalPlayer->GetGroundEntityHandle();
	int movetype = LocalPlayer->GetMoveType();

	static int gEntOccurances = 0;
	if (ucmd->buttons & IN_JUMP && movetype != MOVETYPE_NOCLIP && movetype != MOVETYPE_LADDER) {
		if (gEntity == -1/*!(iFlags & FL_ONGROUND)*/) {
			ucmd->buttons &= ~IN_JUMP;
			gEntOccurances = 0;
			return;
		}

		if (gEntOccurances == 1) {
			ucmd->buttons &= ~IN_JUMP;
		}

		if (gEntOccurances == 2) {
			ucmd->buttons |= IN_JUMP;
			gEntOccurances = 0;
		}

		if (gEntity != -1)
			gEntOccurances++;
	}
}