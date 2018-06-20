#include <cassert>
#include "pv.hpp"

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
