#pragma once

#include <Magick++.h>
#include <string>

class Watermark {
private:
    std::string text;
    size_t font_size;
    double rotation_degrees;
    double transparency;
    Magick::Color color;

public:
    void addWatermark(Magick::Image &image);
};
