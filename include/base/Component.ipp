/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template<typename O, typename I>
void Component<O, I>::update(O& object, I& info) {
    doUpdate(object, info);
}