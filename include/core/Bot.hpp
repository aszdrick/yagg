/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BOT_HPP
#define BOT_HPP

#include "base/Command.hpp"
#include <memory>

template<typename T>
class Bot {
 public:
    virtual std::unique_ptr<Command<T>> play(const T&) = 0;
};

#endif /* BOT_HPP */