#pragma once
#include "hive_dependencies.h"
class CPlayerInfoManager
{
public:
	virtual void *GetPlayerInfo(edict_t *pEdict);
	virtual CGlobalVars *GetGlobalVars();
};