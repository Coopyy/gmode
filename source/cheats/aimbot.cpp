#include "aimbot.h"
#include "../globals.h"
#include "../sdk/interfaces.h"
#include "../utils.h"

#include "../utils/easing/easing.hpp"
#include "../utils/easing/easing_dynamic.hpp"

constexpr float maxFOV = 10;
constexpr int size = 3;
constexpr int possBones[size] = { 6, 3, 0 };

inline bool getBestAimPos(C_BasePlayer* entity, const Vector& src, const QAngle& srcVA, QAngle* bestAngle, float* bestFOV)
{
	matrix3x4_t bones[128];
	if (!entity->GetClientRenderable()->SetupBones(bones, 128, BONE_USED_BY_HITBOX, interfaces::globalVars->curtime))
		return false;

	bool foundAny = false;
	float compareFOV = maxFOV;
	QAngle compareAngle;
	int bone;

	for (size_t i = 0; i < size; i++)
	{
		bone = possBones[i];

		Vector currentPosition;
		currentPosition.x = bones[bone][0][3];
		currentPosition.y = bones[bone][1][3];
		currentPosition.z = bones[bone][2][3];

		if (!utils::CanSeePlayer(entity, currentPosition))
			continue;

		auto enemyAngle = (currentPosition - src).toAngle() - srcVA;
		enemyAngle.FixAngles();
		if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < compareFOV)
		{
			compareAngle = enemyAngle;
			compareFOV = fov;
			foundAny = true;
		}
	}

	if (foundAny)
	{
		*bestAngle = compareAngle;
		*bestFOV = compareFOV;
		return true;
	}
	return false;
}

C_BasePlayer* currentTarget = nullptr;

void aimbot::run(CUserCmd* cmd)
{
	if (!GetAsyncKeyState(VK_LBUTTON) || !interfaces::matSystemSurface->IsCursorLocked())
	{
		currentTarget = nullptr;
		return;
	}
	auto srcEyes = g::localPlayer->EyePosition();
	bool foundTarget = false;

	QAngle bestAngle;
	float bestFOV = maxFOV;

	if (!currentTarget || !currentTarget->IsValidTarget() || !getBestAimPos(currentTarget, srcEyes, cmd->viewangles, &bestAngle, &bestFOV))
	{
		for (size_t i = 0; i < interfaces::entityList->GetMaxEntities(); i++)
		{
			auto entity = (C_BasePlayer*)interfaces::entityList->GetClientEntity(i);
			if (!entity || entity == g::localPlayer || !entity->IsValidTarget())
				continue;

			QAngle enemyAngle;
			float fov;

			if (!getBestAimPos(entity, srcEyes, cmd->viewangles, &enemyAngle, &fov))
				continue;

			if (fov < bestFOV)
			{
				bestAngle = enemyAngle;
				bestFOV = fov;
				foundTarget = true;
				currentTarget = entity;
			}
		}
	}
	else
		foundTarget = true;

	if (!foundTarget)
		return;

	static auto easePitch = easing::getEasingFunction(easing::EasingFunctions::inOutQuint);
	static auto easeYaw = easing::getEasingFunction(easing::EasingFunctions::inOutQuint);

	float easedPitch = (0.15f) * easePitch(1 - bestFOV / maxFOV);
	float easedYaw = (0.1f) * easeYaw(1 - bestFOV / maxFOV);

	bestAngle.x *= easedPitch;
	bestAngle.y *= easedYaw;

	cmd->viewangles += bestAngle;
	cmd->viewangles.FixAngles();
	interfaces::engineClient->SetViewAngles(cmd->viewangles);
}