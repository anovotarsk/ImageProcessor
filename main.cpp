#include <Magick++.h>
#include <iostream>
#include <cstring>
#include <gtkmm.h>

#include "img_processing_src/Image.h"
#include "gui_src/GUI.h"


int main(int argc, char *argv[]) {
    Magick::InitializeMagick(*argv);
    Gtk::Main kit(argc, argv);

    // initialize builder
    init_builder(UI_FILE_LOCATION);

    // Get the window object from the UI file
    Gtk::Window* window;
    builder->get_widget(MAIN_WINDOW_ID, window);
    if (!window) {
        std::cerr << "Error: Could not get window widget from UI file" << std::endl;
        return 1;
    }

    // Show the window and start the Gtk event loop
    window->show_all();
    Gtk::Main::run(*window);
    return 0;
}
