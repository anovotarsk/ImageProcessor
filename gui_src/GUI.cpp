#include "GUI.h"

Glib::RefPtr<Gtk::Builder> builder;
std::shared_ptr<Image> image;

void init_builder(std::string ui_file) {
    image = std::make_shared<Image>();
    image->load("input.png");

    builder = Gtk::Builder::create();

    try {
        // Load the UI file
        builder->add_from_file(UI_FILE_LOCATION);
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

    builder->get_widget(IMG_WIDTH_ENTRY_ID, width_entry);
    builder->get_widget(IMG_HEIGHT_ENTRY_ID, height_entry);
    builder->get_widget(IMG_RESIZE_BUTTON_ID, resize_button);
    builder->get_widget(IMG_ROTATE_SCALE_ID, rotate_scale);

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

void update_image_widget() {
    Gtk::Image *image_widget;
    guint8 * buffer;
    size_t buffer_size;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;

    Magick::Image& raw_image(image->getProcessedImage());

    builder->get_widget(IMAGE_WIDGET_ID, image_widget);

    if (!image_widget) {
        std::cerr << "update_image_widget()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    buffer_size = raw_image.columns() * raw_image.rows() * 3;
    buffer = new guint8[buffer_size];
    raw_image.write(0, 0, raw_image.columns(), raw_image.rows(), "RGB", Magick::CharPixel, buffer);

    pixbuf = Gdk::Pixbuf::create_from_data(
            buffer,
            Gdk::COLORSPACE_RGB,
            false,
            8,
            raw_image.columns(),
            raw_image.rows(),
            raw_image.columns()*3
        );

    image_widget->clear();
    image_widget->set(pixbuf);
}

void on_text_entry_for_resize_changed(Gtk::Entry* entry) {
    std::string text = entry->get_text();

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

    builder->get_widget(IMG_WIDTH_ENTRY_ID, width_entry);
    builder->get_widget(IMG_HEIGHT_ENTRY_ID, height_entry);

    if (!width_entry || !height_entry) {
        std::cerr << "on_resize_button_clicked()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    if (width_entry->get_text().length() == 0 || height_entry->get_text().length() == 0) {
        return;
    }

    size_t new_width = std::stoi(width_entry->get_text().c_str());
    size_t new_height = std::stoi(height_entry->get_text().c_str());

    image->resize(new_width, new_height);

    update_image_widget();
}

void on_rotate_scale_value_changed() {
    Gtk::Scale *rotate_scale;
    
    builder->get_widget(IMG_ROTATE_SCALE_ID, rotate_scale);

    if (!rotate_scale) {
        std::cerr << "on_rotate_scale_value_changed()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    image->rotate(rotate_scale->get_value());

    update_image_widget();
}
