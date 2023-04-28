#pragma once

#include <gtkmm.h>

class Builder {
private:
    Glib::RefPtr<Gtk::Builder> builder;

public:
    Builder();

};