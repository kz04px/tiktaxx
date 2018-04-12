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
        std::cout << "id name ataxx-engine" << std::endl;
        std::cout << "id author kz04px" << std::endl;
        std::cout << "uaiok" << std::endl;
        messageLoop();
    }
    else
    {
        std::cout << "ERROR: Unknown protocol " << mode << std::endl;
    }

    return 0;
}
