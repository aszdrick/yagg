/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef COMMAND_HPP
#define COMMAND_HPP

namespace base {
    template<typename T>
    class Command {
      public:
        void execute(const T&);
        void execute(T&);
    };
}

#endif /* COMMAND_HPP */
