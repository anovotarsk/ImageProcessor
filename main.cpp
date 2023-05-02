#include <Magick++.h>
#include <iostream>
#include <cstring>
#include <gtkmm.h>

#include "img_processing_src/Image.h"
#include "gui_src/GUI.h"


int main(int argc, char *argv[]) {
    Magick::InitializeMagick(*argv);
    Gtk::Main kit(argc, argv);

    init_builder("ui.glade");

    // Get the window object from the UI file
    Gtk::Window* window;
    builder->get_widget("main_window", window);
    if (!window) {
        std::cerr << "Error: Could not get window widget from UI file" << std::endl;
        return 1;
    }

    // Show the window and start the Gtk event loop
    window->show_all();
    Gtk::Main::run(*window);
    return 0;
}


// #include <Magick++.h>
// #include <Magick++/Drawable.h>
// #include <iostream>

// using namespace std;
// using namespace Magick;

// int main(int argc, char **argv)
// {
//     Magick::InitializeMagick(*argv);
//     // Создание изображения
//     Magick::Image image(Geometry(200, 200), Magick::Color("#00000000")); // Прозрачный фон

//     // Настройка шрифта и размера текста
//     DrawableText text(50, 100, "Hello World!");
    
//     image.font("Helvetica");
//     image.fontPointsize(25);
//     image.fillColor(ColorRGB(1.0, 0.0, 0.0));
//     // image.fontTypeMetrics() fillOpacity(0.5);

//     image.draw(text);
//     // image.rotate(45);

//     double theta = 45 * M_PI / 180.0;
//     double arguments[] = {cos(theta), -sin(theta), sin(theta), cos(theta), 0, 0};
//     image.distort(MagickCore::AffineProjectionDistortion, 6, arguments, true);

//     // Сохранение изображения в файл
//     image.write("output.png");

//     return 0;
// }



