#include <iostream>
#include <ctime>

#include "zobrist.hpp"
#include "protocol.hpp"

int main()
{
    srand(time(0));
    zobristInit();

    std::string mode;
    std::cin >> mode;
    if(mode == "manual")
    {
        manual();
    }
    else if(mode == "uai")
    {
        messageLoop();
    }
    else
    {
        std::cout << "ERROR: Unknown protocol " << mode << std::endl;
    }

    return 0;
}
