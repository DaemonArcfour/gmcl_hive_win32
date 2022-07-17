#include "hive_cheats.h"

namespace HiveCheats 
{
	int choked = 0;
	bool* bSendPacket = false;

	void FakeLag(GMODCUserCmd* cmd)
	{
		if(choked > (CLuaMenuCallback.bSendPChoke + CLuaMenuCallback.bSendPSend)){
			choked = 0;
		}
		*bSendPacket = CLuaMenuCallback.bSendPSend >= choked;
		choked++;
	}
}