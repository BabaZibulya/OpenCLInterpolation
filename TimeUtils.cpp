#include "TimeUtils.h"

#include <chrono>
#include <iostream>

void measureTime(const std::string& message, const std::function<void()>& f)
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    f();

    end = std::chrono::system_clock::now();
    int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
 
    std::cout << message << " - execution time: " << elapsed_milliseconds << "ms\n";
}