#include "hive_playerresource.h"
#include "hive_dependencies.h"
int CPlayerResourceNew::GetArmor(int index)
{
	if (*(DWORD*)this == NULL)
		return 0;

	return 0;
	//return *reinterpret_cast<int*>(*(DWORD*)this + HiveNetVarOffsets::m_iArmor + index * 4);
}