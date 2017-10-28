#ifndef PERFT_HPP_INCLUDED
#define PERFT_HPP_INCLUDED

#include <cinttypes>

#include "ataxx.hpp"

void perft(const Position& pos, const int depth);
void splitPerft(const Position& pos, const int depth);
uint64_t perftSearch(const int depth);

#endif
