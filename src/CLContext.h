#pragma once

#include <vector>

#include "CLDevice.h"
#include "CLPlatform.h"

namespace CL {
	class Context {
	public:
#ifdef _WIN32
		using ContextErrorCallback = void(__stdcall*)(const char*, const void*, size_t cb, void* user_data);
#else
		using ContextErrorCallback = void(*)(const char*, const void*, size_t cb, void* user_data);
#endif
		Context(Context&& context);
		Context(const Platform& platform, const std::vector<Device>& devices);
		Context(const Platform& platform, const std::vector<Device>& device, ContextErrorCallback errorCallback);
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;
		~Context();
	public:
		cl_context clHandle;
	};
}