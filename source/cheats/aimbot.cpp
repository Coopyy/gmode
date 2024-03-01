#include "aimbot.h"
#include "../globals.h"
#include "../sdk/interfaces.h"
#include "../utils.h"

#include "../utils/easing/easing.hpp"
#include "../utils/easing/easing_dynamic.hpp"
#include <map>

constexpr int size = 2;
constexpr const char* possBones[size] = { "ValveBiped.Bip01_Head1", "ValveBiped.Bip01_Spine4" };
constexpr float maxCQFOV = 65.0f;
constexpr float maxCQDist = 500;
constexpr float maxFOV = 10;

constexpr float pitchStrength = 0.30f;
constexpr float yawStrength = 0.20f;

// move mouse to screen position
void moveMouse(float deltaX, float deltaY)
{
	// round away from 0
	int ideltaX = deltaX > 0 ? Ceil2Int(deltaX) : Floor2Int(deltaX);
	int ideltaY = deltaY > 0 ? Ceil2Int(deltaY) : Floor2Int(deltaY);

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.time = 0;
	input.mi.dwExtraInfo = 0;
	input.mi.dx = ideltaX;
	input.mi.dy = ideltaY;

	SendInput(1, &input, sizeof(INPUT));
}

bool deltaFromAimPos(Vector worldPos, float* deltaX, float* deltaY)
{
	Vector screenPos;
	if (interfaces::debugOverlay->ScreenPosition(worldPos, screenPos))
		return false;

	int screenWidth = interfaces::client->GetScreenWidth();
	int screenHeight = interfaces::client->GetScreenHeight();

	if (screenPos.x < 0 || screenPos.x > screenWidth || screenPos.y < 0 || screenPos.y > screenHeight)
		return false;

	float rdeltaX = screenPos.x - ((float)screenWidth / 2);
	float rdeltaY = screenPos.y - ((float)screenHeight / 2);

	if (std::abs(rdeltaX) < 2 && std::abs(rdeltaY) < 2)
		return false;

	*deltaX = rdeltaX;
	*deltaY = rdeltaY;

	return true;
}

int Studio_BoneIndexByName(studiohdr_t* pStudioHdr, char const* pName)
{
	for (size_t i = 0; i < pStudioHdr->numbones; i++)
	{
		auto bone = pStudioHdr->pBone(i);
		auto boneName = bone->pszName();

		if (strcmp(boneName, pName) == 0)
		{
			return i;
		}
	}

	return -1;
}

matrix3x4_t* bones = nullptr;
bool getBonePosByName(C_BasePlayer* entity, const char* boneName, Vector* outPos)
{
	if (!bones)
		bones = new matrix3x4_t[512];

	bool shouldFallback = false;

	if (!entity->GetClientRenderable()->SetupBones(bones, 512, BONE_USED_BY_HITBOX, 0))
		shouldFallback = true;

	auto model = entity->GetClientRenderable()->GetModel();
	if (!model)
		shouldFallback = true;

	auto studioModel = interfaces::modelInfo->GetStudiomodel((const model_t*)model);
	if (!studioModel)
		shouldFallback = true;

	int boneIndex = Studio_BoneIndexByName(studioModel, boneName);
	if (boneIndex == -1)
		shouldFallback = true;

	if (shouldFallback)
	{
		static Vector offset = Vector(0, 0, -20);
		*outPos = entity->EyePosition() + offset;
		return false;
	}

	*outPos = Vector(bones[boneIndex][0][3], bones[boneIndex][1][3], bones[boneIndex][2][3]);
	return true;
}

// dynamic fov tings
float getFOVToCompare(float dist)
{
	if (dist < maxCQDist)
	{
		float t = 1 - (dist / maxCQDist);
		return maxFOV + maxCQFOV * t;
	}
	else
		return maxFOV;
}

bool findAimBonePos(C_BasePlayer* entity, Vector eyePos, QAngle eyeAngles, float maxFOV, /* out: */ float* returnedFOV, Vector* returnedBonePos)
{
	bool foundABone = false;

	float bestFOV = maxFOV;
	Vector bestBonePos;

	for (size_t i = 0; i < size; i++)
	{
		Vector bonePos;

		getBonePosByName(entity, possBones[i], &bonePos);

		if (!utils::CanSeePlayer(entity, bonePos))
			continue;

		auto deltaAngle = ((bonePos - eyePos).toAngle() - eyeAngles).FixAngles();
		float compareFOV = std::hypot(deltaAngle.x, deltaAngle.y);

		if (compareFOV < bestFOV)
		{
			bestFOV = compareFOV;
			bestBonePos = bonePos;
			foundABone = true;
		}
	}

	if (foundABone)
	{
		*returnedFOV = bestFOV;
		*returnedBonePos = bestBonePos;
		return true;
	}

	return false;
}

