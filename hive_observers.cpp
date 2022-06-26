#include "hive_draw.h"
#include "hive_cheats.h"


namespace HiveCheats {

	void DrawObservers() {
		if (!CLuaMenuCallback.ShowSpectators)
			return;
		HiveDraw::DrawString(100, 100, Color(255, 0, 0, 255), CHiveFonts.MediumFont, "Spectators: ");
		int pos_x = 100;
		CBaseEntityNew* LocalPlayer = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		for (int i = 0; i < MaxClients; i++)
		{
			CBaseEntityNew* target = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(i);
			player_info_t info;
			if(target){
				if (CHiveInterface.Engine->GetPlayerInfo(i, &info)) {
					C_BasePlayer* Observer = CHiveSourceNative.UTIL_GetPlayerByIndex(i);
					ConMsg("%x\n", Observer);
					if (Observer) {
						if (CHiveSourceNative.GetObserverTarget(Observer) == LocalPlayer) {
							pos_x += 10;
							HiveDraw::DrawString(pos_x, 0, Color(255, 0, 0, 255), HiveDraw::Fonts::ESP, info.name);
						}
					}
				}
			}
			
		}
	}
}