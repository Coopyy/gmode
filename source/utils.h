#pragma once
#include "sdk.h"
#include "sdk/interfaces.h"
#include "globals.h"

#include "valve/engine/trace.h"
#include "valve/engine/gametrace.h"

namespace utils
{
	inline bool CanSeePlayer(C_BasePlayer* player, const Vector& pos)
	{
		CTraceFilter filter;
		filter.pSkip = g::localPlayer;

		auto start = g::localPlayer->EyePosition();

		Ray_t ray;
		ray.Init(start, pos);

		trace_t tr;
		interfaces::engineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);
		return tr.m_pEnt == player || tr.fraction > 0.97f;
	}
}