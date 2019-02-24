#pragma once

#include <string>
#include <functional>

void measureTime(const std::string& message, const std::function<void()>& f);