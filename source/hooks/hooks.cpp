#include "../libs.h"

#include "hooks.h"
#include "../../external/minhook/MinHook.h"
#include "../memory.h"
#include "../globals.h"

#include "../sdk.h"
#include "../sdk/interfaces.h"
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

bool __stdcall hooks::CreateMove(void* cm, float frameTime, CUserCmd* cmd)
{
	g::localPlayer = (C_BasePlayer*)interfaces::entityList->GetClientEntity(interfaces::engineClient->GetLocalPlayer());
	if (cmd && g::localPlayer)
		aimbot::run(cmd);

	return CreateMoveOriginal(cm, frameTime, cmd);
}
