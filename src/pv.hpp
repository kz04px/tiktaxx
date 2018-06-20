#ifndef PV_HPP_INCLUDED
#define PV_HPP_INCLUDED

#include "move.hpp"

struct PV
{
    PV() : num_moves(0)
    {
    }

    int num_moves;
    Move moves[256];
};

std::string get_pv_string(const PV &pv);

#endif
