#include <iostream>

#include "movegen.hpp"
#include "move.hpp"

std::string moveString(const Move n)
{
    if(n.to == n.from)
    {
        return squares[n.to];
    }
    else
    {
        return squares[n.from] + squares[n.to];
    }
}

void printMoves(const Position& pos)
{
    Move moves[256];
    int numMoves = movegen(pos, moves);

    for(int n = 0; n < numMoves; ++n)
    {
        std::cout << n+1
                  << ") "
                  << moveString(moves[n])
                  << " "
                  << (moves[n].to == moves[n].from ? "single" : "double")
                  << std::endl;
    }
}
