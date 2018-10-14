#include <vector>
#include <string>
#include <iostream>

#include "CLPlatform.h"

#include "CL/cl.h"

int main() {
    auto platforms = CLPlatform::getAllAvailableCLPlatforms();
    for (auto platform : platforms) {
        std::cout << platform.platformInfo << '\n';
    }
    return 0;
}
