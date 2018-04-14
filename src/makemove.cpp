#include <string>

#include "makemove.hpp"
#include "bitboards.hpp"
#include "other.hpp"

void makemove(Position& pos, const int to)
{
    makemove(pos, (Move){.from=to, .to=to});
}

void makemove(Position& pos, const int from, const int to)
{
    makemove(pos, (Move){.from=from, .to=to});
}

void makemove(Position& pos, const std::string moveString)
{
    int from = 0;
    int to = 0;

    if(moveString.size() == 2)
    {
        int x = moveString[0]-'a';
        int y = moveString[1]-'1';
        int sq = y*7 + x;

        to = sq;
        from = sq;
    }
    else if(moveString.size() == 4)
    {
        int x = moveString[0]-'a';
        int y = moveString[1]-'1';
        int sq = y*7 + x;
        from = sq;

        x = moveString[2]-'a';
        y = moveString[3]-'1';
        sq = y*7 + x;
        to = sq;
    }
    else
    {
        return;
    }

    makemove(pos, (Move){.from=from, .to=to});
}

void makemove(Position& pos, const Move n)
{
    uint64_t toBB = (1ULL)<<(n.to);
    uint64_t fromBB = (1ULL)<<(n.from);
    uint64_t captured = singleJump(toBB) & pos.pieces[!pos.turn];

    // If there's a piece on the from square, remove it
    pos.pieces[pos.turn] &= ~fromBB;

    // Put a piece on the to square
    pos.pieces[pos.turn] ^= toBB;

    // Flip any captured pieces
    pos.pieces[pos.turn] ^= captured;
    pos.pieces[!pos.turn] ^= captured;

    pos.turn = !pos.turn;
    pos.ply++;
}

bool makemoves(Position& pos, const std::string movesString)
{
    std::vector<std::string> tokens = split(movesString, ' ');

    for(unsigned int n = 0; n < tokens.size(); ++n)
    {
        if(legalMove(pos, tokens[n]) == false) {return false;}
        makemove(pos, tokens[n]);
    }

    return true;
}