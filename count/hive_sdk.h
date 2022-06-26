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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <limits>
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


#pragma comment(lib, "tier0.lib")
#pragma comment(lib, "tier1.lib")
#pragma comment(lib, "tier2.lib")
#pragma comment(lib, "tier3.lib")
#pragma comment(lib, "mathlib.lib")
#pragma comment(lib, "vstdlib.lib")
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "winmm.lib")

class GMODCUserCmd
{
public:
	char _0x0000[4];
	int command_number; //0x0004 
	float tick_count; //0x0008 
	QAngle viewangles; //0x000C 
	float forwardmove; //0x0018 
	float sidemove; //0x001C 
	float upmove; //0x0020 
	int buttons; //0x0024 
	char _0x0028[12];
	int random_seed; //0x0034 
	__int16 mousedx; //0x0038 
	__int16 mousey; //0x003A 
	char _0x003C[288];

};