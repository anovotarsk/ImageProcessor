#pragma once

#include <Magick++.h>
#include <string>
#include <cmath>

class Watermark {
private:
    std::string text;
    size_t font_size;
    double rotation_degrees;
    Magick::Color color;
    size_t x_location;
    size_t y_location;

public:
    Watermark(std::string &text_, size_t font_size_,
              double rotation_degrees_, std::string color,
              size_t x, size_t y);
              
    void addWatermark(Magick::Image &image);
};
