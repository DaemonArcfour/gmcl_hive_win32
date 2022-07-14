#include "hive_draw.h"
#include "hive_native.h"
#include "hive_math.h"
#include "hive_cheats.h"

HiveFonts CHiveFonts;

class screensize
{
public:
	int iScreenWidth;
	int iScreenHeight;
};

void HiveFonts::LoadFonts() {
	static bool Loaded = false;
	if (!Loaded) {
		MenuFont = CHiveInterface.ISurface->CreateFont();
		CHiveInterface.ISurface->SetFontGlyphSet(MenuFont, HiveFont, 20, 500, 0, 0, FONTFLAG_ANTIALIAS);

		MediumFont = CHiveInterface.ISurface->CreateFont();
		CHiveInterface.ISurface->SetFontGlyphSet(MediumFont, HiveFont, 14, 500, 0, 0, NULL);//FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE | FONTFLAG_ADDITIVE);

		SmallFont = CHiveInterface.ISurface->CreateFont();
		CHiveInterface.ISurface->SetFontGlyphSet(SmallFont, HiveFont, 14, 500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);

		vguiSmallFont = CHiveInterface.MaterialSurface->CreateFont();
		CHiveInterface.MaterialSurface->SetFontGlyphSet(vguiSmallFont, HiveFont, 12, 300, 0, 0, 0x200);

		vguiMediumFont = CHiveInterface.MaterialSurface->CreateFont();
		CHiveInterface.MaterialSurface->SetFontGlyphSet(vguiMediumFont, HiveFont, 14, 300, 0, 0, 0x200);

		vguiMenuFont = CHiveInterface.MaterialSurface->CreateFont();
		CHiveInterface.MaterialSurface->SetFontGlyphSet(vguiMenuFont, HiveFont, 20, 300, 0, 0, 0x200);

		HiveDraw::InitializeFonts();
		Loaded = true;
	}
}

namespace HiveDraw {
		namespace Fonts
	{
		DWORD Default;
		DWORD Menu;
		DWORD MenuBold;
		DWORD ESP;
		DWORD MenuText;
		DWORD Icon;
		DWORD MenuTabs;
		DWORD Text;
		DWORD Slider;
		DWORD Clock;
		DWORD LBY;
		DWORD Tabs;
	};

	void DrawString(int x, int y, Color col, vgui::HFont Font, const char* text, ...)
	{
		CHiveInterface.ISurface->DrawSetTextPos(x, y);
		CHiveInterface.ISurface->DrawSetTextColor(col);
		CHiveInterface.ISurface->DrawSetTextFont(Font);
		va_list list;
		char buffer[1024];
		va_start(list, text);
		vsprintf(buffer, text, list);
		va_end(list);
		int wsize = MultiByteToWideChar(65001, 0, buffer, -1, 0, 0);
		LPWSTR wbuf = (LPWSTR)malloc(wsize * sizeof(WCHAR));
		MultiByteToWideChar(65001, 0, buffer, -1, wbuf, wsize);
		CHiveInterface.ISurface->DrawPrintText(wbuf, wcslen(wbuf));
		free(wbuf);
	}

	void DrawLine(int x, int y, int w, int h, Color c)
	{
		CHiveInterface.ISurface->DrawSetColor(c);
		int px[] = { x, w };
		int py[] = { y, h };
		CHiveInterface.ISurface->DrawPolyLine(px, py, 2);
	}

	void DrawLine(Vector pos1, Vector pos2, Color c)
	{
		Vector scr1 = Vector(0, 0, 0), scr2 = Vector(0, 0, 0);
		HiveDraw::WorldToScreen(pos1, scr1);
		HiveDraw::WorldToScreen(pos2, scr2);
		CHiveInterface.ISurface->DrawSetColor(c);
		int px[] = { scr1.x, scr2.x };
		int py[] = { scr1.y, scr2.y };
		CHiveInterface.ISurface->DrawPolyLine(px, py, 2);
	}

