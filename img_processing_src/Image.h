#pragma once

#include <Magick++.h>
#include <iostream>
// #include <gdkmm.h>

class Image {
private:
    Magick::Image image;

public:
    Image();

    void load(std::string file_name);
    void rotate(double degrees) {}
    void resize(size_t width, size_t height);
    void save(std::string file_name);

    Magick::Image& getImage();
};
