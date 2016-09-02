/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace base {
    template<typename Object, typename Info>
    class Component {
     public:
        using Agent = Object;
        using Element = Info;

        void update(Object&, Info&);
     private:
        virtual void doUpdate(Agent&, Element&) = 0;
    };    
}

#include "Component.ipp"

#endif /* COMPONENT_HPP */