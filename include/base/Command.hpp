/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef COMMAND_HPP
#define COMMAND_HPP

namespace base {
    template<typename T, typename V = T>
    class Command {
      public:
        T execute(const V&);
        void execute(const T&);
    };
}

#endif /* COMMAND_HPP */
