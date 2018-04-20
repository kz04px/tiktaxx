#ifndef SORTING_HPP_INCLUDED
#define SORTING_HPP_INCLUDED

#include "ataxx.hpp"
#include "move.hpp"

void sort_moves(const Position &pos, Move *moves, const int num_moves, const Move &tt_move);

#endif
