#include "interfaces.h"
#include "../../memory/memory.h"

void interfaces::setup()
{
	client = memory::getInterface<void>("client.dll", "VClient017");
	printf("[0x%p] client \n", client);

	engineClient = memory::getInterface<CEngineClient>("engine.dll", "VEngineClient015");
	printf("[0x%p] engineClient \n", engineClient);

	entityList = memory::getInterface<CClientEntityList>("client.dll", "VClientEntityList003");
	printf("[0x%p] entityList \n", entityList);

	clientMode = memory::getVMT<void>(reinterpret_cast<uintptr_t>(client), 10, 0x0);
	printf("[0x%p] clientMode \n", clientMode);
}
