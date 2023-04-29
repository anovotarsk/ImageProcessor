#pragma once

#include <gtkmm.h>
#include <string>
#include <iostream>
#include "../img_processing_src/Image.h"

extern Glib::RefPtr<Gtk::Builder> builder;
extern std::shared_ptr<Image> image;


void init_builder(std::string ui_file);
void connect_all();

static void on_text_entry_for_resize_changed(Gtk::Entry* entry);
static void on_resize_button_clicked();
void on_rotate_scale_value_changed();

