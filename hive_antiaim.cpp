#include "hive_cheats.h"
#include "hive_math.h"
#include "hive_util.h"



namespace HiveCheats {
	CBaseEntityNew* pBaseEntity;
	CBaseEntityNew* Target;

	float	flBestDist;
	int		m_nTarget;

	float GetFov(QAngle angle, Vector src, Vector dst)
	{
		QAngle ang, aim;
		float fov;

		HiveMath::CalculateAngle(src, dst, ang);
		HiveMath::MakeVector(angle, aim);
		HiveMath::MakeVector(ang, ang);

		float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
		float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));

		float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

		fov = acos(u_dot_v / (mag_s * mag_d)) * (360.0 / M_PI);

		return fov;
	}

	void EntBestTarget(CBaseEntityNew* me, CBaseEntityNew* pBaseEntity, int index) {
		QAngle AbsAng = QAngle(me->GetAbsAngles().x, me->GetAbsAngles().y, me->GetAbsAngles().z);
		if (GetFov(AbsAng, me->GetEyePosition(), pBaseEntity->GetEyePosition()) <= 360
			&& GetFov(AbsAng, me->GetEyePosition(), pBaseEntity->GetEyePosition()) < flBestDist)
		{

			flBestDist = GetFov(AbsAng, me->GetEyePosition(), pBaseEntity->GetEyePosition());
			m_nTarget = index;
		}
	}

	QAngle GetEnemyPos(CBaseEntityNew* me, CBaseEntityNew* pBaseEntity) {
		QAngle buffer;
		VectorAngles((pBaseEntity->GetOrigin() - me->GetEyePosition()), buffer);
		return buffer;
		//return (isValid(aaply) and (veloPredict(aaply:GetPos() or Vector(), aaply) - me:EyePos()) : Angle()) or silentAngle
	}

	void Antiaim(GMODCUserCmd* cmd, CBaseEntityNew* LocalPlayer)
	{
		if (!cmd->command_number)
			return;

		bool AAchoke = false;
		bool InvertBool = false;
		float EnemyYaw = 0.0f;
		int iAntiaim_Type = CLuaMenuCallback.AntiaimType;

		float OldYaw = 0.0f;


		switch (iAntiaim_Type)
		{
			case 0:
				cmd->viewangles.x = -180.000005f;
				break;

			case 1:
				cmd->viewangles.x = 180.0000076294f;
				break;

			case 2:
				cmd->viewangles.x = 180.00000762939f;
				break;
			
			case 3:
				cmd->viewangles.x = -612.3f + (- 7 + (rand() % 14)); //magic angle -180
				break;
			
			case 4: //-180 shit with magic angle
				if (bSendPacket) {
					cmd->viewangles.x = (AAchoke ? 170 : 190) + (1 + (rand() % 13)) + (0.3 + ((rand() % 8) / 100.0f));
					cmd->viewangles.y = AAchoke ? -612.3 + (0.2 + ((rand() % 9) / 100.0f)) : -612.2 + (0.2 + ((rand() % 9) / 100.0f));
				}
				else {//RealAngles look away from enemy move vel
					//cmd->viewangles.x = (DH.InvertBool ? ey + 90 : ey - 90) + (1 + (rand() % 13) + mRand(0.03, 0.08);
					cmd->viewangles.x = (AAchoke ? 90 : 90) + (1 + (rand() % 13)) + (0.3 + ((rand() % 8) / 100.0f));
					cmd->viewangles.y = AAchoke ? -612.3 + (0.2 + ((rand() % 9) / 100.0f)) : -612.2 + (0.2 + ((rand() % 9) / 100.0f));
				}
				AAchoke = !AAchoke;
				break;

			case 5: //backwards at enemy
				for (int index = CHiveInterface.Engine->GetMaxClients(); index >= 0; --index)
				{
					if (index == CHiveInterface.Engine->GetLocalPlayer())
						continue;

					pBaseEntity = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(index);
					if (!pBaseEntity || !pBaseEntity->isAlive() || NativeClass::IsDormant(pBaseEntity))
						continue;

					if (HiveUTIL::IsFriend((C_BasePlayerNew*)pBaseEntity))
						continue;

					Target = pBaseEntity;
					EntBestTarget(LocalPlayer, pBaseEntity, index);
					EnemyYaw = GetEnemyPos(LocalPlayer, Target).y;
					cmd->viewangles.y = EnemyYaw - 180.0f;
				}
				break;

			case 6: //Invert (Best AA)
				for (int index = CHiveInterface.Engine->GetMaxClients(); index >= 0; --index)
				{
					if (index == CHiveInterface.Engine->GetLocalPlayer())
						continue;

					pBaseEntity = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(index);
					if (!pBaseEntity || !pBaseEntity->isAlive() || NativeClass::IsDormant(pBaseEntity))
						continue;

					if (HiveUTIL::IsFriend((C_BasePlayerNew*)pBaseEntity))
						continue;

					Target = pBaseEntity;
					EntBestTarget(LocalPlayer, pBaseEntity, index);
					EnemyYaw = GetEnemyPos(LocalPlayer, Target).y;

					if (bSendPacket) {
						cmd->viewangles.x = EnemyYaw - (AAchoke ? 170 : 190) + (1 + (rand() % 13)) + (0.3 + ((rand() % 8) / 100.0f));
						cmd->viewangles.y = AAchoke ? -612.3 + (0.2 + ((rand() % 9) / 100.0f)) : -612.2 + (0.2 + ((rand() % 9) / 100.0f));
					}
					else {//RealAngles look away from enemy move vel
						cmd->viewangles.x = (InvertBool ? EnemyYaw + 90 : EnemyYaw - 90) + (1 + (rand() % 13)) + (0.3 + ((rand() % 8) / 100.0f));
						cmd->viewangles.y = AAchoke ? -612.3 + (0.2 + ((rand() % 9) / 100.0f)) : -612.2 + (0.2 + ((rand() % 9) / 100.0f));
					}
					AAchoke = !AAchoke;

					InvertBool = OldYaw > EnemyYaw ? true : false;
					OldYaw = EnemyYaw;
				}
				break;

			default:
				break;
		}
	}

	void CorrectMovement(QAngle vOldAngles, GMODCUserCmd* pCmd, float fOldForward, float fOldSidemove) {
		QAngle NormalizedViewAngles = pCmd->viewangles;
		NormalizedViewAngles.z = 1;
		if (NormalizedViewAngles.x > 89.f || NormalizedViewAngles.x < -89.f)
		{
			NormalizedViewAngles.y += 180;
			NormalizedViewAngles.z = -1;
		}

		HiveMath::NormalizeAngles(NormalizedViewAngles);
		
		float deltaView = NormalizedViewAngles.y - vOldAngles.y;
		float f1;
		float f2;

		if (pCmd->viewangles == vOldAngles)
			return;

		if (vOldAngles.y < 0.f)
			f1 = 360.0f + vOldAngles.y;
		else
			f1 = vOldAngles.y;

		if (NormalizedViewAngles.y < 0.0f)
			f2 = 360.0f + NormalizedViewAngles.y;
		else
			f2 = NormalizedViewAngles.y;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);
		deltaView = 360.0f - deltaView;

		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = (sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove) * NormalizedViewAngles.z;
	}
}