#include "pv.hpp"
#include <cassert>

std::string get_pv_string(const PV &pv) {
    std::string str = "";

    for (int n = 0; n < pv.num_moves; ++n) {
        if (n > 0) {
            str += " ";
        }

        str += static_cast<std::string>(pv.moves[n]);
    }

    return str;
}

bool legal_pv(const libataxx::Position &pos, const PV &pv) {
    assert(pv.num_moves < 256);

    libataxx::Position npos = pos;

    for (int i = 0; i < pv.num_moves; ++i) {
        if (npos.is_legal_move(pv.moves[i]) == false) {
            return false;
        }

        npos.makemove(pv.moves[i]);
    }

    return true;
}
