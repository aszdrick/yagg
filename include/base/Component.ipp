/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template<typename O, typename I, typename R>
R base::Component<O,I,R>::update(O& object, I& info) {
    return doUpdate(object, info);
}