#pragma once

#include "BMPImage.h"

class Convolution {
public:
    Convolution(const std::string &path);
    void solve();
private:
    void solveCpu();

    utils::BMPImage image;
};
