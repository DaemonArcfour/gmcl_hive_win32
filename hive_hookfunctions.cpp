#include "hive_hookfunctions.h"
#include "hive_cheats.h"
#include "hive_math.h"
#include "hive_util.h"
#include "hive_lua_whitelist.h"



void* clientState = 0;
VPANEL GlobalPanel = 0;
int MaxClients = 0;
FireBullets_info BulletInfo;
QAngle OldPunchAngles = QAngle(0, 0, 0);
bool LuaInit = false;
int iHardBtEntity = 2;

using namespace GarrysMod::Lua;

namespace HiveHookedFunctions {
	void * __fastcall RunString(void *edx, void *ecx, const char* ccName, const char* ccPath, const char* ccString, bool bRun, bool bErrors) {
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->GetField(-1, "hook");
		MENU->GetField(-1, "Call");
		MENU->PushString("hiveRunString");
		MENU->PushNil();
		MENU->PushString(ccName);
		MENU->PushString(ccPath);
		MENU->PushString(ccString);
		MENU->PCall(5, 1, 0);

		bool allowed = true;
		if (MENU->IsType(-1, Type::BOOL)) {
			if (MENU->GetBool() == false) allowed = false;
		}

		if (MENU->IsType(-1, Type::STRING)) {
			ccString = MENU->GetString();
		}

		MENU->Pop(3);


		bool whitelisted = isWhitelisted(ccString);
		if ((!CLuaMenuCallback.LuaExecution && !CLuaMenuCallback.CustomLuaRun && !whitelisted) || !allowed)
			ccString = "print('Script stopped, lua execution disabled.')";
		CLuaMenuCallback.CustomLuaRun = false;
		return HiveOriginalFunctions::RunString(edx, ccName, ccPath, ccString, bRun, bErrors);
	}

	void * __fastcall RunStringEx(void *_this, void* edx, char const* ccName, char const* ccPath, char const* ccString, bool bRun, bool bErrors, bool UNK_1, bool UNK_2) {

		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->GetField(-1, "hook");
		MENU->GetField(-1, "Call");
		MENU->PushString("hiveRunStringEx");
		MENU->PushNil();
		MENU->PushString(ccName);
		MENU->PushString(ccPath);
		MENU->PushString(ccString);
		MENU->PCall(5, 1, 0);

		bool allowed = true;
		if (MENU->IsType(-1, Type::BOOL)) {
			if (MENU->GetBool() == false) allowed = false;
		}

		if (MENU->IsType(-1, Type::STRING)) {
			ccString = MENU->GetString();
		}

		MENU->Pop(3);
		bool whitelisted = isWhitelisted(ccString);
		if ((!CLuaMenuCallback.LuaExecution && !CLuaMenuCallback.CustomLuaRun && !whitelisted) || !allowed)
			ccString = "print('Script stopped, lua execution disabled.')";
		CLuaMenuCallback.CustomLuaRun = false;

		void *ret = HiveOriginalFunctions::RunStringEx(_this, ccName, ccPath, ccString, bRun, bErrors, UNK_1, UNK_2);
		if (LuaInit) {
			if (strcmp(ccName, "Startup") == 0) {
				ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
				if (ClientLua) {
					HiveLUA::GetFunctions(ClientLua, LUA_OriginalFunctions);
					LuaInit = false;
				}
			}
		}

		return ret;
	}

	void * __fastcall CompileString(void *_this, void* edx, void* UNK_1, char const& ccString) {
		ConMsg("Compile string called.\n");
		return HiveOriginalFunctions::CompileString(_this, UNK_1, ccString);
	}

	void *__fastcall CloseLuaInterface(void *_this, void *edx, void* luaInterface) {
		MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		MENU->GetField(-1, "hook");
		MENU->GetField(-1, "Call");
		MENU->PushString("HiveClientLuaDestroyed");
		MENU->PushNil();
		MENU->Call(2, 0);
		MENU->Pop(2);


		void *LuaState = HiveOriginalFunctions::CloseLuaInterface(_this, luaInterface);
		
		if (luaInterface == clientState) {
			HiveLUA::RecycleFunctionList(LUA_OriginalFunctions);
			clientState = 0;
		}

		HiveTroubleshooter::PrintInitizalization("Lua interface destroyed, initializing garbage collector...");
		HiveHook::CleanUpLuaHooks();

		return LuaState;
	}

