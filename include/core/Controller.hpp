/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "ui/Interface.hpp"

class Controller {
 public:
    void start();
    void quit();

 private:
    Interface ui;
};

#endif /* CONTROLLER_HPP */
