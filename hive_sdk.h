#pragma once
#ifndef CLIENT_DLL
#define CLIENT_DLL
#endif
#define SECURITY_WIN32
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <time.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <stringapiset.h>
#include <math.h>
#include <xstring>
#include <utility>
#include <WinUser.h>
#include <stdio.h>
#include <wingdi.h>
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <tlhelp32.h>
#include <psapi.h>
#include <memory>
#include <math.h>
#include <map>
#include <deque>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <limits>
#include <filesystem>
#include <shlwapi.h>
#include <unordered_set>
#include "public\\tier0\\wchartypes.h"
#include "public\\tier0\\dbg.h"
// Public
#include "public\\cdll_int.h"
#include "public\\iprediction.h"
#include "public\\bone_setup.h"
#include "public\\icliententitylist.h"
#include "public\\ienginevgui.h"
#include "public\\IGameUIFuncs.h"
#include "public\\dlight.h"
#include "public\\r_efx.h"
#include "public\\igameevents.h"
#include "public\\view_shared.h"
#include "public\\inetchannelinfo.h"
#include "public\\iachievementmgr.h"
#include "public\\steam\\steam_api.h"
#include "public\\steam\\isteamuserstats.h"

// Client
#include "game\\client\\imessagechars.h"
#include "game\\client\\iclientmode.h"
#include "game\\client\\cliententitylist.h"
#include "game\\client\\cdll_client_int.h"
#include "game\\client\\cbase.h"
#include "game\\client\\c_baseanimating.h"
#include "game\\client\\c_basecombatweapon.h"
#include "game\\client\\c_baseplayer.h"
#include "game\\client\\enginesprite.h"
#include "game\\client\\input.h"
#include "game\\client\\c_playerresource.h"
#include "game\\client\\iviewrender.h"
#include "game\\client\\viewrender.h"
#include "game\\client\\game_controls\\commandmenu.h"
#include "game\\client\\hudelement.h"
#include "game\\client\\prediction.h"

// Engine
#include "public\\engine\\ivmodelrender.h"
#include "public\\engine\\ivdebugoverlay.h"
#include "public\\engine\\ivmodelinfo.h"
#include "public\\ivrenderview.h"
#include "public\\engine\\IEngineTrace.h"
#include "public\\engine\\IEngineSound.h"

// Material System
#include "public\\materialsystem\\imaterialsystemstub.h"
#include "public\\materialsystem\\itexture.h"
#include "public\\materialsystem\\IMaterialVar.h"
#include "public\\tier1\\checksum_md5.h"
#include "public\\materialsystem\\imaterial.h"
// VGUI
#include "public\\matsys_controls\\matsyscontrols.h"
#include "public\\vgui\\IClientPanel.h"
#include "public\\vgui\\IPanel.h"
#include "public\\vgui\\ISurface.h"
#include "public\\vgui\\ILocalize.h"
#include "public\\collisionutils.h"
#include "public\\vstdlib\\random.h"

//ICVAR
#include "public\\icvar.h"
#include "public\\tier1\\convar.h"
#include "public\\tier1\\iconvar.h"
#include "public\\tier1\\interface.h"
// VGUI Controls
#include "public\\vgui_controls\\Panel.h"

// VGUI Material Surface
#include "public\\VGuiMatSurface\\IMatSystemSurface.h"
#include "public\\mathlib\\vector.h"

// Shared
#include "game\\shared\\usermessages.h"
#include "game\\shared\\basecombatweapon_shared.h"
#include "game\\shared\\takedamageinfo.h"
#include "game\\shared\\igamemovement.h"
// Net Channel
#include "public\\inetchannel.h"
// Swap
#include "include\\IGameConsole.h"


//OpenSSL
#include "openssl/md5.h"
#include "openssl/aes.h"
#include "openssl/rc4.h"
#include "openssl/rsa.h"
#include "openssl/blowfish.h"

#pragma comment(lib, "OpenSSL/lib/libcryptoMD.lib")
#pragma comment(lib, "OpenSSL/lib/libsslMD.lib")

#pragma comment(lib, "tier0.lib")
#pragma comment(lib, "tier1.lib")
#pragma comment(lib, "tier2.lib")
#pragma comment(lib, "tier3.lib")
#pragma comment(lib, "mathlib.lib")
#pragma comment(lib, "vstdlib.lib")
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Shlwapi.lib")





enum playercontrols
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),
	IN_SPEED = (1 << 17),
	IN_WALK = (1 << 18),
	IN_ZOOM = (1 << 19),
	IN_WEAPON1 = (1 << 20),
	IN_WEAPON2 = (1 << 21),
	IN_BULLRUSH = (1 << 22),
};

class GMODCUserCmd
{
public:
	unsigned		command_number;
	unsigned		tick_count;
	QAngle			viewangles;
	float			forwardmove;
	float		    sidemove;
	float		    upmove;
	unsigned		buttons;
	char			impulse;
	int				weapon_select;
	int				weapon_type;
	unsigned		random_seed;
	short			mousedx;
	short			mousedy;
	bool			predicted;
	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weapon_select, sizeof(weapon_select));
		CRC32_ProcessBuffer(&crc, &weapon_type, sizeof(weapon_type));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_ProcessBuffer(&crc, &predicted, sizeof(predicted));
		CRC32_Final(&crc);
		return crc;
	}

	uint8_t buttons_pressed[5]; // holds current buttons pressed and sends to server used for PlayerButtonDown and other hooks on server
	int8_t scroll_wheel_speed;
	bool world_clicking; // used for context menu aiming
	Vector world_click_direction; // this too
	bool is_typing; // does hand to ear animation
	Vector motion_sensor_positions[20]; // kinect stuff
	bool forced; // CUserCmd_IsForced check gmod wiki

};