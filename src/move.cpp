#include <iostream>

#include "movegen.hpp"
#include "move.hpp"
#include "bitboards.hpp"
#include "other.hpp"

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

int countCaptures(const Position& pos, const Move n)
{
    uint64_t toBB = (1ULL) << n.to;
    uint64_t near = singleJump(toBB);
    uint64_t captured = near & pos.pieces[!pos.turn];
    return popcountll(captured);
}

bool legalMove(const Position& pos, std::string move)
{
    Move moves[256];
    int numMoves = movegen(pos, moves);

    for(int n = 0; n < numMoves; ++n)
    {
        if(moveString(moves[n]) == move)
        {
            return true;
        }
    }

    return false;
}

bool legalMove(const Position& pos, const Move& move)
{
    Move moves[256];
    int numMoves = movegen(pos, moves);

    for(int n = 0; n < numMoves; ++n)
    {
        if(moves[n] == move)
        {
            return true;
        }
    }

    return false;
}
