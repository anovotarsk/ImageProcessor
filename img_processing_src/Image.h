#pragma once

#include <Magick++.h>
#include <iostream>
#include <string>
#include <cmath>

class Image {
private:
    Magick::Image original_image;
    Magick::Image processed_image;

    size_t new_width;
    size_t new_height;
    double rotation_degrees = 0;

public:
    Image() = default;

    void load(std::string file_name);
    void rotate(double degrees);
    void resize(size_t width, size_t height);
    void save(std::string file_name);
    bool isValid();

    Magick::Image& getProcessedImage();
};
