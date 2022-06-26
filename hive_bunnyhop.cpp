#include "hive_cheats.h"

void HiveCheats::Bunnyhop(GMODCUserCmd* ucmd, CBaseEntityNew* LocalPlayer) {
	//int iFlags = LocalPlayer->GetFlags();
	int gEntity = LocalPlayer->GetGroundEntityHandle();
	static int gEntOccurances = 0;
	if (ucmd->buttons & IN_JUMP) {
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