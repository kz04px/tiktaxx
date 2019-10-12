#include "phase.hpp"
#include <cassert>
#include "other.hpp"

float phase(const libataxx::Position &pos) {
    libataxx::Bitboard all = pos.black() | pos.white() | pos.gaps();
    float p = (float)all.count() / 49;
    assert(p >= 0.0);
    assert(p <= 1.0);
    return p;
}

bool is_endgame(const libataxx::Position &pos) {
    return phase(pos) >= 0.75;
}
