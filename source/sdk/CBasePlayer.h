#include "../memory/memory.h"
#include "Vector.h"
class CBasePlayer
{
public:

	constexpr Vector& GetAbsOrigin()
	{
		return memory::call<Vector&>(this, 9);
	}

	constexpr bool IsPlayer()
	{
		return memory::call<bool>(this, 130);
	}
};