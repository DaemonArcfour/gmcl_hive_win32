#include "hive_hookfunctions.h"
#include "hive_cheats.h"
#include "hive_math.h"
#include "hive_lua_whitelist.h"



void* clientState = 0;
VPANEL GlobalPanel = 0;
int MaxClients = 0;
FireBullets_info BulletInfo;
bool LuaInit = false;

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
		MENU->Call(5, 0);
		MENU->Pop(2);
		bool whitelisted = isWhitelisted(ccString);
		if (!CLuaMenuCallback.LuaExecution && !CLuaMenuCallback.CustomLuaRun && !whitelisted)
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
		MENU->Call(5, 0);
		MENU->Pop(2);
		bool whitelisted = isWhitelisted(ccString);
		if (!CLuaMenuCallback.LuaExecution && !CLuaMenuCallback.CustomLuaRun && !whitelisted)
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

	void __fastcall CreateMove(void* CInput, void *edx, int sequence_number, float input_sample_frametime, bool active) {
		HiveOriginalFunctions::CreateMove(CInput, edx, sequence_number, input_sample_frametime, active);
		static Vector Spread = Vector(0, 0, 0);
		GMODCUserCmd *pCmd = (GMODCUserCmd*)CHiveInterface.Input->GetUserCmd(sequence_number);
		CBaseEntityNew * LocalPlayer = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		C_BasePlayerNew* pLocal = (C_BasePlayerNew*)LocalPlayer;
		CHiveOptimize.IsConnected = CHiveInterface.Engine->IsConnected();
		CHiveOptimize.InGame = CHiveInterface.Engine->IsInGame();

		if (CHiveOptimize.InGame && CHiveOptimize.IsConnected) {
			float fOldSidemove = pCmd->sidemove;
			float fOldForward = pCmd->forwardmove;
			QAngle vOldAngles = pCmd->viewangles;
			bool CanShoot = true;
			C_BaseCombatWeaponNew* currentWeapon = (C_BaseCombatWeaponNew*)pLocal->GetActiveWeapon();
			
			if (BulletInfo.m_vecSpread != Vector(0, 0, 0))
				Spread = BulletInfo.m_vecSpread;

			if (CLuaMenuCallback.Bhop)
				HiveCheats::Bunnyhop(pCmd, LocalPlayer);

			if (CLuaMenuCallback.Autostrafe)
				HiveCheats::Autostrafe(pCmd);

			if(CLuaMenuCallback.EnginePredict)
				CHiveEnginePredict.Start(pCmd, pLocal);

			if (CLuaMenuCallback.Aimbot && pCmd->buttons & IN_ATTACK && CanShoot)
			{
				HiveCheats::Aimbot(pCmd, LocalPlayer);
			}

			if (CLuaMenuCallback.Triggerbot && CanShoot)
				HiveCheats::Triggerbot(pCmd->viewangles, pCmd, LocalPlayer);

			if (pCmd->buttons & IN_ATTACK && LocalPlayer->isAlive() && CLuaMenuCallback.NoSpread && CanShoot) 
			{
				HiveCheats::RemoveSpread(pCmd, currentWeapon, Spread);
			}
			if (CLuaMenuCallback.EnginePredict)
				CHiveEnginePredict.Finish(pLocal);

			if(CLuaMenuCallback.Aimbot)
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
		//	HiveCheats::DrawObservers();
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
		if (CLuaMenuCallback.NoRecoil) {
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
		QAngle FixRecoil(0,0,0);
		CHiveInterface.Engine->GetViewAngles(FixRecoil);
		void *ret = HiveOriginalFunctions::SwepPrimaryAttack(ecx, edx);
		if(CLuaMenuCallback.NoRecoil)
			CHiveInterface.Engine->SetViewAngles(FixRecoil);
		return ret;
	}

	bool   __fastcall SetupBones(void* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		C_BasePlayerNew* ply = reinterpret_cast<C_BasePlayerNew*>((C_BasePlayerNew*)ecx - 8); // Is this even right
		CBaseEntityNew* ent = (CBaseEntityNew*)ecx;
		C_BasePlayerNew* localPlayer = reinterpret_cast<C_BasePlayerNew*>(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer()));

		if(!CLuaMenuCallback.EnginePredict)
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
}
