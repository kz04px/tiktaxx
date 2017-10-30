#include <iostream>
#include <ctime>

#include "zobrist.hpp"
#include "protocol.hpp"

int main()
{
    srand(time(0));
    zobristInit();
    messageLoop();
    return 0;
}
