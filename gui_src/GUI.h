#pragma once

#include <gtkmm.h>
#include <string>
#include <iostream>
#include "../img_processing_src/Image.h"
#include "../img_processing_src/Watermark.h"

#define UI_FILE_LOCATION        "ui.glade"

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

extern Glib::RefPtr<Gtk::Builder> builder;
extern std::shared_ptr<Image> image;


void init_builder(std::string ui_file);
void connect_all();
void update_image_widget();
static void set_watermark_to_image();

static void on_text_entry_for_resize_changed(Gtk::Entry* entry);
static void on_resize_button_clicked();
static void on_rotate_scale_value_changed();
static void on_watermark_switch_state_changed(bool state);
static void on_x_y_font_entries_changed(Gtk::Entry* entry);
static void just_update_callback();



