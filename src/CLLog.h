#pragma once

#include <string>
#include <iostream>

void CLLog();

template<typename first, typename ...rest>
void CLLog(first f, rest... r) {
    std::cout << f;
    CLLog(r...);
}
