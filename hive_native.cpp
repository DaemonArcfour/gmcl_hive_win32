#include "hive_native.h"
SourceNative CHiveSourceNative;
WeaponName *WNGrab;
GetIsDormant *IDGrab;
EntEyeAng *AngGrab;
cPaintTraverseConfig PaintTraverseConfig;
class CustomMessage
{
public:
	char dump[36];
};
namespace NativeClass {
	DWORD CL = (DWORD)GetModuleHandle("client.dll");
	DWORD CLSize = HiveScanner::GetModuleSize("client.dll");

	QAngle GetEntEyeAng(CBaseEntityNew *Target) {
		DWORD PageGuard;
		AngGrab = (EntEyeAng*)Target;
				return AngGrab->EyeAngles;
	}

	int Studio_BoneIndexByName(studiohdr_t* pStudioHdr, char const* pName)
	{
		for (int z = 0; z < pStudioHdr->numbones; z++)
		{
			auto bone = pStudioHdr->pBone(z);
			if (bone && bone->parent >= 0)
			{
				if (!(bone->flags & 256))
					continue;

				if (strcmp(bone->pszName(), pName) == 0)
					return z;
			}
		}
		return 0;
	}
	void Studio_GenerateBoneMap(studiohdr_t* pStudioHdr, matrix3x4_t* bones, std::map<const char*, Vector> &m_mBoneMap)
	{
		for (int z = 0; z < pStudioHdr->numbones; z++)
		{
			mstudiobone_t* bone = pStudioHdr->pBone(z);
			if (bone && bone->parent >= 0)
			{
				if (!(bone->flags & 256))
					continue;

				for (int i = 0; i < sizeof(NativeClass::BonesTable) / sizeof(char*); i++)
				{
					if (strcmp(bone->pszName(), NativeClass::BonesTable[i]) == 0)
						m_mBoneMap.insert(std::pair(NativeClass::BonesTable[i], Vector(bones[z][0][3], bones[z][1][3], bones[z][2][3])));
				}
			}
		}

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

	bool GetBonePosition(void *_this, const char* attachment, Vector &origin) 
	{
		matrix3x4_t bones[128];
		C_BasePlayerNew* Player = (C_BasePlayerNew*)_this;
		uintptr_t ClientRenderable = (uintptr_t)Player->GetClientRenderable();
		float EngineTime = CHiveInterface.Engine->Time();
		bool Setup = Player->GetClientRenderable()->SetupBones(bones, 128, BONE_USED_BY_HITBOX, EngineTime);
		if (ClientRenderable < 0x1000 || !Setup)
			return false;
		studiohdr_t* studioHdr = CHiveInterface.ModelInfo->GetStudiomodel((const model_t*)Player->GetClientRenderable()->GetModel());
		if (studioHdr)
		{
			int boneid = Studio_BoneIndexByName(studioHdr, attachment);
			origin = Vector(bones[boneid][0][3], bones[boneid][1][3], bones[boneid][2][3]);
			return true;
		}
		return false;
		/*
		if (_this && attachment != NULL) {
			auto ppp_entAnim = (*(int(__thiscall **)(void*))(*(DWORD *)_this + 156))(_this);					// found conversion in DispatchParticleEffect with
																												//the following comment "Model '%s' doesn't have attachment '%s' to attach particle system '%s' to.\n"  (v3)
			CHiveSourceNative.GetBonePosition((void*)ppp_entAnim, CHiveSourceNative.LookUpBone((void*)ppp_entAnim, attachment), origin, angles);
		}
		else {
			return;
		}
		*/
	}

	bool GetBonePosition(void* _this, const char* attachment, Vector& origin, matrix3x4_t* bones)
	{
		C_BasePlayerNew* Player = (C_BasePlayerNew*)_this;
		uintptr_t ClientRenderable = (uintptr_t)Player->GetClientRenderable();
		float EngineTime = CHiveInterface.Engine->Time();
		bool Setup = Player->GetClientRenderable()->SetupBones(bones, 128, BONE_USED_BY_HITBOX, EngineTime);
		if (ClientRenderable < 0x1000 || !Setup)
			return false;
		studiohdr_t* studioHdr = CHiveInterface.ModelInfo->GetStudiomodel((const model_t*)Player->GetClientRenderable()->GetModel());
		if (studioHdr)
		{
			int boneid = Studio_BoneIndexByName(studioHdr, attachment);
			origin = Vector(bones[boneid][0][3], bones[boneid][1][3], bones[boneid][2][3]);
			return true;
		}
		return false;
	}

	bool GetBoneMap(void* _this, std::map<const char*, Vector> &m_mBoneMap)
	{
		matrix3x4_t bones[128];
		C_BasePlayerNew* Player = (C_BasePlayerNew*)_this;
		uintptr_t ClientRenderable = (uintptr_t)Player->GetClientRenderable();
		float EngineTime = CHiveInterface.Engine->Time();
		bool Setup = Player->GetClientRenderable()->SetupBones(bones, 128, BONE_USED_BY_HITBOX, EngineTime);
		if (ClientRenderable < 0x1000 || !Setup)
			return false;
		studiohdr_t* studioHdr = CHiveInterface.ModelInfo->GetStudiomodel((const model_t*)Player->GetClientRenderable()->GetModel());
		if (studioHdr)
		{
			Studio_GenerateBoneMap(studioHdr, bones, m_mBoneMap);
			return true;
		}
		return false;
	}

	bool GetBoneMap(void* _this, std::map<const char*, Vector>& m_mBoneMap, matrix3x4_t* bones, bool bBoneInterpolation = true)
	{
		C_BasePlayerNew* Player = (C_BasePlayerNew*)_this;
		Vector vAbsOrigin = Player->GetAbsOrigin();
		if (!bBoneInterpolation)
			CHiveSourceNative.SetAbsOrigin(Player, ((CBaseEntityNew*)Player)->GetOrigin());

		uintptr_t ClientRenderable = (uintptr_t)Player->GetClientRenderable();
		float EngineTime = CHiveInterface.Engine->Time();
		bool Setup = Player->GetClientRenderable()->SetupBones(bones, 128, BONE_USED_BY_HITBOX, EngineTime);
		if (ClientRenderable < 0x1000 || !Setup)
		{
			if (!bBoneInterpolation)
				CHiveSourceNative.SetAbsOrigin(Player, vAbsOrigin);
			return false;
		}

		studiohdr_t* studioHdr = CHiveInterface.ModelInfo->GetStudiomodel((const model_t*)Player->GetClientRenderable()->GetModel());
		if (studioHdr)
		{
			Studio_GenerateBoneMap(studioHdr, bones, m_mBoneMap);
			if (!bBoneInterpolation)
				CHiveSourceNative.SetAbsOrigin(Player, vAbsOrigin);
			return true;
		}
		if (!bBoneInterpolation)
			CHiveSourceNative.SetAbsOrigin(Player, vAbsOrigin);
		return false;
	}

	void SendNetMsg(const char* name, const char* value)
	{
		CNetChan* pNetChan = (CNetChan*)CHiveInterface.Engine->GetNetChannelInfo();
		char* pContructorBuffer = new char[36];
		ZeroMemory(pContructorBuffer, 36);
		CHiveSourceNative.NET_SetConvar_Constructor((void*)pContructorBuffer, name, value);
		pNetChan->SendNetMsg(pContructorBuffer, false, false);
		delete[] pContructorBuffer;
		
	}

	bool* bSendPacket = (bool*)CHiveSourceNative.offset_bSendPacket;

	char* BonesTable[20]{
		"ValveBiped.Bip01_Head1"	 , // 0
		"ValveBiped.Bip01_Neck1"	 ,
		"ValveBiped.Bip01_Spine4"	 ,
		"ValveBiped.Bip01_Spine2"	 ,
		"ValveBiped.Bip01_Spine1"	 , // 4
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
		"ValveBiped.Bip01_Head1"	 , // 0
		"ValveBiped.Bip01_Neck1"	 ,
		"ValveBiped.Bip01_Spine4"	 ,
		"ValveBiped.Bip01_Spine2"	 ,
		"ValveBiped.Bip01_Spine1"	 , // 4
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