#include "../sdk/CUserCmd.h"
namespace hooks
{
	void load();
	void unload();

	using hkCreateMove = bool(__thiscall*)(void*, float, CUserCmd*);
	inline hkCreateMove CreateMoveOriginal = nullptr;
	bool __stdcall CreateMove(float frameTime, CUserCmd* cmd);
}