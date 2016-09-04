/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BASE_COMMAND_HPP
#define BASE_COMMAND_HPP

namespace base {
    template<typename T,typename R = void>
    class Command {
      public:
        R execute(const T&);
    };
}

#endif /* BASE_COMMAND_HPP */
