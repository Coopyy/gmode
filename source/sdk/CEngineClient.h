#include "../memory/memory.h"
class CEngineClient
{
public:

	constexpr int GetLocalPlayerIndex()
	{
		return memory::call<int>(this, 12);
	}
};