#include "interfaces.h"

void interfaces::setup()
{
	client = memory::getInterface<CHLClient>("client.dll", "VClient017");
	printf("[0x%p] client \n", client);

	engineClient = memory::getInterface<CEngineClient>("engine.dll", "VEngineClient015");
	printf("[0x%p] engineClient \n", engineClient);

	entityList = memory::getInterface<CClientEntityList>("client.dll", "VClientEntityList003");
	printf("[0x%p] entityList \n", entityList);

	engineTrace = memory::getInterface<IEngineTrace>("engine.dll", "EngineTraceClient003");
	printf("[0x%p] engineTrace \n", engineTrace);

	clientMode = memory::getVMT<ClientModeShared>(reinterpret_cast<uintptr_t>(client), 10, 0x0);
	printf("[0x%p] clientMode \n", clientMode);

	globalVars = memory::getVMT<CGlobalVarsBase>(reinterpret_cast<uintptr_t>(client), 0, 0x94);
	printf("[0x%p] globalVars \n", globalVars);

	matSystemSurface = memory::getInterface<CMatSystemSurface>("vguimatsurface.dll", "VGUI_Surface030");
	printf("[0x%p] matSystemSurface \n", matSystemSurface);
}