	bool __fastcall CheatsGetBool(void* _this, void* edx)
	{
		auto ret = (DWORD)_ReturnAddress();

		//if (ret == *((DWORD*)CHiveInterface.Input + 31))
		//	return true;

		if (true) return true;

		return HiveOriginalFunctions::CheatsGetBool(_this, edx);
	}

	void * __fastcall CreateLuaInterface(void *_this, void *, uchar stateType, bool renew) {
		void *LuaState = HiveOriginalFunctions::CreateLuaInterface(_this, stateType, renew);
		MaxClients = CHiveInterface.Engine->GetMaxClients();
		if (stateType != 0)
			return LuaState;

		HiveTroubleshooter::PrintInitizalization("Lua interface created, initializing hooks...");
		clientState = LuaState;
		LuaInit = true;
		HiveHook::GetRunString(clientState);
		HiveHook::GetRunStringEx(clientState);
		HiveHook::GetCompileString(clientState);

		return clientState;
	}

	bool __fastcall ClientModeCreateMove(void* CClientModeShared, void* edx, float sample_input_frametime, GMODCUserCmd* pCmd) {
			// ClientMode CreateMove hook.
		return HiveOriginalFunctions::ClientModeCreateMove(CClientModeShared, sample_input_frametime, pCmd);
	}

	/*bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
	{
		//static DWORD dwCAM_Think = (DWORD)HiveOriginalFunctions::CAM_Think;
		//void* ptr = reinterpret_cast<DWORD*&>(CHiveInterface.Input->CAM_Think);
		
		static DWORD dwCAM_Think = *((DWORD*)CHiveInterface.Input + 0x230);

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == dwCAM_Think)
			return true;

		return false;
	}*/
	static QAngle RealAngle;
	static QAngle FakeAngle;
	static bool ThirdPerson = false;

