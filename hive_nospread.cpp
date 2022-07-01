#include "hive_cheats.h"
#include "hive_math.h"
const char* GetLuaEntBase(C_BaseCombatWeaponNew* _this)
{
	ILuaInterface* Lua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
	if (!_this->UsesLua())
		return "";
	_this->PushEntity();
	Lua->GetField(-1, "Base");
	const char* out = "";
	if (Lua->IsType(-1, GarrysMod::Lua::Type::STRING))
		out = Lua->GetString(-1, NULL);
	Lua->Pop(2);
	return out;
}

double LuaCurTime()
{
	ILuaInterface* Lua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
	if (!Lua) return 0.;
	Lua->PushSpecial(0); // SPECIAL_GLOB
	Lua->GetField(-1, "CurTime");
	Lua->Call(0, 1);
	double curTime = Lua->GetNumber(-1);
	Lua->Pop(2);
	return curTime;
}

double LuaMathRand(double min, double max) {
	auto Lua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
	if (!Lua) return 0.;
	Lua->PushSpecial(0); // SPECIAL_GLOB
	Lua->GetField(-1, "math");
	Lua->GetField(-1, "Rand");
	Lua->PushNumber(min);
	Lua->PushNumber(max);
	Lua->Call(2, 1);
	double retVal = (double)Lua->GetNumber(-1);
	Lua->Pop(3);
	return retVal;
}

void LuaMathSetSeed(double seed) {
	auto Lua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
	if (!Lua) return;
	Lua->PushSpecial(0); // SPECIAL_GLOB
	Lua->GetField(-1, "math");
	Lua->GetField(-1, "randomseed");
	Lua->PushNumber(seed);
	Lua->Call(1, 0);
	Lua->Pop(2);
}

