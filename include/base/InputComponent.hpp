/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef INPUT_COMPONENT_HPP
#define INPUT_COMPONENT_HPP

template<typename>
class Player;

template<typename Object, typename Provider>
class InputComponent {
 public:
    virtual void update(const Object&, Provider&) = 0;
};

#endif /* INPUT_COMPONENT_HPP */