#include <iostream>
#include <ctime>

#include "zobrist.hpp"
#include "uai.hpp"

int main()
{
    zobrist_init();

    std::string mode;
    std::cin >> mode;
    if(mode == "manual")
    {
        manual();
    }
    else if(mode == "uai")
    {
        uai();
    }
    else if(mode == "about")
    {
        std::cout << "An Ataxx engine written in C++" << std::endl;
        std::cout << "https://github.com/kz04px/ataxx-engine" << std::endl;
        std::cout << "Compiled: " <<   __TIME__ << " " << __DATE__ <<std::endl;
#ifdef NDEBUG
        std::cout << "NDEBUG enabled" << std::endl;
#endif
#ifdef NULLMOVE
        std::cout << "NULLMOVE enabled" << std::endl;
#endif
#ifdef KILLER_MOVES
        std::cout << "KILLER_MOVES enabled" << std::endl;
#endif
#ifdef LMR
        std::cout << "LMR enabled" << std::endl;
#endif
#ifdef ASPIRATION_WINDOWS
        std::cout << "ASPIRATION_WINDOWS enabled" << std::endl;
#endif
    }
    else
    {
        std::cout << "ERROR: Unknown protocol " << mode << std::endl;
    }

    return 0;
}
