#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <iostream>

const char *clErrorString(int errorCode);

#ifdef CL_DEBUG
#define CL(command) \
    {\
        auto result = command;\
        if (result != CL_SUCCESS) {\
            std::cout << "OpenCL error : " << clErrorString(result) << '\n';\
        }\
    }
#else
#define CL(command) command
#endif
