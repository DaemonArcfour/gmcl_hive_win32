#pragma once
#include "hive_dependencies.h"
class CBaseEntityNew;
#define HiveFont "Courier New"

#define HIVE_DRAW_ORANGE    COLORCODE( 255, 100, 000, 255 )
#define HIVE_DRAW_YELLOW    COLORCODE( 255, 255, 0, 255 )
#define HIVE_DRAW_PURPLE    COLORCODE( 55, 25, 128, 255 )
#define HIVE_DRAW_RED       COLORCODE( 255, 0, 0, 255 )
#define HIVE_DRAW_GREEN		COLORCODE( 0, 255, 0, 255 )
#define HIVE_DRAW_BLUE		COLORCODE( 0, 0, 255, 255 )
#define HIVE_DRAW_BLACK		COLORCODE( 0, 0, 0, 255 )
#define HIVE_DRAW_WHITE		COLORCODE( 255, 255, 255, 255 )
#define HIVE_DRAW_GREY		COLORCODE( 120, 120, 120, 255 )

#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))
#define RED(COLORCODE)	((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)	((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)	((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)	((int) COLORCODE & 0xFF )
#define RGBA(COLORCODE) RED( COLORCODE ), GREEN( COLORCODE ), BLUE( COLORCODE ), ALPHA( COLORCODE )
class ESPBox {
public:
	int x;
	int y;
	int w;
	int h;
};

namespace HiveDraw {
	void DrawHealthbar(int Health, Vector vScreen, int x, int y);
	void DrawString(int x, int y, Color col, vgui::HFont Font, const char* text, ...);
	void DrawLine(int x, int y, int w, int h, Color c);
	void DrawLine(Vector pos1, Vector pos2, Color c);
	void DrawCircle(int x, int y, int diameter, Color c);
	void DrawCornerBox(ESPBox size, Color color);
	void DrawSkeleton(Color color, CBaseEntityNew* target);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void DrawName(player_info_t pinfo, ESPBox size);
	void DrawName(const char* name, ESPBox size);
	void InitializeFonts();
	void DrawWeapon(const char* weapon, ESPBox size);
	void Draw2DBox(float x, float y, float w, float h, float px, int r, int g, int b, int a);
	void DrawBone(char* StartBone, char*  EndBone, CBaseEntityNew* target, Color drawCol);
	void DrawHealth(CBaseEntityNew* pEntity, ESPBox size);
	void DrawBarrel(Color color, CBaseEntityNew* pEntity);
	void DrawConnection(ESPBox size, Color color, CBaseEntityNew* pEntity);
	void DrawDist(CBaseEntityNew* pEntity, CBaseEntityNew* me, ESPBox size);
	void Clear(int x, int y, int w, int h, Color color);
	volatile bool GetESPBox(CBaseEntityNew* pEntity, ESPBox &result);
	void Draw3DBox(const Vector &mins, const Vector &maxs, Color c);
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	bool WorldToScreen(Vector &origin, Vector &screen);
	namespace Fonts
	{
		extern DWORD Default;
		extern DWORD Menu;
		extern DWORD MenuBold;
		extern DWORD ESP;
		extern DWORD MenuText;
		extern DWORD Icon;
		extern DWORD MenuTabs;
		extern DWORD Text;
		extern DWORD Slider;
		extern DWORD Clock;
		extern DWORD LBY;
		extern DWORD Tabs;
	};
}
class HiveFonts {
public:
	vgui::HFont MenuFont;
	vgui::HFont SmallFont;
	vgui::HFont MediumFont;
	vgui::HFont vguiMenuFont;
	vgui::HFont vguiSmallFont;
	vgui::HFont vguiMediumFont;
	void LoadFonts();
}; extern HiveFonts CHiveFonts;

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,		// custom generated font - never fall back to asian compatibility mode
	FONTFLAG_BITMAP = 0x800,		// compiled bitmap font - no fallbacks
};