#include "CLLog.h"

void CLLog(const std::string& message) {
#ifdef CL_DEBUG
    std::cout << message << '\n';
#endif
}

void CLLog() {
#ifdef CL_DEBUG
    std::cout << '\n';
#endif
}