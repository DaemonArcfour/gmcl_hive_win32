#include "hive_cheats.h"
#include "hive_esp.h"
#include "hive_native.h"
#include "hive_util.h"
int iHardBtEntity2 = 1;
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

			//if(CLuaMenuCallback.Backtrack)
				//HiveDraw::DrawBacktrackTicks(target);

			if (HiveCheats::cBacktrackInterface.bInit && CLuaMenuCallback.Backtrack)
			{
				if (HiveCheats::cBacktrackInterface.m_mEntities[iHardBtEntity2].m_pPlayerEntity)
				{
					for (int i = 0; i < CLuaMenuCallback.Backtrack_max_tick; i++)
					{
						std::map<const char*, Vector> BoneMap = HiveCheats::cBacktrackInterface.m_mEntities[iHardBtEntity2].GetFrame(i).m_mBoneMap;
						Vector bonepos = BoneMap["ValveBiped.Bip01_Spine2"];
						Vector abspos = HiveCheats::cBacktrackInterface.m_mEntities[iHardBtEntity2].GetFrame(i).m_vAbsOrigin;
						Vector originpos = HiveCheats::cBacktrackInterface.m_mEntities[iHardBtEntity2].GetFrame(i).m_vOrigin;
						abspos.z += 32;
						originpos.z += 64;
						Vector w2s;
						Vector w2s2;
						Vector w2s3;
						HiveDraw::WorldToScreen(bonepos, w2s);
						HiveDraw::WorldToScreen(abspos, w2s2);
						HiveDraw::WorldToScreen(originpos, w2s3);
						if (i == CLuaMenuCallback.tmpBacktrackTick)
						{
							HiveDraw::FillRGBA(w2s.x, w2s.y, 5, 5, 255, 0, 0, 150);
							HiveDraw::FillRGBA(w2s2.x, w2s2.y, 5, 5, 0, 255, 0, 150);
							Color col = Color(255, 255, 255, 255);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[0]], BoneMap[NativeClass::BonesTable[1]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[1]], BoneMap[NativeClass::BonesTable[2]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[2]], BoneMap[NativeClass::BonesTable[3]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[3]], BoneMap[NativeClass::BonesTable[4]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[4]], BoneMap[NativeClass::BonesTable[5]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[2]], BoneMap[NativeClass::BonesTable[6]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[6]], BoneMap[NativeClass::BonesTable[7]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[7]], BoneMap[NativeClass::BonesTable[8]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[2]], BoneMap[NativeClass::BonesTable[9]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[9]], BoneMap[NativeClass::BonesTable[10]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[10]],BoneMap[NativeClass::BonesTable[11]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[5]], BoneMap[NativeClass::BonesTable[12]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[12]],BoneMap[NativeClass::BonesTable[13]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[13]],BoneMap[NativeClass::BonesTable[14]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[14]],BoneMap[NativeClass::BonesTable[15]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[5]], BoneMap[NativeClass::BonesTable[16]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[16]],BoneMap[NativeClass::BonesTable[17]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[17]],BoneMap[NativeClass::BonesTable[18]], col);
							HiveDraw::DrawBone(BoneMap[NativeClass::BonesTable[18]],BoneMap[NativeClass::BonesTable[19]], col);
							//HiveDraw::DrawCircle(w2s2.x, w2s2.y, 10, Color(0, 255, 0, 255));
							//HiveDraw::DrawCircle(w2s3.x, w2s3.y, 10, Color(0, 0, 255, 255));
						}
						else
						{
							HiveDraw::FillRGBA(w2s.x, w2s.y, 5, 5, 255, 100, 40, 150);
							HiveDraw::FillRGBA(w2s2.x, w2s2.y, 5, 5, 40, 100, 255, 150);
							//HiveDraw::DrawCircle(w2s2.x, w2s2.y, 10, Color(40, 100, 255, 255));
							//HiveDraw::DrawCircle(w2s3.x, w2s3.y, 10, Color(200, 100, 255, 255));
						}
					}
				}
			}

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
