#pragma once

#include <string>
#include <functional>

#define MEASURE_TIME(message, FUNC) \
    { \
        std::chrono::time_point<std::chrono::system_clock> start, end; \
        start = std::chrono::system_clock::now(); \
        FUNC; \
        end = std::chrono::system_clock::now(); \
        int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds> \
                                (end-start).count(); \
        std::cout << message << " - execution time: " << elapsed_milliseconds << "ms\n"; \
    }

void measureTime(const std::string& message, const std::function<void()>& f);