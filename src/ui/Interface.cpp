/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "ui/Interface.hpp"
#include "ui/cairo.hpp"
#include "ui/gtk.hpp"

Interface::Interface() { }

void Interface::build() {
    builder = gtk_builder_new_from_file("res/view.ui");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    canvas = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    // More stuff goes here

    connect_signals();
}

void Interface::connect_signals() {
    g_signal_connect(window, "delete-event", 
        G_CALLBACK(signals::quit), nullptr);
    g_signal_connect(canvas, "configure-event", 
        G_CALLBACK(signals::configure), NULL);
    g_signal_connect(canvas, "draw",
        G_CALLBACK(signals::draw), NULL);
    // More stuff goes here
}

void Interface::start() {
    gtk_widget_show_all(window);
    cairo::clear();
    queueDraw();
    gtk_main();
}

void Interface::quit() {
    gtk_main_quit();
}

void Interface::show_message(GtkMessageType type, GtkButtonsType buttons,
                             const std::string& title, const std::string& message,
                             const std::string& submessage) const {
    auto dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         type,
                                         buttons,
                                         message.c_str());
    
    gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
                                             submessage.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void Interface::queueDraw() {
    gtk::queue_draw(canvas);
}