	void Clear(int x, int y, int w, int h, Color color)
	{
		CHiveInterface.ISurface->DrawSetColor(color);
		CHiveInterface.ISurface->DrawFilledRect(x, y, x + w, y + h);
	}

	void DrawOutlinedRect(int x, int y, int w, int h, Color col)
	{
		CHiveInterface.ISurface->DrawSetColor(col);
		CHiveInterface.ISurface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
	{
		CHiveInterface.ISurface->DrawSetColor(r, g, b, a);
		CHiveInterface.ISurface->DrawFilledRect(x, y, x + w, y + h);
	}

	void Draw2DBox(float x, float y, float w, float h, float px, int r, int g, int b, int a)
	{
		FillRGBA(x, (y + h - px), w, px, r, g, b, a);
		FillRGBA(x, y, px, h, r, g, b, a);
		FillRGBA(x, y, w, px, r, g, b, a);
		FillRGBA((x + w - px), y, px, h, r, g, b, a);
	}

	bool WorldToScreen(Vector &origin, Vector &screen)
	{
		const VMatrix worldToScreen = CHiveInterface.Engine->WorldToScreenMatrix();
		float w = worldToScreen[3][0] * origin[0] + worldToScreen[3][1] * origin[1] + worldToScreen[3][2] * origin[2] + worldToScreen[3][3]; //Calculate the angle in comparison to the player's camera.
		screen.z = 0; //Screen doesn't have a 3rd dimension.
		static screensize gScreenSize;
		CHiveInterface.Engine->GetScreenSize(gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);

		if (w > 0.001) //If the object is within view.
		{
			float fl1DBw = 1 / w; //Divide 1 by the angle.
			screen.x = (gScreenSize.iScreenWidth / 2) + (0.5 * ((worldToScreen[0][0] * origin[0] + worldToScreen[0][1] * origin[1] + worldToScreen[0][2] * origin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.iScreenWidth + 0.5); //Get the X dimension and push it in to the Vector.
			screen.y = (gScreenSize.iScreenHeight / 2) - (0.5 * ((worldToScreen[1][0] * origin[0] + worldToScreen[1][1] * origin[1] + worldToScreen[1][2] * origin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.iScreenHeight + 0.5); //Get the Y dimension and push it in to the Vector.
			return true;
		}

		return false;
	}

	void DrawConnection(ESPBox size, Color color, CBaseEntityNew* pEntity)
	{
		Vector src3D, src;
		src3D = pEntity->GetAbsOrigin() - Vector(0, 0, 0);
		if (!HiveDraw::WorldToScreen(src3D, src))
			return;

		int ScreenWidth, ScreenHeight;
		CHiveInterface.Engine->GetScreenSize(ScreenWidth, ScreenHeight);

		int x = (int)(ScreenWidth * 0.5f);
		int y = 0;


		y = ScreenHeight;

		HiveDraw::DrawLine((int)(src.x), (int)(src.y), x, y, Color(0, 255, 0, 255));
	}

	void DrawBarrel( Color color, CBaseEntityNew* pEntity)
	{
		QAngle dummy;
		Vector src3D, src, dst3D, box2D, dst;
		QAngle inAngle = pEntity->GetEyeAngles();
		NativeClass::GetBonePosition(pEntity, "ValveBiped.Bip01_Head1", src3D);

		trace_t tr;
		CTraceFilter filter;
		filter.pSkip = pEntity;
		Ray_t ray;
		QAngle qFix;
		Vector vEyePos = src3D;
		Vector vEnd;
		AngleVectors(inAngle, &vEnd);
		vEnd = vEnd * CLuaMenuCallback.ESPBarrelDist + vEyePos;
		ray.Init(vEyePos, vEnd);
		CHiveInterface.EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);
		dst3D = tr.endpos;
		box2D = tr.plane.normal;
		if (!HiveDraw::WorldToScreen(src3D, src))
			return;
		if (!HiveDraw::WorldToScreen(dst3D, dst))
			return;
		HiveDraw::DrawLine((int)(src.x), (int)(src.y), (int)(dst.x), (int)(dst.y), color);
		HiveDraw::Draw3DBox(dst3D - Vector(2, 2, 2), dst3D + Vector(3, 3, 3), color);
		QAngle Normal = QAngle(0, 0, 0);
		HiveMath::VectorAngles(box2D, Normal);
		QAngle NormalRight = Normal + QAngle(0, 90, 0);
		QAngle NormalUp = Normal + QAngle(-90, 0, 0);
		HiveMath::NormalizeAngles(NormalRight);
		HiveMath::NormalizeAngles(NormalUp);
		Vector right = Vector(0, 0, 0), up = Vector(0, 0, 0);
		HiveMath::AngleVectors(NormalRight, right);
		HiveMath::AngleVectors(NormalUp, up);
		Color Red = Color(255, 0, 0, 255);
		HiveDraw::DrawLine(dst3D - right - up, dst3D + right - up, Red);
		HiveDraw::DrawLine(dst3D - right + up, dst3D + right + up, Red);
		HiveDraw::DrawLine(dst3D - up - right, dst3D + up - right, Red);
		HiveDraw::DrawLine(dst3D - up + right, dst3D + up + right, Red);
		HiveDraw::DrawLine(dst3D - right - up, dst3D + right + up, Red);
		HiveDraw::DrawLine(dst3D - right + up, dst3D + right - up, Red);
																	   
	}
	void DrawCircle(int x, int y, int diameter, Color Col)
	{
		int quality = 50;
		diameter = diameter * 0.5;
		for (int i = 1; i <= quality; i++)
		{
			float ang = ((i * 360) * M_PI / 180.0) / quality;
			float ang2 = (((i+1) * 360) * M_PI / 180.0) / quality;
			vec_t lx = x + diameter * cos(ang);
			vec_t ly = y + diameter * sin(ang);
			vec_t lx2 = x + diameter * cos(ang2);
			vec_t ly2 = y + diameter * sin(ang2);
			DrawLine(lx, ly, lx2, ly2, Col);

		}

	}

	RECT GetTextSize(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t *wcstring = new wchar_t[100];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x = 0, y = 0;
		CSurface *surf = (CSurface*)CHiveInterface.ISurface;
		surf->GMOD_GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		//ConMsg("%x, x: %d, y: %d, convChars: %d, origsize: %d\n", font, x, y, convertedChars, origsize);
		delete[] wcstring;
		return rect;
	}

	void DrawCornerBox(ESPBox size, Color color)
	{

		int VertLine = (((float)size.w) * (0.10f));
		int HorzLine = (((float)size.h) * (0.10f));

		Clear(size.x, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
		Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
		Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));
		Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));

		Clear(size.x - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
		Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));
		Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
		Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));

		Clear(size.x, size.y, VertLine, 1, color);
		Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
		Clear(size.x, size.y + size.h, VertLine, 1, color);
		Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

		Clear(size.x, size.y, 1, HorzLine, color);
		Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
		Clear(size.x + size.w, size.y, 1, HorzLine, color);
		Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	}

	void DrawSkeleton(Color color, CBaseEntityNew* target) 
	{
		matrix3x4_t bones[128];
		C_BasePlayerNew* Player = (C_BasePlayerNew*)target;
		uintptr_t ClientRenderable = (uintptr_t)Player->GetClientRenderable();
		float EngineTime = CHiveInterface.Engine->Time();
		bool Setup = Player->GetClientRenderable()->SetupBones(bones, 128, BONE_USED_BY_HITBOX, EngineTime);
		if (ClientRenderable < 0x1000 || !Setup)
			return;
		studiohdr_t* studioHdr = CHiveInterface.ModelInfo->GetStudiomodel((const model_t*)Player->GetClientRenderable()->GetModel());
		
		for (int z = 0; z < studioHdr->numbones; z++)
		{
			auto bone = studioHdr->pBone(z);
			if (bone && bone->parent >= 0)
			{
				if (!(bone->flags & 256))
					continue;
				Vector normalBonePos = Vector(bones[z][0][3], bones[z][1][3], bones[z][2][3]);
				Vector normalParentBonePos = Vector(bones[bone->parent][0][3], bones[bone->parent][1][3], bones[bone->parent][2][3]);
				if (normalBonePos == Vector(0, 0, 0) || normalParentBonePos == Vector(0, 0, 0))
					continue;

				Vector bonePosFrom;
				Vector parentBonePos;
				if (!WorldToScreen(normalBonePos, bonePosFrom) || !WorldToScreen(normalParentBonePos, parentBonePos))
					continue;
				DrawLine(bonePosFrom.x, bonePosFrom.y, parentBonePos.x, parentBonePos.y, Color(255,255,255,255));
				//DrawString(bonePosFrom.x, bonePosFrom.y, Color(255, 255, 255, 255), Fonts::ESP, bone->pszName());
			}
		}

		/*
		HiveDraw::DrawBone(NativeClass::BonesTable[0], NativeClass::BonesTable[1], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[1], NativeClass::BonesTable[2], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[2], NativeClass::BonesTable[3], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[3], NativeClass::BonesTable[4], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[4], NativeClass::BonesTable[5], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[2], NativeClass::BonesTable[6], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[6], NativeClass::BonesTable[7], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[7], NativeClass::BonesTable[8], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[2], NativeClass::BonesTable[9], target,   color);
		HiveDraw::DrawBone(NativeClass::BonesTable[9], NativeClass::BonesTable[10], target,  color);
		HiveDraw::DrawBone(NativeClass::BonesTable[10], NativeClass::BonesTable[11], target, color);
		HiveDraw::DrawBone(NativeClass::BonesTable[5], NativeClass::BonesTable[12], target,  color);
		HiveDraw::DrawBone(NativeClass::BonesTable[12], NativeClass::BonesTable[13], target, color);
		HiveDraw::DrawBone(NativeClass::BonesTable[13], NativeClass::BonesTable[14], target, color);
		HiveDraw::DrawBone(NativeClass::BonesTable[14], NativeClass::BonesTable[15], target, color);
		HiveDraw::DrawBone(NativeClass::BonesTable[5], NativeClass::BonesTable[16], target,  color);
		HiveDraw::DrawBone(NativeClass::BonesTable[16], NativeClass::BonesTable[17], target, color);
		HiveDraw::DrawBone(NativeClass::BonesTable[17], NativeClass::BonesTable[18], target, color);
		HiveDraw::DrawBone(NativeClass::BonesTable[18], NativeClass::BonesTable[19], target, color);
		*/
	}

	void DrawWeapon(const char* weapon, ESPBox size)
	{
		RECT weaponSize = GetTextSize((DWORD)Fonts::ESP, weapon);
		DrawString(size.x + (size.w / 2) - (weaponSize.right / 2), size.y + size.h + 4,
		Color(255, 255, 255, 255), Fonts::ESP, weapon);
	}

	void DrawName(player_info_t pinfo, ESPBox size)
	{
		RECT nameSize = GetTextSize((DWORD)Fonts::ESP, pinfo.name);
		DrawString(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16,
		Color(255, 255, 255, 255), Fonts::ESP, pinfo.name);
	}

	void DrawName(const char* name, ESPBox size)
	{
		RECT nameSize = GetTextSize((DWORD)Fonts::ESP, name);
		DrawString(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16,
			Color(255, 255, 255, 255), Fonts::ESP, name);
	}

	void DrawDist(CBaseEntityNew* pEntity, CBaseEntityNew* me, ESPBox size)
	{
		Vector vdist = pEntity->GetAbsOrigin() - me->GetEyePosition();
		int dist = vdist.Length2D();
		char distBuffer[15];
		sprintf_s(distBuffer, "%d", dist);
		RECT distSize = GetTextSize((DWORD)Fonts::ESP, distBuffer);
		DrawString(size.x + (size.w / 2) - (distSize.right / 2), size.y - 25,
			Color(255, 255, 255, 255), Fonts::ESP, distBuffer);
	}

	void DrawHealth(CBaseEntityNew* pEntity, ESPBox size)
	{
		int HPEnemy = 100;
		HPEnemy = pEntity->Health();
		char strBuffer[512];
		sprintf_s(strBuffer, "%d", HPEnemy);
		

		float h = (size.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float health = pEntity->Health();
		if (health > 100)
			health = 100;

		if (health < 0)
			health = 0;
		UINT hp = h - (UINT)((h * health) / 100);

		int Red = 255 - (health*2.55);
		int Green = health*2.55;
		RECT hpSize = GetTextSize((DWORD)Fonts::ESP, strBuffer);
		DrawOutlinedRect((size.x - 6) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 180));

		DrawLine((size.x - 6), size.y + hp, (size.x - 6), size.y + h, Color(Red, Green, 0, 180));

		if (health) {

			DrawString(size.x - hpSize.right - 9, size.y + hp, Color(255, 255, 255, 255), Fonts::ESP, strBuffer);
		}
	}

	void DrawArmor(CBaseEntityNew* pEntity, ESPBox size)
	{
		int ArmorEnemy = CHiveInterface.PlayerResource->GetArmor(pEntity->Index());;
		if (!ArmorEnemy)
			return;
		char strBuffer[512];
		sprintf_s(strBuffer, "%d", ArmorEnemy);


		float h = (size.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float armor = ArmorEnemy;
		if (armor > 100)
			armor = 100;
		if (armor < 0)
			armor = 0;
		UINT ap = h - (UINT)((h * armor) / 100);

		int Green = 255 - (armor * 2.55);
		int Blue = armor * 2.55;
		RECT armorSize = GetTextSize((DWORD)Fonts::ESP, strBuffer);
		DrawOutlinedRect((size.x + size.w + 5) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 180));

		DrawLine((size.x + size.w + 5), size.y + ap, (size.x + size.w + 5), size.y + h, Color(0, Green, Blue, 180));
		
		if (armor) {

			DrawString(size.x + size.w + 8, size.y + ap, Color(255, 255, 255, 255), Fonts::ESP, strBuffer);
		}
		
	}

	void DrawBacktrackTicks(CBaseEntityNew* pEntity)
	{
		if (!HiveCheats::cBacktrackInterface.bInit)
			return;

		if (HiveCheats::cBacktrackInterface.m_mEntities[pEntity->Index()].m_pPlayerEntity && pEntity->Index() == HiveCheats::aimbot_target)
		{
			for (int i = 0; i < CLuaMenuCallback.Backtrack_max_tick; i++)
			{
				std::map<const char*, Vector> BoneMap = HiveCheats::cBacktrackInterface.m_mEntities[pEntity->Index()].GetFrame(i).m_mBoneMap;
				Vector bonepos = BoneMap["ValveBiped.Bip01_Spine2"];
				Vector abspos = HiveCheats::cBacktrackInterface.m_mEntities[pEntity->Index()].GetFrame(i).m_vAbsOrigin;
				Vector originpos = HiveCheats::cBacktrackInterface.m_mEntities[pEntity->Index()].GetFrame(i).m_vOrigin;
				abspos.z += 32;
				originpos.z += 64;
				Vector w2s;
				Vector w2s2;
				Vector w2s3;
				HiveDraw::WorldToScreen(bonepos, w2s);
				HiveDraw::WorldToScreen(abspos, w2s2);
				HiveDraw::WorldToScreen(originpos, w2s3);
				HiveDraw::FillRGBA(w2s.x, w2s.y, 5, 5, 255, 100, 40, 150);
				HiveDraw::FillRGBA(w2s2.x, w2s2.y, 5, 5, 40, 100, 255, 150);
				Color col = Color(255, 255, 255, 255);
				/*
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
				*/
				//HiveDraw::DrawCircle(w2s2.x, w2s2.y, 10, Color(0, 255, 0, 255));
				//HiveDraw::DrawCircle(w2s3.x, w2s3.y, 10, Color(0, 0, 255, 255));
			}
		}
	}

	void InitializeFonts()
	{
		Fonts::Default = 0x1D;
		Fonts::Menu = CHiveInterface.ISurface->CreateFont();
		Fonts::MenuBold = CHiveInterface.ISurface->CreateFont();
		Fonts::ESP = CHiveInterface.ISurface->CreateFont();
		Fonts::MenuText = CHiveInterface.ISurface->CreateFont();
		Fonts::Icon = CHiveInterface.ISurface->CreateFont();
		Fonts::MenuTabs = CHiveInterface.ISurface->CreateFont();
		Fonts::MenuText = CHiveInterface.ISurface->CreateFont();
		Fonts::Slider = CHiveInterface.ISurface->CreateFont();
		Fonts::Clock = CHiveInterface.ISurface->CreateFont();
		Fonts::LBY = CHiveInterface.ISurface->CreateFont();
		Fonts::Tabs = CHiveInterface.ISurface->CreateFont();

		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::Menu, "DINPro-Regular", 14, 500, 0, 0, FONTFLAG_ANTIALIAS);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::Text, "DINPro-Regular", 30, 500, 0, 0, FONTFLAG_ANTIALIAS);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::MenuBold, "DINPro-Regular", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::ESP, "Calibri", 14, 500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::MenuText, "Calibri", 16, 500, 0, 0, FONTFLAG_ANTIALIAS);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::MenuTabs, "MyScriptFont", 18, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::Slider, "Arial", 17, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::Clock, "Arial", 22, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::Tabs, "MyScriptFont", 50, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		CHiveInterface.ISurface->SetFontGlyphSet(Fonts::LBY, "Verdana", 20, 800, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);
	}

	void Draw3DBox(const Vector &mins, const Vector &maxs, Color c)
	{
		Vector diff = maxs - mins;
		Vector pointsToDraw[24] = {
		// Bottom Horizontal lines
		mins, mins + Vector(diff.x, 0, 0), // 1 - 2
		mins + Vector(diff.x, 0, 0), mins + Vector(diff.x, diff.y, 0), // 2 - 3
		mins + Vector(diff.x, diff.y, 0), mins + Vector(0, diff.y, 0), // 3 - 4
		mins + Vector(0, diff.y, 0), mins, // 4 - 1
		mins, mins + Vector(0, 0, diff.z), // 1 - 6
		mins + Vector(diff.x, 0, 0), mins + Vector(diff.x, 0, diff.z), // 2 - 7
		mins + Vector(diff.x, diff.y, 0), mins + Vector(diff.x, diff.y, diff.z), // 3 - 8
		mins + Vector(0, diff.y, 0), mins + Vector(0, diff.y, diff.z), // 4 - 5
		maxs, maxs - Vector(diff.x, 0, 0), // 8 - 5
		maxs - Vector(diff.x, 0, 0), maxs - Vector(diff.x, diff.y, 0), // 5 - 6
		maxs - Vector(diff.x, diff.y, 0), maxs - Vector(0, diff.y, 0), // 6 - 7
		maxs - Vector(0, diff.y, 0), maxs // 7 - 8
		};
		Vector startPos, endPos;
		for (int i = 0; i < 24; i += 2)
		{
			if (WorldToScreen(pointsToDraw[i], startPos))
			{
				if (WorldToScreen(pointsToDraw[i + 1], endPos))
					DrawLine(startPos.x, startPos.y, endPos.x, endPos.y, c);
			}
		}
	}

	void DrawHealthbar(int Health, Vector vScreen, int x, int y)
	{
		if (Health <= 100) {
			int r = 255 - (Health*2.55);
			int g = Health*2.55;

			FillRGBA(vScreen.x - x - 1, vScreen.y - y - 1, 4, 31, RED(HIVE_DRAW_BLACK), GREEN(HIVE_DRAW_BLACK), BLUE(HIVE_DRAW_BLACK), ALPHA(HIVE_DRAW_BLACK));
			FillRGBA(vScreen.x - x, vScreen.y - y, 2, 29, r, g, 0, 255);
			FillRGBA(vScreen.x - x, vScreen.y - y, 2, 29 - (Health / 2.5), RED(HIVE_DRAW_BLACK), GREEN(HIVE_DRAW_BLACK), BLUE(HIVE_DRAW_BLACK), ALPHA(HIVE_DRAW_BLACK));
		}
		else {
			int r = 255 - (100 * 2.55);
			int g = 100 * 2.55;

			FillRGBA(vScreen.x - x - 1, vScreen.y - y - 1, 4, 31, RED(HIVE_DRAW_BLACK), GREEN(HIVE_DRAW_BLACK), BLUE(HIVE_DRAW_BLACK), ALPHA(HIVE_DRAW_BLACK));
			FillRGBA(vScreen.x - x, vScreen.y - y, 2, 29, r, g, 0, 255);
			FillRGBA(vScreen.x - x, vScreen.y - y, 2, 29 - (100 / 2.5), RED(HIVE_DRAW_BLACK), GREEN(HIVE_DRAW_BLACK), BLUE(HIVE_DRAW_BLACK), ALPHA(HIVE_DRAW_BLACK));
		}
	}

	void DrawBone(char* StartBone, char*  EndBone, CBaseEntityNew* target, Color drawCol)
	{
		static QAngle dummy;
		Vector StartBonePos, EndBonePos;
		Vector StartDrawPos, EndDrawPos;
		if (!target)
			return;

		NativeClass::GetBonePosition(target, StartBone, StartBonePos);
		NativeClass::GetBonePosition(target, EndBone, EndBonePos);

		if (StartBonePos.IsZero() || EndBonePos.IsZero())
			return;

		if (!HiveDraw::WorldToScreen(StartBonePos, StartDrawPos))
			return;

		if (!HiveDraw::WorldToScreen(EndBonePos, EndDrawPos))
			return;
		DrawLine(StartDrawPos.x, StartDrawPos.y, EndDrawPos.x, EndDrawPos.y, drawCol);

	}

	void DrawBone(Vector StartBone, Vector EndBone, Color drawCol)
	{
		Vector StartDrawPos, EndDrawPos;

		if (StartBone.IsZero() || EndBone.IsZero())
			return;

		if (!HiveDraw::WorldToScreen(StartBone, StartDrawPos))
			return;

		if (!HiveDraw::WorldToScreen(EndBone, EndDrawPos))
			return;
		DrawLine(StartDrawPos.x, StartDrawPos.y, EndDrawPos.x, EndDrawPos.y, drawCol);

	}

	volatile bool GetESPBox(CBaseEntityNew* pEntity, ESPBox &result)
	{
		 Vector  vOrigin, min, max, sMin, sMax, sOrigin,
			flb, brt, blb, frt, frb, brb, blt, flt;
		 float left, top, right, bottom, TMaxs, TMins;

		vOrigin = pEntity->GetAbsOrigin();
		//ConMsg("%d, %d, %d, %d\n", vOrigin.x, vOrigin.y, vOrigin.z, pEntity->Health());
		Vector tMaxs, tMins;
		//CHiveSourceNative.GetRenderBounds(pEntity, tMaxs, tMins);
		min = pEntity->OBBMins() + vOrigin;
		max = pEntity->OBBMaxs() + vOrigin;
		//ConMsg("%d, %d, %d, %d\n", min.x, min.y, max.x, max.y);

		Vector points[8] = { Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z) };

		if (!HiveDraw::WorldToScreen(points[3], flb) || !HiveDraw::WorldToScreen(points[5], brt)
			|| !HiveDraw::WorldToScreen(points[0], blb) || !HiveDraw::WorldToScreen(points[4], frt)
			|| !HiveDraw::WorldToScreen(points[2], frb) || !HiveDraw::WorldToScreen(points[1], brb)
			|| !HiveDraw::WorldToScreen(points[6], blt) || !HiveDraw::WorldToScreen(points[7], flt))
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		left = flb.x;
		top = flb.y;
		right = flb.x;
		bottom = flb.y;
		for (volatile int i = 1; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;
			if (bottom < arr[i].y)
				bottom = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (top > arr[i].y)
				top = arr[i].y;
		}

		result.x = left;
		result.y = top;
		result.w = right - left;
		result.h = bottom - top;
		

		return true;
	}

}