bool findBestAimPos(Vector eyePos, QAngle eyeAngles, /* out: */ C_BasePlayer** returnedTarget, float* returnedFOV, float* returnedMaxFOV, Vector* returnedPos)
{
	bool foundTarget = false;

	float bestFOV = 180;
	float bestTargetMaxFOV = 180;
	Vector bestAimPos;
	C_BasePlayer* bestTarget = nullptr;


	for (int i = 0; i < 64; i++)
	{
		C_BasePlayer* entity = (C_BasePlayer*)interfaces::entityList->GetClientEntity(i);
		if (!entity || entity == g::localPlayer || !entity->IsValidTarget())
			continue;

		float compareFOV = 180;
		Vector comparePos;

		float dynFOV = getFOVToCompare((entity->GetAbsOrigin() - eyePos).Length());

		if (!findAimBonePos(entity, eyePos, eyeAngles, dynFOV, &compareFOV, &comparePos))
			continue;

		if (compareFOV < bestFOV)
		{
			bestFOV = compareFOV;
			bestAimPos = comparePos;
			bestTargetMaxFOV = dynFOV;
			bestTarget = entity;

			foundTarget = true;
		}
	}

	if (foundTarget)
	{
		*returnedFOV = bestFOV;
		*returnedPos = bestAimPos;
		*returnedMaxFOV = bestTargetMaxFOV;
		*returnedTarget = bestTarget;

		return true;
	}

	return false;
}

C_BasePlayer* currentTarget = nullptr;

bool waitForKeyRelease = false;

void aimbot::run(CUserCmd* cmd)
{
	// if we're not holding the aimbot key, don't aimbot. We also should not keep aimbotting if our target is not valid anymore
	if (!GetAsyncKeyState(VK_LBUTTON) || !interfaces::matSystemSurface->IsCursorLocked() || !(cmd->buttons & IN_ATTACK))
	{
		currentTarget = nullptr;

		// key released, so we should not wait for key release anymore
		waitForKeyRelease = false;
		return;
	}

	bool shouldFindNew = false;

	if (GetAsyncKeyState(VK_XBUTTON2))
	{
		shouldFindNew = true;
		waitForKeyRelease = false;
	}

	// waiting to let go of key and click again
	if (waitForKeyRelease)
		return;

	Vector eyePos = g::localPlayer->EyePosition();
	QAngle eyeAngles = cmd->viewangles;

	float bestFOV = 180;
	float bestMaxFOV = 180;
	Vector bestAimPos;

	// since we clicked, we havent found a target yet
	if (!currentTarget)
	{
		if (!findBestAimPos(eyePos, eyeAngles, &currentTarget, &bestFOV, &bestMaxFOV, &bestAimPos))
		{
			return;
		}
	}
	else
	{
		bestMaxFOV = getFOVToCompare((currentTarget->GetAbsOrigin() - eyePos).Length());
		if (!currentTarget->IsValidTarget())
		{
			currentTarget = nullptr;
			waitForKeyRelease = true;
			return;
		}

		if (!findAimBonePos(currentTarget, eyePos, eyeAngles, 360, &bestFOV, &bestAimPos) || bestFOV > bestMaxFOV)
		{
			if (shouldFindNew)
				currentTarget = nullptr;
			return;
		}
	}

	// we passed conditions, so we should aimbot

	QAngle deltaAngle = (bestAimPos - eyePos).toAngle() - eyeAngles;
	deltaAngle.FixAngles();

	float deltaX = 0;
	float deltaY = 0;

	if (!deltaFromAimPos(bestAimPos, &deltaX, &deltaY))
		return;

	// smooth aimbot
	static auto easePitch = easing::getEasingFunction(easing::EasingFunctions::inOutQuint);
	static auto easeYaw = easing::getEasingFunction(easing::EasingFunctions::inOutQuint);

	float easedPitch = pitchStrength * easePitch(1 - bestFOV / bestMaxFOV);
	float easedYaw = yawStrength * easeYaw(1 - bestFOV / bestMaxFOV);

	deltaX *= easedPitch;
	deltaY *= easedYaw;

	moveMouse(deltaX, deltaY);
}