	void __fastcall CreateMove(void* CInput, void *edx, int sequence_number, float input_sample_frametime, bool active) {
		HiveOriginalFunctions::CreateMove(CInput, edx, sequence_number, input_sample_frametime, active);
		HiveCheats::cBacktrackInterface.SetSequenceNumber(sequence_number);
		static Vector Spread = Vector(0, 0, 0);
		GMODCUserCmd *pCmd = (GMODCUserCmd*)CHiveInterface.Input->GetUserCmd(sequence_number);
		CBaseEntityNew * LocalPlayer = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		C_BasePlayerNew* pLocal = (C_BasePlayerNew*)LocalPlayer;
		CHiveOptimize.IsConnected = CHiveInterface.Engine->IsConnected();
		CHiveOptimize.InGame = CHiveInterface.Engine->IsInGame();

		if (CHiveOptimize.InGame && CHiveOptimize.IsConnected) {
			float fServerTime = LocalPlayer->GetTickBase() * CHiveInterface.Globals->interval_per_tick;
			float fOldSidemove = pCmd->sidemove;
			float fOldForward = pCmd->forwardmove;
			QAngle vOldAngles = pCmd->viewangles;
			bool CanShoot = true;
			C_BaseCombatWeaponNew* currentWeapon = (C_BaseCombatWeaponNew*)pLocal->GetActiveWeapon();

			if (pLocal->IsAlive() && currentWeapon)
				CanShoot = HiveCheats::CheckFire(currentWeapon);
			else
				CanShoot = false;

			if (CanShoot)
			{
				CanShoot = LocalPlayer->GetNextPrimaryAttack() <= fServerTime;

				if(!CanShoot)
					pCmd->buttons &= ~IN_ATTACK;
			}

			if (BulletInfo.m_vecSpread != Vector(0, 0, 0))
				Spread = BulletInfo.m_vecSpread;

			HiveCheats::FakeLag(pCmd);

			if (CLuaMenuCallback.Bhop)
			{
				HiveCheats::Bunnyhop(pCmd, LocalPlayer);
			}

			if (CLuaMenuCallback.Autostrafe)
				HiveCheats::Autostrafe(pCmd);

			if(CLuaMenuCallback.EnginePredict && (pCmd->buttons & IN_ATTACK) && LocalPlayer->isAlive() && CanShoot)
				CHiveEnginePredict.Start(pCmd, pLocal);

			//if (CLuaMenuCallback.Antiaim && pLocal->IsAlive() && currentWeapon)
			if (CLuaMenuCallback.Antiaim && pLocal->IsAlive() && !(pCmd->buttons & IN_ATTACK))
			{
				if (HiveCheats::CheckFire(currentWeapon))
					HiveCheats::Antiaim(pCmd, LocalPlayer);
			}

			if (CLuaMenuCallback.Aimbot && CanShoot)
			{
				HiveCheats::Aimbot(pCmd, LocalPlayer);
			}

			if (CLuaMenuCallback.Triggerbot && CanShoot)
				HiveCheats::Triggerbot(pCmd->viewangles, pCmd, LocalPlayer);

			if (pCmd->buttons & IN_ATTACK && LocalPlayer->isAlive() && CLuaMenuCallback.NoSpread && CanShoot) 
			{
				HiveCheats::RemoveSpread(pCmd, currentWeapon, Spread);
			}

			if (pCmd->buttons & IN_ATTACK && LocalPlayer->isAlive() && CLuaMenuCallback.NoRecoil && CanShoot && !currentWeapon->UsesLua())
			{
				if(!CLuaMenuCallback.PSilent)
					pCmd->viewangles -= OldPunchAngles;
			}

			if (CLuaMenuCallback.PSilent && CLuaMenuCallback.Aimbot && CanShoot)
			{
				pCmd->world_clicking = true;
				Vector tmp;
				AngleVectors(pCmd->viewangles, &tmp);
				pCmd->viewangles = vOldAngles;
				pCmd->world_click_direction = tmp;
			}

			
			//static CameraThirdData_t* CamData = new CameraThirdData_t;
			if (ThirdPerson || CHiveInterface.Input->CAM_IsThirdPerson()) {
				if (*HiveCheats::bSendPacket)
				{
					RealAngle = pCmd->viewangles;
				}
				else
				{
					FakeAngle = pCmd->viewangles;
				}
			}

			static CameraThirdData_t* CamData = new CameraThirdData_t;
			static CViewSetup* CamSetup = new CViewSetup;

			if (HiveUTIL::IsKeyPressed(CLuaMenuCallback.ThirdpersonKey, false)) { //ThirdPerson
				ThirdPerson = !ThirdPerson;
			}

			if (ThirdPerson) {
				Vector eye_pos = LocalPlayer->GetEyePosition();
				CTraceFilter filter;
				trace_t tr;
				Ray_t ray;
				filter.pSkip = LocalPlayer;

				Vector forward;

				QAngle angles = pCmd->viewangles;
				angles.z = 0.f;

				AngleVectors(angles, &forward);

				int range = 40;
				ray.Init(eye_pos, eye_pos - forward * range);
				//0x46004003
				CHiveInterface.EngineTrace->TraceRay(ray, MASK_SOLID, &filter, &tr);

				if (tr.fraction < 1.f)
					range *= tr.fraction;

				angles.z = 450.0f;//range;
				CHiveInterface.Input->m_fCameraInThirdPerson = true;

				
				//CamSetup->fovViewmodel = 130.0f;
				//CHiveInterface.ClientMode->OverrideView(CamSetup);

				CamData->m_flDist = 450.0f;//range;
				CamData->m_flYaw = angles.x;
				CamData->m_flPitch = angles.y;
				CamData->m_flLag = 0;

				//CHiveInterface.Input->m_pCameraThirdData = CamData;
				CHiveInterface.Input->m_fCameraDistanceMove = true;
				CHiveInterface.Input->m_angPreviousViewAngles = angles;
				CHiveInterface.Input->m_CameraIsOrthographic = false;
				CHiveInterface.Input->CAM_SetCameraThirdData(CamData, angles);
			}
			else {
				CHiveInterface.Input->m_fCameraInThirdPerson = false;
				//CHiveInterface.Input->m_pCameraThirdData = CamData;
			}

			if (CLuaMenuCallback.EnginePredict && (pCmd->buttons & IN_ATTACK) && LocalPlayer->isAlive() && CanShoot)
				CHiveEnginePredict.Finish(pLocal);

			if(CLuaMenuCallback.Aimbot || CLuaMenuCallback.Antiaim)
				HiveCheats::CorrectMovement(vOldAngles, pCmd, fOldForward, fOldSidemove);
		}
		/*
		  v5 = (_DWORD *)*((_DWORD *)this + 49);
		  v54 = *((_DWORD *)this + 50); <---- VerifiedCommandList
		  *((_DWORD *)this + 58) = a3;
		  v6 = 81 * ((signed int)a3 % 90);
		  v55 = (signed int)a3 % 90;
		*/
		
		auto VerifiedCommandList = *((DWORD*)CHiveInterface.Input + 50); // CInput::CreateMove (dylibs)
		CVerifiedUserCmd*  VerifiedCommand = (CVerifiedUserCmd*)(VerifiedCommandList + 328 * (sequence_number % 90));

		if (!VerifiedCommand)
			return;

		VerifiedCommand->m_cmd = *pCmd;
		CRC32_t *m_crc = ((CRC32_t*)VerifiedCommand + 81);
		*m_crc = CHiveSourceNative.GetChecksum(pCmd);
	}

