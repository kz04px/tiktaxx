#include <cassert>
#include "pv.hpp"

std::string get_pv_string(const Move *moves, const int pv_length)
{
    assert(moves != NULL);

    std::string pv_string = "";

    for(int n = 0; n < pv_length; ++n)
    {
        if(n > 0)
        {
            pv_string += " ";
        }

        pv_string += move_string(moves[n]);
    }

    return pv_string;
}
