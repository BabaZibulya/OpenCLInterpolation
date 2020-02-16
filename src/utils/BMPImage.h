#pragma once

#include <memory>
#include <string>
#include <array>

namespace utils {
    struct BMPImageImpl;
    class BMPImage {
    public:
        BMPImage(const std::string& path);
        BMPImage(const BMPImage& other);
        BMPImage& operator=(const BMPImage& other);

        unsigned getWidth() const;
        unsigned getHeight() const;

        void save(const std::string& path);

        ~BMPImage();

        const std::unique_ptr<unsigned char[]>& getBuffer() const;
    private:
        std::array<unsigned char, 54> header;
        std::unique_ptr<BMPImageImpl> impl;
    };
}