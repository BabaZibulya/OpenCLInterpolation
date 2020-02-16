#include "BMPImage.h"

#include <iostream>
#include <fstream>
#include <array>
#include <cstring>

namespace utils {
    struct BMPImageImpl {
        unsigned width, height;
        std::unique_ptr<unsigned char[]> buff;
    };
    
    BMPImage::BMPImage(const std::string& path) : impl(std::unique_ptr<BMPImageImpl>(new BMPImageImpl)) {
        auto file = std::ifstream(path, std::ios::binary);
        file.read((char *) &header, 54);

        // extract image height and width from header
        impl->width = *reinterpret_cast<uint32_t *>(&header[18]);
        impl->height = *reinterpret_cast<uint32_t *>(&header[22]);
        auto depth = *reinterpret_cast<uint16_t *>(&header[28]);

        int size = 3 * impl->width * impl->height;
        impl->buff = std::unique_ptr<unsigned char[]>(new unsigned char[size]); // allocate 3 bytes per pixel
        file.read((char *)impl->buff.get(), size);

        file.close();
    }

    BMPImage::BMPImage(const BMPImage& other) : impl(std::unique_ptr<BMPImageImpl>(new BMPImageImpl)) {
        impl->width = other.getWidth();
        impl->height = other.getHeight();
        int size = 3 * impl->width * impl->height;
        impl->buff = std::unique_ptr<unsigned char[]>(new unsigned char[size]);
        std::memcpy(impl->buff.get(), other.getBuffer().get(), size);
        std::memcpy(&header[0], &other.header[0], 54);
    }

    BMPImage& BMPImage::operator=(const BMPImage& other) {
        if (this == &other) {
            return *this;
        }
        impl->width = other.getWidth();
        impl->height = other.getHeight();
        header = other.header;
        int size = 3 * impl->width * impl->height;
        impl->buff = std::unique_ptr<unsigned char[]>(new unsigned char[size]);
        std::memcpy(impl->buff.get(), other.getBuffer().get(), size);
        return *this;
    }

    unsigned BMPImage::getWidth() const {
        return impl->width;
    }

    unsigned BMPImage::getHeight() const {
        return impl->height;
    }

    const std::unique_ptr<unsigned char[]>& BMPImage::getBuffer() const {
        return impl->buff;
    }

    void BMPImage::save(const std::string& path) {
        auto file = std::ofstream(path, std::ios::binary);
        file.write((char *)&header, 54);

        int size = 3 * impl->width * impl->height;
        file.write((char *)impl->buff.get(), size);

        file.close();
    }

    BMPImage::~BMPImage() {

    }
}

