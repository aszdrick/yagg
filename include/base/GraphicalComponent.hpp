/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GRAPHICAL_COMPONENT_HPP
#define GRAPHICAL_COMPONENT_HPP

template<typename Object, typename Renderer>
class GraphicalComponent {
 public:
    void update(const Object&, Renderer&);
};

#endif /* GRAPHICAL_COMPONENT_HPP */