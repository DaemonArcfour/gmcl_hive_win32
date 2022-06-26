#include "hive_native.h"
SourceNative CHiveSourceNative;
WeaponName *WNGrab;
GetIsDormant *IDGrab;
EntEyeAng *AngGrab;
cPaintTraverseConfig PaintTraverseConfig;
namespace NativeClass {
	DWORD CL = (DWORD)GetModuleHandle("client.dll");
	DWORD CLSize = HiveScanner::GetModuleSize("client.dll");

	QAngle GetEntEyeAng(CBaseEntityNew *Target) {
		DWORD PageGuard;
		AngGrab = (EntEyeAng*)Target;
				return AngGrab->EyeAngles;
	}


	bool IsDormant(CBaseEntityNew *Target) {
		DWORD PageGuard;
		IDGrab = (GetIsDormant*)Target;
		// Avoid possible Access Violation errors.
		if (VirtualProtect(IDGrab, sizeof(GetIsDormant), PAGE_READWRITE, &PageGuard)) {
			if (PageGuard == PAGE_READWRITE || PageGuard == PAGE_READONLY || PageGuard == PAGE_EXECUTE_READWRITE) {
				VirtualProtect(IDGrab, sizeof(GetIsDormant), PageGuard, NULL);
				
				if(IDGrab->isDormant == 255)
					return 1;
				if (IDGrab->isDormant == 0)
					return 0;

				return 0;
			}
			else {
				HiveTroubleshooter::Print("Access violation in \"isDormant\"\n", ERROR);
				return 0;
			}
		}
		else return 0;
	}

	void GetBonePosition(void *_this, const char* attachment, Vector &origin, QAngle &angles) {
		if (_this && attachment != NULL) {
			auto ppp_entAnim = (*(int(__thiscall **)(void*))(*(DWORD *)_this + 156))(_this);					// found conversion in DispatchParticleEffect with
																												//the following comment "Model '%s' doesn't have attachment '%s' to attach particle system '%s' to.\n"  (v3)
			CHiveSourceNative.GetBonePosition((void*)ppp_entAnim, CHiveSourceNative.LookUpBone((void*)ppp_entAnim, attachment), origin, angles);
		}
		else {
			return;
		}
	}
	bool* bSendPacket = (bool*)CHiveSourceNative.offset_bSendPacket;

	char* BonesTable[20]{
		"ValveBiped.Bip01_Head1"	 ,
		"ValveBiped.Bip01_Neck1"	 ,
		"ValveBiped.Bip01_Spine4"	 ,
		"ValveBiped.Bip01_Spine2"	 ,
		"ValveBiped.Bip01_Spine1"	 ,
		"ValveBiped.Bip01_Spine"	 ,
		"ValveBiped.Bip01_R_UpperArm",
		"ValveBiped.Bip01_R_Forearm" ,
		"ValveBiped.Bip01_R_Hand"	 ,
		"ValveBiped.Bip01_L_UpperArm",
		"ValveBiped.Bip01_L_Forearm" ,
		"ValveBiped.Bip01_L_Hand"	 ,
		"ValveBiped.Bip01_R_Thigh"	 ,
		"ValveBiped.Bip01_R_Calf"	 ,
		"ValveBiped.Bip01_R_Foot"	 ,
		"ValveBiped.Bip01_R_Toe0"	 ,
		"ValveBiped.Bip01_L_Thigh"	 ,
		"ValveBiped.Bip01_L_Calf"	 ,
		"ValveBiped.Bip01_L_Foot"	 ,
		"ValveBiped.Bip01_L_Toe0"
	};
	char* PriorityPoints[21]{
		"ValveBiped.Bip01_Head1"	 ,
		"ValveBiped.Bip01_Neck1"	 ,
		"ValveBiped.Bip01_Spine4"	 ,
		"ValveBiped.Bip01_Spine2"	 ,
		"ValveBiped.Bip01_Spine1"	 ,
		"ValveBiped.Bip01_Spine"	 ,
		"ValveBiped.Bip01_L_Calf"	 ,
		"ValveBiped.Bip01_R_Calf"	 ,
		"ValveBiped.Bip01_R_Thigh"	 ,
		"ValveBiped.Bip01_L_Thigh"	 ,
		"ValveBiped.Bip01_L_UpperArm",
		"ValveBiped.Bip01_L_Forearm" ,
		"ValveBiped.Bip01_R_UpperArm",
		"ValveBiped.Bip01_R_Forearm" ,
		"ValveBiped.Bip01_R_Hand"	 ,
		"ValveBiped.Bip01_L_Hand"	 ,
		"ValveBiped.Bip01_R_Foot"	 ,
		"ValveBiped.Bip01_R_Toe0"	 ,
		"ValveBiped.Bip01_L_Foot"	 ,
		"ValveBiped.Bip01_L_Toe0"	 ,
		"ValveBiped.Bip01_Head1"
	};

}