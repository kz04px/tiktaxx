#ifndef ZOBRIST_HPP_INCLUDED
#define ZOBRIST_HPP_INCLUDED

#include <cstdint>

#include "ataxx.hpp"

void zobrist_init();
uint64_t generate_key(const Position &pos);
uint64_t get_turn_key();
uint64_t get_piece_key(const int side, const int sq);

#endif
