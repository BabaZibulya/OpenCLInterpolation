#pragma once

#include <vector>

#include "CLDevice.h"

class CLContext {
public:
    using ErrorCallback = void(const char*, const void*, size_t cb, void* user_data);
    CLContext(const std::vector<CLDevice>& devices);
    CLContext(const std::vector<CLDevice>& device, const ErrorCallback& errorCallback);
    ~CLContext();
public:
    cl_context clContext;
};
