/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <gtk/gtk.h>
#include <string>

class Controller;

namespace signals {
    void set_controller(Controller*);

    bool configure(GtkWidget* const, GdkEventConfigure* const);
    
    void draw(GtkWidget* const, cairo_t* const);

    void init(int*, char***);

    void start();

    void quit();
};

#endif /* SIGNALS_HPP */
