#include "Image.h"

void Image::load(std::string file_name) {
    original_image.read(file_name);
    processed_image = original_image;
}

void Image::resize(size_t width, size_t height) {
    if (!isValid()) {
        return;
    }

    new_width = width;
    new_height = height;
}

void Image::rotate(double degrees) {
    if (isValid())
        rotation_degrees = degrees;
}

void Image::save(std::string file_name) {
    if (!isValid()) {
        return;
    }

    getProcessedImage();
    processed_image.write(file_name);
}

Magick::Image& Image::getProcessedImage() {
    if (!isValid()) {
        return processed_image;
    }

    Magick::Geometry new_geometry(new_width, new_height, 0, 0);

    new_geometry.aspect(true);

    processed_image = original_image;
    processed_image.resize(new_geometry);

    if (std::fmod(rotation_degrees, 90) != 0.0) {
        processed_image.borderColor("transparent");
        processed_image.border(Magick::Geometry(1, 1));
    }

    double theta = rotation_degrees * M_PI / 180.0;
    double arguments[] = {cos(theta), -sin(theta), sin(theta), cos(theta), 0, 0};
    processed_image.distort(MagickCore::AffineProjectionDistortion, 6, arguments, true);


    processed_image.write("output.png");
    return processed_image;
}

bool Image::isValid() {
    return original_image.isValid();
}

