#ifndef SEARCHSTACK_HPP_INCLUDED
#define SEARCHSTACK_HPP_INCLUDED

#include "move.hpp"

typedef struct
{
    int ply;
#ifdef KILLER_MOVES
    Move killer;
#endif
#ifdef NULLMOVE
    bool nullmove;
#endif
} searchStack;

#endif
