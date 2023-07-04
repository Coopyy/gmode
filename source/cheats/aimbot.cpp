#include "aimbot.h"
#include "../cheat.h"

void aimbot::run(CUserCmd* cmd)
{
	if (!(cmd->buttons & (1 << 0))) // IN_ATTACK 
		return;

	for (size_t i = 0; i < 64; i++)
	{
		auto entity = interfaces::entityList->GetEntityFromIndex(i);
		if (!entity || !entity->IsPlayer() || entity == g::localPlayer)
			continue;

		auto pos = entity->GetAbsOrigin();

		printf("[0x%p] entity (%f, %f, %f)\n", entity, pos.x, pos.y, pos.z);
	}
}
