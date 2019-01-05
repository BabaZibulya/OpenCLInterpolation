#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <iostream>
#include <exception>

#include "CLLog.h"

// Utility function and macros
const char *clErrorString(int errorCode);
void checkForCLError(int error);
void logError(int error);

#ifdef CL_DEBUG
#define CL(command) \
    if (auto result = command; result != CL_SUCCESS) {\
        logError(result);\
    }
#else
#define CL(command) command
#endif
