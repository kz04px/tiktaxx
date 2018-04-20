#ifndef PV_HPP_INCLUDED
#define PV_HPP_INCLUDED

#include "move.hpp"

struct PV
{
    int num_moves;
    Move moves[64];
};

std::string get_pv_string(const Move *moves, const int pv_length);

#endif