	void __fastcall PaintTraverse(void* _this, void* edx, VPANEL Panel, bool frp, bool allwfr) {
		HiveOriginalFunctions::PaintTraverse(_this, edx, Panel, frp, allwfr);
		if (!GlobalPanel) {
			if (CHiveInterface.IPanel->GetName(Panel)) {
				if (strcmp(VGUI_PANEL, CHiveInterface.IPanel->GetName(Panel)) == 0) {
					GlobalPanel = Panel;
				}
			}
		}
		if (Panel == GlobalPanel) {
			PaintTraverseConfig.ecx = _this;
			PaintTraverseConfig.edx = edx;
			PaintTraverseConfig.panel = Panel;
			PaintTraverseConfig.frp = frp;
			PaintTraverseConfig.allwfr = allwfr;

			CHiveFonts.LoadFonts();
			HiveCheats::DrawEsp();
			HiveCheats::DrawObservers();

			MENU->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
			MENU->GetField(-1, "hook");
			MENU->GetField(-1, "Call");
			MENU->PushString("HivePaint");
			MENU->PushNil();
			MENU->Call(2, 0);
			MENU->Pop(2);
		}

	}

	void  __fastcall DrawModelExecute(void* ModelRender, void* edx, DrawModelState_t const& DrawModelState, ModelRenderInfo_t const& ModelRenderInfo, matrix3x4_t* pCustomBoneToWorld) {
		HiveOriginalFunctions::DrawModelExecuteHook->UnHook();
		HiveCheats::Chams((IVModelRender*)ModelRender, DrawModelState, ModelRenderInfo, pCustomBoneToWorld);
		HiveOriginalFunctions::DrawModelExecute(ModelRender, DrawModelState, ModelRenderInfo, pCustomBoneToWorld);
		CHiveInterface.ModelRender->ForcedMaterialOverride(0);
		HiveOriginalFunctions::DrawModelExecuteHook->ReHook();
		
	}

