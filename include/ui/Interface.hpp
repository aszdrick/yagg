/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include "signals.hpp"

class Interface {
 public:
    Interface();

    void show_message(GtkMessageType, GtkButtonsType, const std::string&,
                      const std::string&, const std::string&) const;
    
    void queueDraw();

    void build();
    void start();
    void quit();

 private:
    GtkBuilder* builder;
    GtkWidget* window;
    GtkWidget* canvas;

    void connect_signals();
};

#endif /* INTERFACE_HPP */
