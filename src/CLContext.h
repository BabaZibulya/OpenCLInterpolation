#pragma once

#include <vector>

#include "CLDevice.h"
#include "CLPlatform.h"

class CLContext {
public:
    using ErrorCallback = void (*)(const char*, const void*, size_t cb, void* user_data);
    CLContext(CLContext&& context);
    CLContext(const CLPlatform& platform, const std::vector<CLDevice>& devices);
    CLContext(const CLPlatform& platform, const std::vector<CLDevice>& device, ErrorCallback errorCallback);
    CLContext(const CLContext&) = delete;
    CLContext& operator=(const CLContext&) = delete;
    ~CLContext();
public:
    cl_context clContext;
};
