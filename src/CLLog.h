#pragma once

#include <string>
#include <iostream>

void CLLog();

template<typename first, typename ...rest>
void CLLog(first f, rest... r) {
#ifdef CL_DEBUG
    std::cout << f;
    CLLog(r...);
#endif
}
