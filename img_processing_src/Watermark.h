#pragma once

#include <Magick++.h>
#include <string>

class Watermark {
private:
    std::string text;
    size_t font_size;
    double rotation_degrees;

public:
    void addWatermark(Magick::Image &image);
};