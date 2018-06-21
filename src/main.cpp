#include <iostream>
#include <ctime>

#include "zobrist.hpp"
#include "uai.hpp"
#include "move.hpp"
#include "hashtable.hpp"

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
        std::cout << "Tiktaxx - An Ataxx engine written in C++" << std::endl;
        std::cout << "https://github.com/kz04px/tiktaxx" << std::endl;
        std::cout << "Compiled: " <<   __TIME__ << " " << __DATE__ << std::endl;
        std::cout << std::endl;

#ifdef NDEBUG
        std::cout << "Release mode" << std::endl;
#else
        std::cout << "Debug mode" << std::endl;
#endif
        std::cout << std::endl;

        std::cout << "Search:" << std::endl;
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
#ifdef IID
        std::cout << "IID enabled" << std::endl;
#endif
#ifdef FUTILITY_PRUNING
        std::cout << "FUTILITY_PRUNING enabled" << std::endl;
#endif
        std::cout << std::endl;

        std::cout << "Other:" << std::endl;
        std::cout << "sizeof(Move): " << sizeof(Move) << " bytes" << std::endl;
        std::cout << "sizeof(Entry): " << sizeof(Entry) << " bytes" << std::endl;
        std::cout << "entries/MB: " << 1048576/sizeof(Entry) << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "ERROR: Unknown protocol " << mode << std::endl;
    }

    return 0;
}
