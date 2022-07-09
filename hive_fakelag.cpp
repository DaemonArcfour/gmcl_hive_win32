#include "hive_cheats.h"

namespace HiveCheats 
{
	int choked = 0;
	void FakeLag(GMODCUserCmd* cmd)
	{
		*bSendPacket = choked >= CLuaMenuCallback.Choke;
		if (*bSendPacket)
			choked = 0;
		else
			choked++;
	}
}