#pragma once

#include <gtkmm.h>
#include <string>
#include <iostream>
#include <regex>
#include "../img_processing_src/Image.h"
#include "../img_processing_src/Watermark.h"

// define the UI file location
#define UI_FILE_LOCATION        "ui.glade"

// define IDs for various widgets
#define MAIN_WINDOW_ID          "main_window"
#define OPEN_BUTTON_ID          "open_button"
#define SAVE_BUTTON_ID          "save_button"
#define IMAGE_WIDGET_ID         "image"
#define IMG_ROTATE_SCALE_ID     "img_rotate_scale"
#define IMG_WIDTH_ENTRY_ID      "width_entry"
#define IMG_HEIGHT_ENTRY_ID     "height_entry"
#define IMG_RESIZE_BUTTON_ID    "resize_button"
#define WTRMARK_SWITCH_ID       "watermark_switch"
#define WTRMARK_ENTRY_ID        "watermark_entry"
#define WTRMARK_FONT_SIZE_ID    "watermark_font_entry"
#define WTRMARK_COLOR_ID        "watermark_color"
#define WTRMARK_ROTATE_SCALE_ID "watermark_rotate_scale"
#define WTRMARK_X_ENTRY_ID      "watermark_x_entry"
#define WTRMARK_Y_ENTRY_ID      "watermark_y_entry"

// declare global variables
extern Glib::RefPtr<Gtk::Builder> builder;
extern std::shared_ptr<Image> image;


// function to initialize Gtk::Builder object
// this function must be called at main
void init_builder(std::string ui_file);

// function to connect all signal handlers to their respective widgets
void connect_all();

// function to update the image widget with the latest changes
void update_image_widget();

// function to set the watermark to the current image
static void set_watermark_to_image();

// function to display a message in a dialog box
static void view_message(std::string message);

// signals handlers for widgets events
static void on_open_button_clicked();
static void on_save_button_clicked();
static void on_text_entry_for_resize_changed(Gtk::Entry* entry);
static void on_resize_button_clicked();
static void on_rotate_scale_value_changed();
static void on_watermark_switch_state_changed(bool state);
static void on_x_y_font_entries_changed(Gtk::Entry* entry);
static void just_update_callback();
