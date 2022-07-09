#include "hive_troubleshooter.h"
#include "hive_native.h"
Color extern HIVE_CON(255, 179, 0, 255);
Color extern HIVE_WHITE(255, 255, 255, 255);
Color extern HIVE_RED(255, 0, 0, 255);
Color extern HIVE_OK(0, 255, 20, 255);
Color extern HIVE_TROUBLESHOOTER(0, 180, 255, 255);
Color extern HIVE_LIGHTGREEN(120, 255, 0, 255);
Color extern HIVE_LIGHTBLUE(145, 255, 255, 255);
Color extern HIVE_PURPLE(255, 0, 120, 255);
Color extern HIVE_DARKBLUE(71,107,135,255);
DWORD dummy;

namespace HiveTroubleshooter {
	void PrintError(std::string msg, bool success, bool &bError) {
		msg += "\n";
		if (success) {
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_TROUBLESHOOTER, "TS");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_OK, "+");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_CON, msg.c_str());
		}
		else {
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_TROUBLESHOOTER, "TS");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_RED, "-");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_CON, msg.c_str());
			bError = true;
		}
	}

	void Print(std::string msg, bool success) {
		msg += "\n";
		if (success) {
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_OK, "+");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_CON, msg.c_str());
		}
		else {
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_RED, "-");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_CON, msg.c_str());
		}
	}
	
	void PrintInitizalization(std::string msg) {
		msg += "\n";
			ConColorMsg(HIVE_WHITE, "[");
			ConColorMsg(HIVE_LIGHTGREEN, "~");
			ConColorMsg(HIVE_WHITE, "] ");
			ConColorMsg(HIVE_PURPLE, msg.c_str());
	}

	void PrintBlock(std::string msg) {
		msg += "\n";
		ConColorMsg(HIVE_WHITE, "[");
		ConColorMsg(HIVE_RED, "!");
		ConColorMsg(HIVE_WHITE, "] ");
		ConColorMsg(HIVE_LIGHTBLUE, msg.c_str());
	}

	void PrintOpenSSL(std::string msg) {
		msg += "\n";
		ConColorMsg(HIVE_WHITE, "[");
		ConColorMsg(HIVE_LIGHTBLUE, "OpenSSL");
		ConColorMsg(HIVE_WHITE, "] ");
		ConColorMsg(HIVE_LIGHTGREEN, msg.c_str());
	}

	void PrintLUA(std::string msg) {
		msg += "\n";
		ConColorMsg(HIVE_WHITE, "[");
		ConColorMsg(HIVE_LIGHTBLUE, "LUA");
		ConColorMsg(HIVE_WHITE, "] ");
		ConColorMsg(HIVE_DARKBLUE, msg.c_str());
	}

	void Warning() {
		//PlaySoundW(L"HiveSounds\\error.wav", NULL, SND_ASYNC);
		Print("Errors found, further module execution has been stopped to prevent crashes! Have a very safe day.", ERROR);
	}

	bool factoryErrors() {
		bool bError = false;

		if (!CHiveFactory.ClientFactory)
			PrintError("Failed to get client factory.", ERROR, bError);
		else
			PrintError("Client factory loaded!", OK, bError);

		if (!CHiveFactory.EngineFactory)
			PrintError("Failed to get engine factory.", ERROR, bError);
		else
			PrintError("Engine factory loaded!", OK, bError);
		/* Disabled, because its not used.
		if (!CHiveFactory.FileSystemFactory)
			PrintError("Failed to get file system factory.", ERROR, bError);
		else
			PrintError("File system factory loaded!.", OK, bError);
		*/

		if (!CHiveFactory.GameUIFactory)
			PrintError("Failed to get file GameUI factory.", ERROR, bError);
		else
			PrintError("GameUI factory loaded!", OK, bError);

		if (!CHiveFactory.LuaFactory)
			PrintError("Failed to get LUA factory.", ERROR, bError);
		else
			PrintError("LUA factory loaded!", OK, bError);

		if (!CHiveFactory.MaterialFactory)
			PrintError("Failed to get material factory.", ERROR, bError);
		else
			PrintError("Material factory loaded!", OK, bError);

		if (!CHiveFactory.PhysicFactory)
			PrintError("Failed to get physics factory.", ERROR, bError);
		else
			PrintError("Physics factory loaded!", OK, bError);

		if (!CHiveFactory.StudioFactory)
			PrintError("Failed to get studio factory.", ERROR, bError);
		else
			PrintError("Studio factory loaded!", OK, bError);

		if (!CHiveFactory.ServerFactory)
			PrintError("Failed to get server factory.", ERROR, bError);
		else
			PrintError("Server factory loaded!", OK, bError);

		if (!CHiveFactory.VGUIFactory)
			PrintError("Failed to get VGUI factory.", ERROR, bError);
		else
			PrintError("VGUI factory loaded!", OK, bError);

		if (!CHiveFactory.VGUI2Factory)
			PrintError("Failed to get VGUI2 factory.", ERROR, bError);
		else
			PrintError("VGUI2 factory loaded!", OK, bError);

		if(!CHiveFactory.VSTD)
			PrintError("Failed to get VSTD factory.", ERROR, bError);
		else
			PrintError("VSTD factory loaded!", OK, bError);

			return bError;
	}

	bool interfaceErrors() {
		bool bError = false;

		if(!CHiveInterface.Client)
			PrintError("Failed to get client interface.", ERROR, bError);
		else
			PrintError("Client interface loaded!", OK, bError);

		if (!CHiveInterface.ClientMode)
			PrintError("Failed to get client mode interface.", ERROR, bError);
		else
			PrintError("Client mode interface loaded!", OK, bError);

		if (!CHiveInterface.Cvar)
			PrintError("Failed to get cvar interface.", ERROR, bError);
		else
			PrintError("Cvar interface loaded!", OK, bError);

		if (!CHiveInterface.Engine)
			PrintError("Failed to get Engine interface.", ERROR, bError);
		else
			PrintError("Engine interface loaded!", OK, bError);

		if (!CHiveInterface.EngineTrace)
			PrintError("Failed to get engine trace interface.", ERROR, bError);
		else
			PrintError("Engine trace interface loaded!", OK, bError);

		if (!CHiveInterface.EntityList)
			PrintError("Failed to get entity list interface.", ERROR, bError);
		else
			PrintError("Entity list interface loaded!", OK, bError);

		if (!CHiveInterface.GameConsole)
			PrintError("Failed to get game console interface.", ERROR, bError);
		else
			PrintError("Game console interface loaded!", OK, bError);

		if (!CHiveInterface.Globals)
			PrintError("Failed to get global vars interface.", ERROR, bError);
		else
			PrintError("Global vars interface loaded!", OK, bError);

		if (!CHiveInterface.Input)
			PrintError("Failed to get input interface.", ERROR, bError);
		else
			PrintError("Input interface loaded!", OK, bError);

		if (!CHiveInterface.IPanel)
			PrintError("Failed to get IPanel interface.", ERROR, bError);
		else
			PrintError("IPanel interface loaded!", OK, bError);

		if (!CHiveInterface.ISurface)
			PrintError("Failed to get ISurface interface.", ERROR, bError);
		else
			PrintError("ISurface interface loaded!", OK, bError);

		if (!CHiveInterface.LuaShared)
			PrintError("Failed to get LUA interface.", ERROR, bError);
		else
			PrintError("LUA interface loaded!", OK, bError);

		if (!CHiveInterface.MaterialSurface)
			PrintError("Failed to get material surface interface.", ERROR, bError);
		else
			PrintError("Material surface interface loaded!", OK, bError);

		if (!CHiveInterface.MaterialSystem)
			PrintError("Failed to get material system interface.", ERROR, bError);
		else
			PrintError("Material system interface loaded!", OK, bError);

		if (!CHiveInterface.Render)
			PrintError("Failed to get render interface.", ERROR, bError);
		else
			PrintError("Render system interface loaded!", OK, bError);
		
		return bError;
	}
	bool hookErrors() {
		bool bError = false;
		
		if(!HiveOriginalFunctions::CloseLuaInterface)
			PrintError("Failed to hook \"CloseLuaInterface\" function.", ERROR, bError);
		else
			PrintError("\"CloseLuaInterface\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::CreateLuaInterface)
			PrintError("Failed to hook \"CreateLuaInterface\" function.", ERROR, bError);
		else
			PrintError("\"CreateLuaInterface\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::CreateMove)
			PrintError("Failed to hook \"CreateMove\" function.", ERROR, bError);
		else
			PrintError("\"CreateMove\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::PaintTraverse)
			PrintError("Failed to hook \"PaintTraverse\" function.", ERROR, bError);
		else
			PrintError("\"PaintTraverse\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::CalcView)
			PrintError("Failed to hook \"CalcView\" function.", ERROR, bError);
		else
			PrintError("\"CalcView\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::FireBullets)
			PrintError("Failed to hook \"FireBullets\" function.", ERROR, bError);
		else
			PrintError("\"FireBullets\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::DrawModelExecute)
			PrintError("Failed to hook \"DrawModelExecute\" function.", ERROR, bError);
		else
			PrintError("\"DrawModelExecute\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::RenderCapture)
			PrintError("Failed to hook \"RenderCapture\" function.", ERROR, bError);
		else
			PrintError("\"RenderCapture\" hook loaded!", OK, bError);

		if (!HiveOriginalFunctions::SetupBones)
			PrintError("Failed to hook \"SetupBones\" function.", ERROR, bError);
		else
			PrintError("\"SetupBones\" hook loaded!", OK, bError);

		return bError;
	}
	
	bool netvarErrors() {
		bool bError = false;

		if(!HiveNetVarOffsets::m_Collision)
			PrintError("Failed to find \"m_Collision\" netvar.", ERROR, bError);
		else
			PrintError("\"m_Collision\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_Collision);
		if (!HiveNetVarOffsets::m_fFlags)
			PrintError("Failed to find \"m_fFlags\" netvar.", ERROR, bError);
		else
			PrintError("\"m_fFlags\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_fFlags);
		if (!HiveNetVarOffsets::m_flFriction)
			PrintError("Failed to find \"m_flFriction\" netvar.", ERROR, bError);
		else
			PrintError("\"m_flFriction\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_flFriction);
		if (!HiveNetVarOffsets::m_flMaxSpeed)
			PrintError("Failed to find \"m_flMaxSpeed\" netvar.", ERROR, bError);
		else
			PrintError("\"m_flMaxSpeed\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_flMaxSpeed);
		if (!HiveNetVarOffsets::m_flSimulationTime)
			PrintError("Failed to find \"m_flSimulationTime\" netvar.", ERROR, bError);
		else
			PrintError("\"m_flSimulationTime\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_flSimulationTime);
		if (!HiveNetVarOffsets::m_hActiveWeapon)
			PrintError("Failed to find \"m_hActiveWeapon\" netvar.", ERROR, bError);
		else
			PrintError("\"m_hActiveWeapon\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_hActiveWeapon);
		if (!HiveNetVarOffsets::m_hGroundEntity)
			PrintError("Failed to find \"m_hGroundEntity\" netvar.", ERROR, bError);
		else
			PrintError("\"m_hGroundEntity\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_hGroundEntity);
		if (!HiveNetVarOffsets::m_iHealth)
			PrintError("Failed to find \"m_iHealth\" netvar.", ERROR, bError);
		else
			PrintError("\"m_iHealth\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_iHealth);
		if (!HiveNetVarOffsets::m_iArmor)
			PrintError("Failed to find \"m_iArmor\" netvar.", ERROR, bError);
		else
			PrintError("\"m_iArmor\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_iArmor);
		if (!HiveNetVarOffsets::m_iTeamNum)
			PrintError("Failed to find \"m_iTeamNum\" netvar.", ERROR, bError);
		else
			PrintError("\"m_iTeamNum\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_iTeamNum);
		if (!HiveNetVarOffsets::m_lifeState)
			PrintError("Failed to find \"m_lifeState\" netvar.", ERROR, bError);
		else
			PrintError("\"m_lifeState\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_lifeState);
		if (!HiveNetVarOffsets::m_nTickBase)
			PrintError("Failed to find \"m_nTickBase\" netvar.", ERROR, bError);
		else
			PrintError("\"m_nTickBase\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_nTickBase);
		if (!HiveNetVarOffsets::m_vecMaxs)
			PrintError("Failed to find \"m_vecMaxs\" netvar.", ERROR, bError);
		else
			PrintError("\"m_vecMaxs\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_vecMaxs);
		if (!HiveNetVarOffsets::m_vecMins)
			PrintError("Failed to find \"m_vecMins\" netvar.", ERROR, bError);
		else
			PrintError("\"m_vecMins\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_vecMins);
		if (!HiveNetVarOffsets::m_vecPunchAngle)
			PrintError("Failed to find \"m_vecPunchAngle\" netvar.", ERROR, bError);
		else
			PrintError("\"m_vecPunchAngle\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_vecPunchAngle);
		if (!HiveNetVarOffsets::m_vecPunchAngleVel)
			PrintError("Failed to find \"m_vecPunchAngleVel\" netvar.", ERROR, bError);
		else
			PrintError("\"m_vecPunchAngleVel\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_vecPunchAngleVel);
		if (!HiveNetVarOffsets::m_vecVelocity)
			PrintError("Failed to find \"m_vecVelocity\" netvar.", ERROR, bError);
		else
			PrintError("\"m_vecVelocity\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_vecVelocity);
		if (!HiveNetVarOffsets::m_angEyeAngles)
			PrintError("Failed to find \"m_angEyeAngles\" netvar.", ERROR, bError);
		else
			PrintError("\"m_angEyeAngles\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_angEyeAngles);
		if (!HiveNetVarOffsets::m_iObserverMode)
			PrintError("Failed to find \"m_iObserverMode\" netvar.", ERROR, bError);
		else
			PrintError("\"m_iObserverMode\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_iObserverMode);
		if (!HiveNetVarOffsets::m_hObserverTarget)
			PrintError("Failed to find \"m_hObserverTarget\" netvar.", ERROR, bError);
		else
			PrintError("\"m_hObserverTarget\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_hObserverTarget);
		if (!HiveNetVarOffsets::m_iPrimaryAmmoType)
			PrintError("Failed to find \"m_iPrimaryAmmoType\" netvar.", ERROR, bError);
		else
			PrintError("\"m_iPrimaryAmmoType\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_iPrimaryAmmoType);
		if (!HiveNetVarOffsets::m_flNextPrimaryAttack)
			PrintError("Failed to find \"m_flNextPrimaryAttack\" netvar.", ERROR, bError);
		else
			PrintError("\"m_flNextPrimaryAttack\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_flNextPrimaryAttack);
		/*
		if (!HiveNetVarOffsets::m_bDormant)
			PrintError("Failed to find \"m_bDormant\" netvar.", ERROR, bError);
		else
			PrintError("\"m_bDormant\" netvar found!", OK, bError);
		ConMsg("[0x%.8X]\n", HiveNetVarOffsets::m_bDormant);
		*/
		return bError;
	}

	bool nativeErrors() {
		bool bError = false;
		if (!CHiveSourceNative.GetPlayerActiveWeapon)
			PrintError("Source Engine -> *\"GetActiveWeapon\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"GetActiveWeapon\" has been found!", OK, bError);
		
		if (!CHiveSourceNative.GetBonePosition)
			PrintError("Source Engine -> *\"GetBonePosition\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"GetBonePosition\" has been found!", OK, bError);
		
		if (!CHiveSourceNative.GetObserverTarget)
			PrintError("Source Engine -> *\"GetObserverTarget\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"GetObserverTarget\" has been found!", OK, bError);

		if (!CHiveSourceNative.GetPlayerActiveWeapon)
			PrintError("Source Engine -> *\"GetPlayerActiveWeapon\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"GetPlayerActiveWeapon\" has been found!", OK, bError);

		if (!CHiveSourceNative.LookUpBone)
			PrintError("Source Engine -> *\"LookUpBone\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"LookUpBone\" has been found!", OK, bError);

		if (!CHiveSourceNative.MD5_PseudoRandom)
			PrintError("Source Engine -> *\"MD5_PseudoRandom\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"MD5_PseudoRandom\" has been found!", OK, bError);

		if (!CHiveSourceNative.UTIL_GetPlayerByIndex)
			PrintError("Source Engine -> *\"UTIL_GetPlayerByIndex\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"UTIL_GetPlayerByIndex\" has been found!", OK, bError);

		if (!CHiveSourceNative.offset_bSendPacket)
			PrintError("Source Engine -> *\"bSendPacket\" has not been found!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"bSendPacket\" has been found!", OK, bError);

	//	ConMsg("[0x%.8X]\n", (DWORD)CHiveSourceNative.offset_bSendPacket);

		if (!VirtualProtect((LPVOID)(CHiveSourceNative.offset_bSendPacket - 100), 1024, PAGE_EXECUTE_READWRITE, &dummy))
			PrintError("Source Engine -> *\"bSendPacket\" Page guard NOT patched!", ERROR, bError);
		else
			PrintError("Source Engine -> *\"bSendPacket\" Page guard patched!", OK, bError);
		
		return bError;
	}

	bool protectorErrors() {
		bool bError = false;
		if(!HiveProtection::orgCreateFileA)
			PrintError("Kernel32 -> CreateFileA is unprotected!", ERROR, bError);
		else
			PrintError("Kernel32 -> CreateFileA is protected.", OK, bError);

		if (!HiveProtection::orgCreateFileW)
			PrintError("Kernel32 -> CreateFileW is unprotected!", ERROR, bError);
		else
			PrintError("Kernel32 -> CreateFileW is protected.", OK, bError);
		return bError;
	}


}