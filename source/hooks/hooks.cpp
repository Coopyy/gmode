#include "hooks.h"
#include "../../external/minhook/MinHook.h"
#include "../memory/memory.h"
#include "../sdk/interfaces/interfaces.h"
#include "../globals.h"

#include "../cheats/aimbot.h"

void hooks::load()
{
	printf("Hooking methods\n");
	MH_Initialize();

	MH_CreateHook(
		memory::get(interfaces::clientMode, 21),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	);

	MH_EnableHook(MH_ALL_HOOKS);
	printf("Hooks created\n");
}

void hooks::unload()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd)
{
	CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	/*if (!cmd || !cmd->command_number)
		return false;*/

	g::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engineClient->GetLocalPlayerIndex());

	if (!g::localPlayer)
		return false;

	aimbot::run(cmd);

	return false;
}
