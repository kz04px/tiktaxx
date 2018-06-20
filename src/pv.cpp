#include <cassert>
#include "pv.hpp"
#include "makemove.hpp"

std::string get_pv_string(const PV &pv)
{
    std::string str = "";

    for(int n = 0; n < pv.num_moves; ++n)
    {
        if(n > 0)
        {
            str += " ";
        }

        str += move_string(pv.moves[n]);
    }

    return str;
}

bool legal_pv(const Position &pos, const PV &pv)
{
    assert(pv.num_moves < 256);

    Position npos = pos;

    for(int i = 0; i < pv.num_moves; ++i)
    {
        if(legal_move(npos, pv.moves[i]) == false)
        {
            return false;
        }

        makemove(npos, pv.moves[i]);
    }

    return true;
}
