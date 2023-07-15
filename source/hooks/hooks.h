#pragma once
#ifndef HOOKS_H
#define HOOKS_H
class CUserCmd;
namespace hooks
{
	void load();
	void unload();

	using hkCreateMove = bool(__thiscall*)(void*, float, CUserCmd*);
	inline hkCreateMove CreateMoveOriginal = nullptr;
	bool __stdcall CreateMove(void* clientMode, float frameTime, CUserCmd* cmd);
}
#endif // !HOOKS_H
