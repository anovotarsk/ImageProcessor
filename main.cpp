#include <Magick++.h>
#include <iostream>
#include <cstring>
#include <gtkmm.h>

#include "img_processing_src/Image.h"
#include "gui_src/Builder.h"

Glib::RefPtr<Gtk::Builder> builder;
std::shared_ptr<Image> image;

void on_entry_changed()
{
    std::cout << "Entry text has changed." << std::endl;
}

void on_resize_button_clicked() {
    Gtk::Entry *width_entry;
    Gtk::Entry *height_entry;
    Gtk::Image *image_widget;

    builder->get_widget("width_entry", width_entry);
    builder->get_widget("height_entry", height_entry);
    builder->get_widget("image", image_widget);

    if (!width_entry || !height_entry || !image_widget) {
        std::cerr << "on_resize_button_clicked()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    size_t new_width = std::stoi(width_entry->get_text().c_str());
    size_t new_height = std::stoi(height_entry->get_text().c_str());

    image->resize(new_width, new_height);

    Magick::Image& raw_image(image->getImage());
    size_t to_allocate = raw_image.columns() * raw_image.rows() * 3;
    guint8 * buffer = new guint8[to_allocate];
    raw_image.write(0, 0, raw_image.columns(), raw_image.rows(), "RGB", Magick::CharPixel, buffer);
    Glib::RefPtr<Gdk::Pixbuf> pBuff = Gdk::Pixbuf::create_from_data(
            buffer,
            Gdk::COLORSPACE_RGB,
            false,
            8,
            raw_image.columns(),
            raw_image.rows(),
            raw_image.columns()*3
        );

    image_widget->clear();
    image_widget->set(pBuff);

    delete[] buffer;
}

int main(int argc, char *argv[]) {
    Magick::InitializeMagick(*argv);
    image = std::make_shared<Image>();
    image->load("input.png");
    Gtk::Main kit(argc, argv);
    builder = Gtk::Builder::create();

    try {
        // Load the UI file
        builder->add_from_file("ui.glade");
    } catch (const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    } catch (const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    } catch (const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    // Get the window object from the UI file
    Gtk::Window* window;
    builder->get_widget("main_window", window);
    if (!window) {
        std::cerr << "Error: Could not get window widget from UI file" << std::endl;
        return 1;
    }

    Gtk::Button *resize_button;
    Gtk::Entry *width_entry;
    builder->get_widget("resize_button", resize_button);
    builder->get_widget("width_entry", width_entry);
    if (!resize_button) {
        std::cerr << "Error: Could not get widgets from UI file" << std::endl;
        return 1;
    }
    resize_button->signal_clicked().connect(sigc::ptr_fun(&on_resize_button_clicked));
    width_entry->signal_changed().connect(sigc::ptr_fun(&on_entry_changed));

    // Show the window and start the Gtk event loop
    window->show_all();
    Gtk::Main::run(*window);
    return 0;


}