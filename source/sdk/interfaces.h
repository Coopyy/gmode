#pragma once

#ifndef INTERFACES_H 
#define INTERFACES_H

#include "../memory.h"

#include "../sdk.h"

namespace interfaces
{
	inline CHLClient* client;
	inline CClientEntityList* entityList;
	inline CEngineClient* engineClient;

	inline IEngineTrace* engineTrace;

	inline ClientModeShared* clientMode;

	inline CGlobalVarsBase* globalVars;

	inline CMatSystemSurface* matSystemSurface;

	void setup();
}
#endif // !INTERFACES_H 
