/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template<typename T>
MiniMaxTree<T>::MiniMaxTree(const RatingFunction<T>& heuristic,
    const RatingFunction<T>& utility)
 : h_function(heuristic),
   u_function(utility) {

}

template<typename T>
Command<T> MiniMaxTree<T>::findNextMove(const T&) {
    return Command<T>();
}