namespace HiveCheats {
	bool CheckFire() {
		const char* weapon = CHiveSourceNative.GetClassName(CHiveSourceNative.GetPlayerActiveWeapon(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer())));
		if ((strcmp(weapon, "weapon_physgun") == 0) || (strcmp(weapon, "weapon_physcannon") == 0) || (strcmp(weapon, "gmod_tool") == 0))
		{
			aimbot_target = -1;
			return false;
		}
		return true;
	}

	void RemoveRecoil(QAngle &viewangle, CBaseEntityNew* LocalPlayer)
	{
		QAngle p = LocalPlayer->GetPunchAngles();
		CHiveInterface.Engine->GetViewAngles(viewangle);
		float normal = HiveMath::NormalizeAngleLength(p);
		normal = max(normal - (10.f + normal * 0.5f) * CHiveInterface.Globals->interval_per_tick, 0.f);
		p *= normal;
		p.z = 0.f;
		//cmd->viewangles -= p;
		
		CHiveInterface.Engine->SetViewAngles(viewangle - p);
	}

	void RemoveSpread(GMODCUserCmd *pCmd, C_BaseCombatWeaponNew* gun, Vector Spread) {
		ILuaInterface* ClientLua = CHiveInterface.LuaShared->GetLuaInterface(LUA_CLIENT);
		RandomSeed(pCmd->random_seed & 255);
		C_BasePlayerNew* localPlayer = (C_BasePlayerNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
		double spread = FLT_MAX;
		if (!gun->UsesLua())
		{
			const char* rev = CHiveSourceNative.GetClassName(CHiveSourceNative.GetPlayerActiveWeapon(CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer())));
			if (strcmp(rev, "weapon_357") == 0)
				return;
			Vector gunSpread = gun->GetBulletSpread();
			spread = (gunSpread.x + gunSpread.y + gunSpread.z) / 3.0f;
		}

		else
		{
			int topop = 3;
			gun->PushEntity();

			if (!strcmp(GetLuaEntBase(gun), "tfa_gun_base"))
			{
				--topop;
				ClientLua->GetField(-1, "CalculateConeRecoil");
				ClientLua->Push(-2);
				ClientLua->Call(1, 1);

				spread = ClientLua->GetNumber(-1);
			}

			else if (!strcmp(GetLuaEntBase(gun), "cw_base"))
			{
				
				ClientLua->GetField(-1, "MaxSpreadInc");
				double curCone = 0.09f + ClientLua->GetNumber(-1);
				ClientLua->Pop(2);
				// 	self.CurCone = math.Clamp(cone + self.AddSpread * (self.dt.Bipod and 0.5 or 1) + (vel / 10000 * self.VelocitySensitivity) * (self.dt.Status == FAS_STAT_ADS and 0.25 or 1) + self.Owner.ViewAff, 0, 0.09 + self.MaxSpreadInc)

				//if self.Owner:Crouching() then cone = cone * 0.85 end
				if (pCmd->buttons & IN_DUCK)
					curCone *= 0.85f;

				// 	math.randomseed(commandNumber)
				QAngle spreadAng;
				LuaMathSetSeed((double)pCmd->command_number);
				spreadAng.x = LuaMathRand(-curCone, curCone);
				spreadAng.y = LuaMathRand(-curCone, curCone);

				spreadAng.z = 0;

				pCmd->viewangles -= (spreadAng * 25.f);
				pCmd->viewangles -= localPlayer->GetViewPunch();
				
				
				// Basically, reversing the previous usercmd's angles will simply max out the Recoil
				if (pCmd->command_number)
				{
					GMODCUserCmd* prevcmd = pCmd;
					for (int i = 0; i < 3; i++)
					{
						GMODCUserCmd* cmd = (GMODCUserCmd*)CHiveInterface.Input->GetUserCmd(prevcmd->command_number - 1);
						cmd->viewangles.y = prevcmd->viewangles.y - 180.f;
						cmd->viewangles.x = -prevcmd->viewangles.x;
						cmd->viewangles.FixAngles();

						CHiveInterface.Input->m_pVerifiedCommands[cmd->command_number % 90].m_cmd = *cmd;
						CHiveInterface.Input->m_pVerifiedCommands[cmd->command_number % 90].m_crc = cmd->GetChecksum();
						

						prevcmd = cmd;
					}
				}
				return;

			}

			else if (!strcmp(GetLuaEntBase(gun), "fas2_base"))
			{
				ClientLua->GetField(-1, "MaxSpreadInc");
				double curCone = 0.09f + ClientLua->GetNumber(-1);
				ClientLua->Pop(2);
				// 	self.CurCone = math.Clamp(cone + self.AddSpread * (self.dt.Bipod and 0.5 or 1) + (vel / 10000 * self.VelocitySensitivity) * (self.dt.Status == FAS_STAT_ADS and 0.25 or 1) + self.Owner.ViewAff, 0, 0.09 + self.MaxSpreadInc)

				QAngle spreadAng;

				LuaMathSetSeed(LuaCurTime());
				spreadAng.x = LuaMathRand(-curCone, curCone);
				spreadAng.y = LuaMathRand(-curCone, curCone);
				spreadAng.z = 0;

				pCmd->viewangles -= (spreadAng * 25.f);
				pCmd->viewangles -= localPlayer->GetViewPunch();

				if (pCmd->command_number)
				{
					GMODCUserCmd* prevCmd = (GMODCUserCmd*)CHiveInterface.Input->GetUserCmd(pCmd->command_number - 1);
					prevCmd->viewangles.x = -pCmd->viewangles.x;
					prevCmd->viewangles.y = pCmd->viewangles.y - 180.f;
					prevCmd->viewangles.FixAngles();

					CHiveInterface.Input->m_pVerifiedCommands[prevCmd->command_number % 90].m_cmd = *prevCmd;
					CHiveInterface.Input->m_pVerifiedCommands[prevCmd->command_number % 90].m_crc = prevCmd->GetChecksum();
				}
				return;
			}

			else
			{
				ClientLua->GetField(-1, "Primary");
				if (!ClientLua->IsType(-1, GarrysMod::Lua::Type::TABLE))
				{
					--topop;
					ClientLua->Pop(1);
				}
				ClientLua->GetField(-1, "Spread");
				if (ClientLua->IsType(-1, GarrysMod::Lua::Type::NUMBER))
				{
					spread = ClientLua->GetNumber(-1);
				}
			}
			ClientLua->Pop(topop);
		}

		if (spread == FLT_MAX)
		{
			return;
		}

		QAngle engineSpread(RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f), 
			RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f), 0);

		Vector ShootDirection = Vector(1.f, 1.f, 1.f);
		ShootDirection.y = (spread * engineSpread.y);
		ShootDirection.z = -(spread * engineSpread.x);
		pCmd->viewangles += ShootDirection.toAngle();
	}
	bool BreakLuaGameModeHooks = false;
}