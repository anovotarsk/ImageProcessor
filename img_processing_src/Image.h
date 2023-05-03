#pragma once

#include <Magick++.h>
#include <iostream>
#include <string>
#include <cmath>

class Image {
private:
    Magick::Image original_image; // Magick++ Image object for the original image
    Magick::Image processed_image; // Magick++ Image object for the processed image

    size_t new_width; // Width of the new image after resizing
    size_t new_height; // Height of the new image after resizing
    double rotation_degrees = 0; // Rotation angle of the image

    bool need_to_reprocess = false; // Flag indicating whether the image needs to be reprocessed

public:
    Image() = default;

    void load(std::string file_name);
    void rotate(double degrees);
    void resize(size_t width, size_t height);
    void save(std::string file_name);
    bool isValid();

    void reprocessForce(); // Force the image to be reprocessed
    Magick::Image& getProcessedImage(); // Get the processed image
};
