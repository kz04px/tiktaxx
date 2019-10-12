#ifndef PV_HPP_INCLUDED
#define PV_HPP_INCLUDED

#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <string>

struct PV {
    PV() : num_moves(0) {
    }

    int num_moves;
    libataxx::Move moves[256];
};

std::string get_pv_string(const PV &pv);
bool legal_pv(const libataxx::Position &pos, const PV &pv);

#endif
