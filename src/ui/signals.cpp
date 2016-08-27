/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "ui/signals.hpp"
#include "core/Controller.hpp"
#include "ui/cairo.hpp"

namespace {
    Controller* controller;
}

namespace signals {
    void set_controller(Controller* c) {
        controller = c;
    }

    bool configure(GtkWidget* const widget, GdkEventConfigure* const event) {
        cairo::update(widget);
        return true;
    }

    void draw(GtkWidget* const widget, cairo_t* const cr) {
        //controller->draw(widget, cr);
        cairo::draw(cr);
    }

    void init(int* argc, char*** argv) {
        gtk_init(argc, argv);
    }

    void start() {
        controller->start();
    }

    void quit() {
        controller->quit();
        cairo::destroy();
    }
}
