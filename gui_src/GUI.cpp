#include "GUI.h"

Glib::RefPtr<Gtk::Builder> builder;
std::shared_ptr<Image> image;

void init_builder(std::string ui_file) {
    image = std::make_shared<Image>();
    image->load("input.png");

    builder = Gtk::Builder::create();

    try {
        // Load the UI file
        builder->add_from_file("ui.glade");
    } catch (const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        exit(0);
    } catch (const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        exit(0);
    } catch (const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        exit(0);
    }

    connect_all();
}

void connect_all() {
    Gtk::Entry *width_entry;
    Gtk::Entry *height_entry;
    Gtk::Button *resize_button;
    Gtk::Scale *rotate_scale;

    builder->get_widget("width_entry", width_entry);
    builder->get_widget("height_entry", height_entry);
    builder->get_widget("resize_button", resize_button);
    builder->get_widget("rotate_scale", rotate_scale);

    if (!width_entry || !height_entry || !resize_button || !rotate_scale) {
        std::cerr << "Builder::connect_all: Builder could not get widgets from UI file" << std::endl;
        exit(0);
    }
    width_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_text_entry_for_resize_changed), width_entry)
        );
    height_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_text_entry_for_resize_changed), height_entry)
        );

    resize_button->signal_clicked().connect(sigc::ptr_fun(&on_resize_button_clicked));
    rotate_scale->signal_value_changed().connect(sigc::ptr_fun(&on_rotate_scale_value_changed));
}

void on_text_entry_for_resize_changed(Gtk::Entry* entry) {
    std::string text = entry->get_text();
    // std::cout << text << std::endl;

    for (auto it = text.begin(); it != text.end(); ) {
        if (!std::isdigit(*it))
            it = text.erase(it);
        else
            it++;
    }

    entry->set_text(text);
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

    if (width_entry->get_text().length() == 0 || height_entry->get_text().length() == 0) {
        return;
    }

    size_t new_width = std::stoi(width_entry->get_text().c_str());
    size_t new_height = std::stoi(height_entry->get_text().c_str());

    image->resize(new_width, new_height);

    Magick::Image& raw_image(image->getProcessedImage());
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

    // pBuff->add_alpha(true, 0xff, 0xff, 0xff);

    image_widget->clear();
    image_widget->set(pBuff);

    // delete[] buffer;
}

void on_rotate_scale_value_changed() {
    Gtk::Scale *rotate_scale;
    
    builder->get_widget("rotate_scale", rotate_scale);

    if (!rotate_scale) {
        std::cerr << "on_rotate_scale_value_changed()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    image->rotate(rotate_scale->get_value());

    Magick::Image& raw_image(image->getProcessedImage());
    size_t to_allocate = raw_image.columns() * raw_image.rows() * 3;
    guint8 * buffer = new guint8[to_allocate];
    raw_image.write(0, 0, raw_image.columns(), raw_image.rows(), "RGB", Magick::CharPixel, buffer);
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_data(
            buffer,
            Gdk::COLORSPACE_RGB,
            false,
            8,
            raw_image.columns(),
            raw_image.rows(),
            raw_image.columns()*3
        );
    // pixbuf->add_alpha(true, 0xff, 0xff, 0xff);

    Gtk::Image *image_widget;

    builder->get_widget("image", image_widget);

    if (!image_widget) {
        std::cerr << "on_resize_button_clicked()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    image_widget->clear();
    image_widget->set(pixbuf);


    // delete[] buffer;
}
