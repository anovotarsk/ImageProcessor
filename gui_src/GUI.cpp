#include "GUI.h"


// global variables definition
Glib::RefPtr<Gtk::Builder> builder;
std::shared_ptr<Image> image;


void init_builder(std::string ui_file) {
    image = std::make_shared<Image>();

    builder = Gtk::Builder::create();

    try {
        // Load the UI file
        builder->add_from_file(ui_file);
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

    // connect all signal handlers to their respective widgets
    connect_all();
}

void connect_all() {
    Gtk::Button *open_button;
    Gtk::Button *save_button;
    Gtk::Entry *width_entry;
    Gtk::Entry *height_entry;
    Gtk::Button *resize_button;
    Gtk::Scale *rotate_scale;
    Gtk::Switch *watermark_switch;
    Gtk::Entry *watermark_text_entry;
    Gtk::Scale *watermark_rotate_scale;
    Gtk::ColorButton *watermark_color_button;
    Gtk::Entry *font_size_entry;
    Gtk::Entry *x_location_entry;
    Gtk::Entry *y_location_entry;

    // get widgets from builder
    builder->get_widget(OPEN_BUTTON_ID, open_button);
    builder->get_widget(SAVE_BUTTON_ID, save_button);
    builder->get_widget(IMG_WIDTH_ENTRY_ID, width_entry);
    builder->get_widget(IMG_HEIGHT_ENTRY_ID, height_entry);
    builder->get_widget(IMG_RESIZE_BUTTON_ID, resize_button);
    builder->get_widget(IMG_ROTATE_SCALE_ID, rotate_scale);
    builder->get_widget(WTRMARK_SWITCH_ID, watermark_switch);
    builder->get_widget(WTRMARK_ENTRY_ID, watermark_text_entry);
    builder->get_widget(WTRMARK_ROTATE_SCALE_ID, watermark_rotate_scale);
    builder->get_widget(WTRMARK_COLOR_ID, watermark_color_button);
    builder->get_widget(WTRMARK_FONT_SIZE_ID, font_size_entry);
    builder->get_widget(WTRMARK_X_ENTRY_ID, x_location_entry);
    builder->get_widget(WTRMARK_Y_ENTRY_ID, y_location_entry);

    // check if every widget valid
    if (!open_button ||
        !save_button ||
        !width_entry ||
        !height_entry ||
        !resize_button ||
        !rotate_scale ||
        !watermark_switch ||
        !watermark_text_entry ||
        !watermark_rotate_scale ||
        !watermark_color_button ||
        !font_size_entry ||
        !x_location_entry ||
        !y_location_entry)
    {
        std::cerr << "Builder::connect_all: Builder could not get widgets from UI file" << std::endl;
        exit(0);
    }

    // connect signal handlers
    open_button->signal_clicked().connect(sigc::ptr_fun(&on_open_button_clicked));
    save_button->signal_clicked().connect(sigc::ptr_fun(&on_save_button_clicked));
    width_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_text_entry_for_resize_changed), width_entry));
    height_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_text_entry_for_resize_changed), height_entry));
    resize_button->signal_clicked().connect(sigc::ptr_fun(&on_resize_button_clicked));
    rotate_scale->signal_value_changed().connect(sigc::ptr_fun(&on_rotate_scale_value_changed));
    watermark_switch->signal_state_changed().connect(sigc::ptr_fun(&on_watermark_switch_state_changed));
    watermark_text_entry->signal_changed().connect(sigc::ptr_fun(&just_update_callback));
    watermark_rotate_scale->signal_value_changed().connect(sigc::ptr_fun(&just_update_callback));
    watermark_color_button->signal_color_set().connect(sigc::ptr_fun(&just_update_callback));
    font_size_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_x_y_font_entries_changed), font_size_entry));
    x_location_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_x_y_font_entries_changed), x_location_entry));
    y_location_entry->signal_changed().connect(
            sigc::bind(sigc::ptr_fun(&on_x_y_font_entries_changed), y_location_entry));
}