	void  *__fastcall CalcView(void* ecx, void* edx, Vector& vec, QAngle& ang, float& unk1, float& unk2, float& unk3) {
		
		if (CLuaMenuCallback.NoRecoil) 
		{
			Vector vecpunch = NET_VECTOR(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer()), HiveNetVarOffsets::m_vecPunchAngle);
			OldPunchAngles = QAngle(vecpunch.x, vecpunch.y, vecpunch.z);
			NET_VECTOR(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer()), HiveNetVarOffsets::m_vecPunchAngle) = { 0.0f, 0.0f, 0.0f };
			NET_VECTOR(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer()), HiveNetVarOffsets::m_vecPunchAngleVel) = { 0.0f, 0.0f, 0.0f };
		}
		
		return HiveOriginalFunctions::CalcView(ecx, vec, ang, unk1, unk2, unk3);
	}


	void __fastcall FireBullets(void* ecx, void* edx, void* FireBullets_t) {
		if(ecx == CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer()))
			BulletInfo = *(FireBullets_info*)FireBullets_t;
		return HiveOriginalFunctions::FireBullets(ecx, edx, FireBullets_t);
	}

	void __fastcall FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t stage) {
		/*
		if (CHiveOptimize.InGame && CHiveOptimize.IsConnected)
		{
			C_BasePlayerNew* LocalPlayer = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
			if (LocalPlayer && LocalPlayer->IsAlive())
			{
				if (CLuaMenuCallback.NoRecoil)
				{
					LocalPlayer->GetViewPunch() = QAngle(0, 0, 0); breaks decals and desyncs shots
				}
			}
			
		}
		*/

		if (stage == FRAME_RENDER_START)
		{
			if (CHiveOptimize.InGame && CHiveOptimize.IsConnected)
			{
				C_BasePlayerNew* LocalPlayer = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
				if (LocalPlayer && LocalPlayer->IsAlive() && CHiveInterface.Input->CAM_IsThirdPerson()) {
					if (*HiveCheats::bSendPacket) {
						LocalPlayer->SetLocalViewAngles(RealAngle);
					}
					else {
						LocalPlayer->SetLocalViewAngles(FakeAngle);
					}
				}
			}
		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			
		

		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
		{

		}

		if (stage == FRAME_NET_UPDATE_END)
		{
			if (CLuaMenuCallback.Backtrack)
			{
				HiveCheats::cBacktrackInterface.ProcessTick();
			}
		}
		HiveOriginalFunctions::FrameStageNotify(ecx, edx, stage);
	}

	void  *__fastcall RenderCapture(void* ecx, void* edx) {
		//PlaySoundW(L"HiveSounds\\screengrab.wav", NULL, SND_ASYNC);
		if (CLuaMenuCallback.AntiScreengrab) 
		{
			HiveTroubleshooter::PrintBlock("render.Capture -> ScreenGrab attempt blocked.");
			CViewSetup PlayerView;
			C_BasePlayerNew* LocalPlayer = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
			int w = 0, h = 0;
			HiveOriginalFunctions::DrawModelExecuteHook->UnHook();
			CHiveInterface.Engine->GetScreenSize(w, h);
			CHiveInterface.IPanel->SetVisible(PaintTraverseConfig.panel, false);
			CHiveInterface.MaterialSystem->GetRenderContext()->ClearBuffers(1, 1, 1);
			CHiveInterface.EngineTool->GetPlayerView(PlayerView, 0, 0, w, h);
			PlayerView.fovViewmodel = 54;
			PlayerView.fov = LocalPlayer->GetFOV();
			HiveCheats::BreakLuaGameModeHooks = true;
			CHiveInterface.EngineTool->RenderView(PlayerView, VIEW_CLEAR_FULL_TARGET, RENDERVIEW_DRAWVIEWMODEL | RENDERVIEW_DRAWHUD);
			HiveCheats::BreakLuaGameModeHooks = false;
			CHiveInterface.IPanel->SetVisible(PaintTraverseConfig.panel, true);
			void* rCap = HiveOriginalFunctions::RenderCapture(ecx, edx);
			HiveOriginalFunctions::DrawModelExecuteHook->ReHook();
			return rCap;
		}

		else 
		{
			void* rCap = HiveOriginalFunctions::RenderCapture(ecx, edx);
			return rCap;
		}
	}

	bool __fastcall LuaGameModeCallWithArgs(void* ecx, void* edx, int args)
	{
		if (CLuaMenuCallback.StopLuaGameModeHooks)
			return false;

		if (HiveCheats::BreakLuaGameModeHooks)
			return false;

		return HiveOriginalFunctions::LuaGameModeCallWithArgs(ecx, args);
	}

	void* __fastcall SwepPrimaryAttack(void* ecx, void *edx) {
		void* ret = HiveOriginalFunctions::SwepPrimaryAttack(ecx, edx);
		return ret;
	}

	bool __fastcall SetupBones(void* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		C_BasePlayerNew* ply = reinterpret_cast<C_BasePlayerNew*>((C_BasePlayerNew*)ecx - 8); // Is this even right
		CBaseEntityNew* ent = (CBaseEntityNew*)ecx;

		C_BasePlayerNew* localPlayer = reinterpret_cast<C_BasePlayerNew*>(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer()));
		if(!CLuaMenuCallback.EnginePredict || (CLuaMenuCallback.Backtrack /*&& !HiveCheats::cBacktrackInterface.bDisableBoneInterpolation*/))
			return  HiveOriginalFunctions::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

		float simulationTime = ply == localPlayer ? CHiveEnginePredict.GetServerTime() : ent->flSimulationTime();

		float oldCurtime = CHiveInterface.Globals->curtime;
		float oldFrametime = CHiveInterface.Globals->frametime;
		CHiveInterface.Globals->curtime = simulationTime;
		CHiveInterface.Globals->frametime = CHiveInterface.Globals->interval_per_tick;

		ent->m_iEFlags() |= static_cast<int>(EFlags::DIRTY_ABSVELOCITY);
		ent->GetEffects() |= static_cast<int>(EEffects::NOINTERP);

		bool ret = HiveOriginalFunctions::SetupBones(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, simulationTime);

		ent->GetEffects() &= ~static_cast<int>(EEffects::NOINTERP);
		ent->m_iEFlags() &= ~static_cast<int>(EFlags::DIRTY_ABSVELOCITY);

		CHiveInterface.Globals->curtime = oldCurtime;
		CHiveInterface.Globals->frametime = oldFrametime;

		return ret;
	}

	void __fastcall RunCommand(void* _this, void*, C_BasePlayerNew* player, GMODCUserCmd* ucmd, void* moveHelper)
	{
		if (CLuaMenuCallback.NoRecoil)
		{
			QAngle angle;
			
			CHiveInterface.Engine->GetViewAngles(angle);
			HiveOriginalFunctions::RunCommand(_this, player, ucmd, moveHelper);
			CHiveInterface.Engine->SetViewAngles(angle);
		}

		else
		{
			return HiveOriginalFunctions::RunCommand(_this, player, ucmd, moveHelper);
		}
	}

	bool __fastcall WriteUsercmdDeltaToBuffer(void* _this, void* ecx, bf_write* buf, int from, int to, int isnewcommand)
	{
		/*
		static int iResetTicks = 0;
		{
			if (bReset)
				iResetTicks++;

			if (bReset == true && iResetTicks == 3)
			{
				float m_flInterp = CHiveInterface.Cvar->FindVar("cl_interp")->GetFloat();
				char buf[32];
				sprintf_s(buf, "%f", m_flInterp);
				NativeClass::SendNetMsg("cl_interp", buf);
				bShooting = false;
				bReset = false;
				iResetTicks = 0;
			}

			if (bShooting && CLuaMenuCallback.Backtrack)
			{
				float m_flInterp = fCorrectInterp;
				char buf[32];
				sprintf_s(buf, "%f", m_flInterp);
				NativeClass::SendNetMsg("cl_interp", buf);
				bShooting = false;
				bReset = true;
			}
			

		}
		*/

		if (CLuaMenuCallback.Backtrack)
			HiveCheats::cBacktrackInterface.Interp_WriteUserCmdDeltaToBuffer();

		return HiveOriginalFunctions::WriteUsercmdDeltaToBuffer(_this, buf, from, to, isnewcommand);
	}
}


