/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

template<typename Object, typename Info>
class Component {
 public:
    void update(const Object&, Info&);
 private:
    virtual void doUpdate(const Object&, Info&) = 0;
};

#include "Component.hpp"

#endif /* COMPONENT_HPP */