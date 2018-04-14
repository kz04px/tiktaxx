#ifndef ZOBRIST_HPP_INCLUDED
#define ZOBRIST_HPP_INCLUDED

#include <cstdint>

#include "ataxx.hpp"

void zobristInit();
uint64_t generateKey(const Position& pos);
uint64_t getTurnKey();
uint64_t getPieceKey(const int side, const int sq);

#endif