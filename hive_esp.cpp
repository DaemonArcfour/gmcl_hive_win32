#include "hive_cheats.h"
#include "hive_esp.h"
#include "hive_native.h"

namespace HiveCheats {
	void DrawEsp() {
		if (!CLuaMenuCallback.ESP)
			return;
		for (int i = 0; i <= MaxClients; i++) {
			CBaseEntityNew* target = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(i);
			CBaseEntityNew* me = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
			//CBaseEntityNew* weapon = (CBaseEntityNew*);
			if (!me)
				continue;
			if (i == me->Index())
				continue;
			player_info_t info;

			if (!target || NativeClass::IsDormant(target) || !target->isAlive() || !CHiveInterface.Engine->GetPlayerInfo(i, &info))
				continue;

			ESPBox Box;
			
					if (CLuaMenuCallback.BoneESP)
						HiveDraw::DrawSkeleton(Color(255, 255, 255, 255), target);
					if (HiveDraw::GetESPBox(target, Box)) {
						if (CLuaMenuCallback.ESPBox)
							HiveDraw::DrawCornerBox(Box, Color(255, 180, 0, 255));
						if(CLuaMenuCallback.ESPName)
							HiveDraw::DrawName(info, Box);

						if (CLuaMenuCallback.ESPDist)
							HiveDraw::DrawDist(target, me, Box);

						if(CLuaMenuCallback.ESPHealth)
							HiveDraw::DrawHealth(target, Box);

						if (CLuaMenuCallback.ESPWeapon) {
							void* ent = CHiveSourceNative.GetPlayerActiveWeapon(CHiveInterface.EntityList->GetClientEntity(i));
							if (ent) {
								HiveDraw::DrawWeapon(CHiveSourceNative.GetClassName(ent), Box);
							}
							else {
								HiveDraw::DrawWeapon("None", Box);
							}
						}
						
						if(CLuaMenuCallback.ESPBarrel)
							HiveDraw::DrawBarrel(Color(0, 160, 255, 255), target);

						if (CLuaMenuCallback.ESPConnections)
							HiveDraw::DrawConnection(Box, Color(0, 255, 0), target);
					}

			}
	}
}
