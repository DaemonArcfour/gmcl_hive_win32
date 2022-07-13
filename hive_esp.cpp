#include "hive_cheats.h"
#include "hive_esp.h"
#include "hive_native.h"
#include "hive_util.h"
int iHardBtEntity2 = 2;
namespace HiveCheats {
	/*void DrawEsp() {
		if (!CLuaMenuCallback.ESP)
			return;
		for (int i = 0; i <= MaxClients; i++) {
			CBaseEntityNew* target = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(i);
			CBaseEntityNew* me = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());

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
	}*/

	CBaseEntityNew* target;
	CBaseEntityNew* me;

	void Start(int i) {
		me = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		target = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(i);
		//target_class = (ClientClass*)target->GetClientClass();
	}

	void PlayerESP() {
		ESPBox Box;
		player_info_t info;

		if (!target || (NativeClass::IsDormant(target) && !CLuaMenuCallback.ESPDormant) || !target->isAlive() || !CHiveInterface.Engine->GetPlayerInfo(target->Index(), &info))
			return;

		if (CLuaMenuCallback.BoneESP)
			HiveDraw::DrawSkeleton(Color(255, 255, 255, 255), target);
		if (HiveDraw::GetESPBox(target, Box)) {
			if (CLuaMenuCallback.ESPBox)
			{
				if( target->Index() == aimbot_target )
					HiveDraw::DrawCornerBox(Box, Color(117, 29, 175, 255));

				else if( HiveUTIL::IsFriend((C_BasePlayerNew*)target) )
					HiveDraw::DrawCornerBox(Box, Color(128, 255, 0, 255));

				else
					HiveDraw::DrawCornerBox(Box, Color(255, 180, 0, 255));
			}

			if (CLuaMenuCallback.ESPName)
				HiveDraw::DrawName(info, Box);

			if (CLuaMenuCallback.ESPDist)
				HiveDraw::DrawDist(target, me, Box);

			if (CLuaMenuCallback.ESPHealth)
				HiveDraw::DrawHealth(target, Box);

			if (CLuaMenuCallback.ESPArmor)
				HiveDraw::DrawArmor(target, Box);

			if(CLuaMenuCallback.Backtrack)
				HiveDraw::DrawBacktrackTicks(target);

			if (CLuaMenuCallback.ESPWeapon) {
				void* ent = CHiveSourceNative.GetPlayerActiveWeapon(CHiveInterface.EntityList->GetClientEntity(target->Index()));
				if (ent) {
					HiveDraw::DrawWeapon(CHiveSourceNative.GetClassName(ent), Box);
				}
				else {
					HiveDraw::DrawWeapon("None", Box);
				}
			}

			if (CLuaMenuCallback.ESPBarrel)
				HiveDraw::DrawBarrel(Color(0, 160, 255, 255), target);

			if (CLuaMenuCallback.ESPConnections)
				HiveDraw::DrawConnection(Box, Color(0, 255, 0), target);
		}
	}

	void EntityESP() {
		ESPBox Box;

		if (HiveDraw::GetESPBox(target, Box)) {
			if (CLuaMenuCallback.ESPBox)
				HiveDraw::DrawCornerBox(Box, Color(255, 180, 0, 255));

			if (CLuaMenuCallback.ESPName) {
				HiveDraw::DrawName(CHiveSourceNative.GetClassName(target), Box);
			}

			if (CLuaMenuCallback.ESPDist)
				HiveDraw::DrawDist(target, me, Box);

			if (CLuaMenuCallback.ESPConnections)
				HiveDraw::DrawConnection(Box, Color(0, 255, 0), target);
		}
	}

	bool EntityShouldDrawESP() {
		if (!target) return false;
		return CLuaMenuCallback.EntityESPList.find(CHiveSourceNative.GetClassName(target)) != CLuaMenuCallback.EntityESPList.end();
	}

	void DrawEsp() {

		if (CLuaMenuCallback.ESP) {
			for (int i = 0; i <= MaxClients; i++) {
				Start(i);

				if (!me) continue;
				if (i == me->Index()) continue;

				PlayerESP();
			}
		}

		if (CLuaMenuCallback.EntityESP) {
			int a = CHiveInterface.EntityList->GetHighestEntityIndex();


			for (int i = 0; i <= a; i++) {
				Start(i);
				//EntityShouldDrawESP();
				if (EntityShouldDrawESP())
					EntityESP();
			}
		}
	}
}