namespace HiveOriginalFunctions {
	CHook* RunStringHook = nullptr;
	CHook* RunStringExHook = nullptr;
	CHook* CompileStringHook = nullptr;
	CHook* CreateLuaInterfaceHook = nullptr;
	CHook* CloseLuaInterfaceHook = nullptr;
	CHook* ClientModeCreateMoveHook = nullptr;
	CHook* CreateMoveHook = nullptr;
	CHook* PaintTraverseHook = nullptr;
	CHook* DrawModelExecuteHook = nullptr;
	CHook* FireBulletsHook = nullptr;
	CHook* FrameStageNotifyHook = nullptr;
	CHook* SetupBonesHook = nullptr;
	CHook* RunCommandHook = nullptr;
	//hive_func_CAM_Think CAM_Think = nullptr;
	hive_func_WriteUsercmdDeltaToBuffer WriteUsercmdDeltaToBuffer = 0;
	hive_func_RunCommand RunCommand = 0;
	hive_func_CompileString CompileString = 0;
	hive_func_RunString RunString = 0;
	hive_func_RunStringEx RunStringEx = 0;
	hive_func_CreateLuaInterface CreateLuaInterface = 0;
	hive_func_CloseLuaInterface CloseLuaInterface = 0;
	hive_func_ClientModeCreateMove ClientModeCreateMove = 0;
	hive_func_PaintTraverse PaintTraverse = 0;
	hive_func_DrawModelExecute DrawModelExecute = 0;
	hive_func_CalcView CalcView = 0;
	hive_func_CreateMove CreateMove = 0;
	hive_func_FrameStageNotify FrameStageNotify = 0;
	hive_func_RenderCapture RenderCapture = 0;
	hive_func_SwepPrimaryAttack SwepPrimaryAttack = 0;
	hive_func_FireBullets FireBullets = 0;
	hive_func_SetupBones SetupBones = 0;
	hive_func_CLuaGameModeCallWithArgs LuaGameModeCallWithArgs;
	hive_func_CLuaGameModeCallFinish LuaGameModeCallFinish;
	hive_func_CheatsGetBool CheatsGetBool;
}
