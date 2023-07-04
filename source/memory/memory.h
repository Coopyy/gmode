#include "../includes.h"

#ifndef MEMORY_H
#define MEMORY_H

namespace memory
{
	template<typename T>
	T* getVMT(uintptr_t address, int index, uintptr_t offset) // Address must be a VTable pointer, not a VTable !
	{
#ifdef _WIN64
		uintptr_t step = 3;
		uintptr_t instructionSize = 7;
		uintptr_t instruction = ((*(uintptr_t**)(address))[index] + offset);

		uintptr_t relativeAddress = *(DWORD*)(instruction + step);
		uintptr_t realAddress = instruction + instructionSize + relativeAddress;
		return *(T**)(realAddress);
#else
		uintptr_t instruction = ((*(uintptr_t**)(address))[index] + offset);
		return *(T**)(*(uintptr_t*)(instruction));
#endif
	}

	template <typename Interface>
	Interface* getInterface(const char* moduleName, const char* interfaceName)
	{
		const HINSTANCE handle = GetModuleHandle(moduleName);

		if (!handle)
			return nullptr;

		// get the exported Createinterface function
		using CreateInterfaceFn = Interface * (__cdecl*)(const char*, int*);
		const CreateInterfaceFn createInterface =
			reinterpret_cast<CreateInterfaceFn>(GetProcAddress(handle, "CreateInterface"));

		// return the interface pointer by calling the function
		return createInterface(interfaceName, nullptr);
	}

	constexpr void* get(void* vmt, const std::uint32_t index)
	{
		return (*static_cast<void***>(vmt))[index];
	}

	template <typename Return, typename ... Arguments>
	constexpr Return call(void* vmt, const std::uint32_t index, Arguments ... args) noexcept
	{
		using Function = Return(__thiscall*)(void*, decltype(args)...);
		return (*static_cast<Function**>(vmt))[index](vmt, args...);
	}
}
#endif // !MEMORY_H
