/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */


#include "core/Controller.hpp"

void Controller::start() {
    ui.build();
    ui.start();
}

void Controller::quit() {
    ui.quit();
}
