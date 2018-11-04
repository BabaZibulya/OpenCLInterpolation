#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <iostream>
#include <exception>

// Utility function and macros
const char *clErrorString(int errorCode);
void checkForCLError(int error);

#ifdef CL_DEBUG
#define CL(command) \
    if (auto result = command; result != CL_SUCCESS) {\
        std::cout << "OpenCL error : " << clErrorString(result) << " " << __FILE__ << " " << __LINE__ << '\n';\
    }
#else
#define CL(command) command
#endif
