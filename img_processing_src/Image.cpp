#include "Image.h"

void Image::load(std::string file_name) {
    original_image.read(file_name);
    third_changed_image = original_image;
    processed_image = third_changed_image;
    need_to_reprocess = true;
}

void Image::resize(size_t width, size_t height) {
    if (!isValid()) {
        return;
    }

    new_width = width;
    new_height = height;
    need_to_reprocess = true;
}

void Image::rotate(double degrees) {
    if (!isValid())
        return;

    rotation_degrees = degrees;
    need_to_reprocess = true;
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

    if (need_to_reprocess == false)
        return processed_image;
    
    need_to_reprocess = false;

    Magick::Geometry new_geometry(new_width, new_height, 0, 0);

    new_geometry.aspect(true);
    // processed_image = third_changed_image;
    processed_image = original_image;
    processed_image.resize(new_geometry);

    if (std::fmod(rotation_degrees, 90) != 0.0) {
        processed_image.borderColor("transparent");
        processed_image.border(Magick::Geometry(1, 1));
    }

    double theta = rotation_degrees * M_PI / 180.0;
    double arguments[] = {cos(theta), -sin(theta), sin(theta), cos(theta), 0, 0};
    processed_image.distort(MagickCore::AffineProjectionDistortion, 6, arguments, true);

    return processed_image;
}

bool Image::isValid() {
    return original_image.isValid();
}

void Image::reprocessForce() {
    need_to_reprocess = true;
}

void Image::clearThirdChangedImage() {
    third_changed_image = original_image;
}

Magick::Image& Image::getThirdChangedImage() {
    return third_changed_image;
}
