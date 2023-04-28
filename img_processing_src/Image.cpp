#include "Image.h"

Image::Image() {
}

void Image::load(std::string file_name) {
    image.read(file_name);
}

void Image::resize(size_t width, size_t height) {
    size_t image_width = image.columns();
    size_t image_height = image.rows();

    image.resize(Magick::Geometry(width, height));


}

void Image::save(std::string file_name) {
    image.write(file_name);
}

Magick::Image& Image::getImage() {
    return image;
}
