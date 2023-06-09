#include "Watermark.h"

Watermark::Watermark(std::string& text_, size_t font_size_, double rotation_degrees_,
                     std::string color_, size_t x, size_t y)
    : text(text_), font_size(font_size_), rotation_degrees(rotation_degrees_),
      color(color_), x_location(x), y_location(y) {
}

void Watermark::addWatermark(Magick::Image& image) {
    // Check if the image is valid
    if (!image.isValid())
        return;

    // Calculate the metrics for the text
    Magick::TypeMetric metrics;
    image.fontPointsize(font_size);
    image.fontTypeMetrics(text, &metrics);

    // Create a new image for the watermark
    Magick::Geometry geometry(metrics.textWidth() + 1, metrics.textHeight() + 1);
    Magick::Image watermark(geometry, "transparent");

    // Add the text to the watermark
    watermark.fontPointsize(font_size);
    watermark.fillColor(color);
    watermark.annotate(text, Magick::GravityType::CenterGravity);

    // Add a transparent border if the rotation is not a multiple of 90 degrees
    if (std::fmod(rotation_degrees, 90) != 0.0) {
        watermark.borderColor("transparent");
        watermark.border(Magick::Geometry(1, 1));
    }

    // Rotate the watermark
    double theta = rotation_degrees * M_PI / 180.0;
    double arguments[] = {cos(theta), sin(theta), -sin(theta), cos(theta), 0, 0};
    watermark.distort(MagickCore::AffineProjectionDistortion, 6, arguments, true);

    // Add the watermark to the image
    image.composite(watermark, Magick::Geometry(0, 0, x_location, y_location), Magick::CompositeOperator::OverCompositeOp);
}
