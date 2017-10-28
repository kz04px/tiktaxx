#include "pv.hpp"

std::string getPvString(const Move *moves, const int pvLength)
{
    std::string pvString = "";

    for(int n = 0; n < pvLength; ++n)
    {
        if(n > 0)
        {
            pvString += " ";
        }

        pvString += moveString(moves[n]);
    }

    return pvString;
}
