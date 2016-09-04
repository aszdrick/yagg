/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template<typename O, typename I, typename R>
R base::Component<O,I,R>::update(O& object, I& info) {
    return doUpdate(object, info);
}