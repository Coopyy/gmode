#include "../includes.h"
#include "../memory/memory.h"
#include "CBasePlayer.h"
class CClientEntityList
{
public:
	constexpr CBasePlayer* GetEntityFromIndex(int index)
	{
		return memory::call<CBasePlayer*>(this, 3, index);
	}

	constexpr int GetHighestEntityIndex()
	{
		return memory::call<int>(this, 6);
	}
};