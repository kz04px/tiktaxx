#ifndef PERFT_HPP_INCLUDED
#define PERFT_HPP_INCLUDED

#include <cinttypes>

#include "ataxx.hpp"
#include "hashtable.hpp"

void perft(Hashtable *tt, const Position& pos, const int depth);
void splitPerft(Hashtable *tt, const Position& pos, const int depth);

#endif
