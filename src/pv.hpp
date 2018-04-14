#ifndef PV_HPP_INCLUDED
#define PV_HPP_INCLUDED

#include "move.hpp"

typedef struct
{
    int numMoves;
    Move moves[64];
} PV;

std::string getPvString(const Move *moves, const int pvLength);

#endif