void update_image_widget() {
    // do nothing if image is invalid
    if (!image->isValid()) {
        return;
    }

    // update watermark on image
    set_watermark_to_image();


    Gtk::Image *image_widget;
    guint8 * buffer;
    size_t buffer_size;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    Magick::Image& raw_image(image->getProcessedImage());

    // get image widget from builder
    builder->get_widget(IMAGE_WIDGET_ID, image_widget);
    if (!image_widget) {
        std::cerr << "update_image_widget()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    // converting Magick::Image to Gtk::Pixbuf
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

    // set image to image widget
    image_widget->clear();
    image_widget->set(pixbuf);
}

static void on_text_entry_for_resize_changed(Gtk::Entry* entry) {
    std::string text = entry->get_text();

    // delete all non-digit chars
    for (auto it = text.begin(); it != text.end(); ) {
        if (!std::isdigit(*it))
            it = text.erase(it);
        else
            it++;
    }

    entry->set_text(text);
}

static void on_resize_button_clicked() {
    Gtk::Entry *width_entry;
    Gtk::Entry *height_entry;

    // get widgets from builder
    builder->get_widget(IMG_WIDTH_ENTRY_ID, width_entry);
    builder->get_widget(IMG_HEIGHT_ENTRY_ID, height_entry);
    if (!width_entry || !height_entry) {
        std::cerr << "on_resize_button_clicked()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    // do nothing if width_entry or height_entry are empty
    if (width_entry->get_text().length() == 0 || height_entry->get_text().length() == 0) {
        return;
    }

    // resize image
    size_t new_width = std::stoi(width_entry->get_text().c_str());
    size_t new_height = std::stoi(height_entry->get_text().c_str());

    image->resize(new_width, new_height);

    // update image on image widget
    update_image_widget();
}

static void on_rotate_scale_value_changed() {
    Gtk::Scale *rotate_scale;
    
    // get widgets from builder
    builder->get_widget(IMG_ROTATE_SCALE_ID, rotate_scale);
    if (!rotate_scale) {
        std::cerr << "on_rotate_scale_value_changed()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    // rotate image
    image->rotate(rotate_scale->get_value());

    // update image on image widget
    update_image_widget();
}

static void on_watermark_switch_state_changed(bool state) {
    static bool last_widget_state = false;
    Gtk::Switch *watermark_switch;

    // get widgets from builder
    builder->get_widget(WTRMARK_SWITCH_ID, watermark_switch);
    if (!watermark_switch) {
        std::cerr << "on_watermark_switch_state_changed()    Error: Could not get widgets from UI file" << std::endl;
        exit(0);
    }

    // do nothing if switch state has not been changed
    if (last_widget_state == watermark_switch->get_state())
        return;
    last_widget_state = watermark_switch->get_state();

    // update image on image widget (adding the watermark)
    update_image_widget();
}

static void just_update_callback() {
    // just update image on image widget
    update_image_widget();
}

static void on_x_y_font_entries_changed(Gtk::Entry* entry) {
    std::string text = entry->get_text();
    std::string text_befor_modify = text;

    // delete all non-digit chars
    for (auto it = text.begin(); it != text.end(); ) {
        if (!std::isdigit(*it))
            it = text.erase(it);
        else
            it++;
    }

    entry->set_text(text);

    // update image on image widget (update watermark)
    update_image_widget();
}


static Glib::RefPtr<Gtk::FileFilter> create_image_file_filter_filter() {
    auto filter = Gtk::FileFilter::create();

    // setting image patterns to file filter
    filter->add_pattern("*.png");
    filter->add_pattern("*.jpg");
    filter->add_pattern("*.jpeg");
    filter->add_pattern("*.gif");
    filter->add_pattern("*.tiff");
    filter->add_pattern("*.BMP");
    filter->set_name("Images");

    return filter;
}

bool is_image_file(const std::string& file_path) {
    std::list<std::string> patterns = {"\\.png$", "\\.jpg$", "\\.jpeg$", "\\.gif$", "\\.tiff$", "\\.BMP$"};

    // check is file image
    for (auto it = patterns.begin(); it != patterns.end(); it++) {
        std::regex pattern(*it, std::regex_constants::icase);

        if (std::regex_search(file_path, pattern))
            return true;
    }

  return false;
}

static void view_message(std::string message) {
    Gtk::Window* window;

     // get main window from builder
    builder->get_widget(MAIN_WINDOW_ID, window);
    if (!window) {
        std::cerr << "Error: Could not get window widget from UI file" << std::endl;
        exit(0);
    }

    Gtk::MessageDialog dialog(*window, message, false);
    dialog.run();
}

static void on_open_button_clicked() {
    Gtk::FileChooserDialog dialog("Select image", Gtk::FILE_CHOOSER_ACTION_OPEN);
    Gtk::Window* window;

     // get main window from builder
    builder->get_widget(MAIN_WINDOW_ID, window);
    if (!window) {
        std::cerr << "Error: Could not get window widget from UI file" << std::endl;
        exit(0);
    }
    
    // configuring FileChooserDialog
    auto filter = create_image_file_filter_filter();
    dialog.add_filter(filter);
    dialog.set_current_folder(Glib::get_home_dir());
    dialog.set_select_multiple(false);
    dialog.set_modal(true);
    dialog.set_transient_for(*window);
    dialog.add_button("Open", Gtk::RESPONSE_OK);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);

    // exec file dialog
    int result = dialog.run();
    if (result != Gtk::RESPONSE_OK) 
        return;
    
    // reading image from file
    std::string filename = dialog.get_filename();
    try {
        image->load(filename);
    }
    catch (std::exception &e) {
        view_message(e.what());
        return;
    }

    // update image on image widget
    update_image_widget();
}

static void on_save_button_clicked() {
    if (!image->isValid()) {
        view_message("You need to open file first");
        return;
    }

    Gtk::FileChooserDialog dialog("Save image", Gtk::FILE_CHOOSER_ACTION_SAVE);
    Gtk::Window* window;

    // get main window from builder
    builder->get_widget(MAIN_WINDOW_ID, window);
    if (!window) {
        std::cerr << "Error: Could not get window widget from UI file" << std::endl;
        exit(0);
    }
    
    // configuring FileChooserDialog
    auto filter = create_image_file_filter_filter();
    dialog.add_filter(filter);
    dialog.set_current_folder(Glib::get_home_dir());
    dialog.set_select_multiple(false);
    dialog.set_modal(true);
    dialog.set_transient_for(*window);
    dialog.add_button("Save", Gtk::RESPONSE_OK);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);

    // exec file dialog
    int result = dialog.run();
    if (result != Gtk::RESPONSE_OK)
        return;


    std::string filename = dialog.get_filename();

    // check is file realy image
    if ( !is_image_file(filename) ) {
        view_message("invalid file type");
        return;
    }

    // writing image to file
    try {
        image->save(filename);
    }
    catch (std::exception &e) {
        view_message(e.what());
        return;
    }
}

static void set_watermark_to_image() {
    Gtk::Switch *watermark_switch;
    Gtk::Entry *watermark_text;
    Gtk::Entry *font_size_entry;
    Gtk::ColorButton *color_button;
    Gtk::Scale *watermark_rotation;
    Gtk::Entry *x_location_entry;
    Gtk::Entry *y_location_entry;

    // get widgets from builder
    builder->get_widget(WTRMARK_SWITCH_ID, watermark_switch);
    builder->get_widget(WTRMARK_ENTRY_ID, watermark_text);
    builder->get_widget(WTRMARK_FONT_SIZE_ID, font_size_entry);
    builder->get_widget(WTRMARK_COLOR_ID, color_button);
    builder->get_widget(WTRMARK_ROTATE_SCALE_ID, watermark_rotation);
    builder->get_widget(WTRMARK_X_ENTRY_ID, x_location_entry);
    builder->get_widget(WTRMARK_Y_ENTRY_ID, y_location_entry);

    // check if every widget valid
    if (!watermark_switch   ||
        !watermark_text     ||
        !font_size_entry    || 
        !color_button       ||
        !watermark_rotation ||
        !x_location_entry   ||
        !y_location_entry)
    {
        std::cerr << "Builder::connect_all: Builder could not get widgets from UI file" << std::endl;
        exit(0);
    }

    // do nothing if watermark is disabled
    if (!watermark_switch->get_state())
        return;

    // init watermark fields with default values
    std::string text = "Watermark";
    size_t font_size = 30;
    std::string color;
    double rotation;
    size_t x_location = 0;
    size_t y_location = 0;

    // read watermark fields values from widgets
    if (watermark_text->get_text().size() != 0)
        text = watermark_text->get_text().c_str();
    
    if (font_size_entry->get_text().size() != 0)
        font_size = std::stoi(font_size_entry->get_text().c_str());
    
    color = color_button->get_color().to_string().c_str();
    rotation = watermark_rotation->get_value();

    if (x_location_entry->get_text().size() != 0)
        x_location = std::stoi(x_location_entry->get_text().c_str());

    if (y_location_entry->get_text().size() != 0)
        y_location = std::stoi(y_location_entry->get_text().c_str());

    // create watermark
    Watermark watermark(text, font_size, rotation, color, x_location, y_location);

    // set force reprocess to clear last watermark
    image->reprocessForce();

    // add watermark to image
    watermark.addWatermark(image->getProcessedImage());
}