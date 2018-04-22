#include <string>
#include <cassert>

#include "makemove.hpp"
#include "bitboards.hpp"
#include "invalid.hpp"
#include "other.hpp"

void makemove(Position &pos, const int to)
{
    makemove(pos, (Move){.from=to, .to=to});
}

void makemove(Position &pos, const int from, const int to)
{
    makemove(pos, (Move){.from=from, .to=to});
}

void makemove(Position &pos, const std::string &move_string)
{
    int from = 0;
    int to = 0;

    if(move_string.size() == 2)
    {
        int x = move_string[0]-'a';
        int y = move_string[1]-'1';
        int sq = y*7 + x;

        to = sq;
        from = sq;
    }
    else if(move_string.size() == 4)
    {
        int x = move_string[0]-'a';
        int y = move_string[1]-'1';
        int sq = y*7 + x;
        from = sq;

        x = move_string[2]-'a';
        y = move_string[3]-'1';
        sq = y*7 + x;
        to = sq;
    }
    else
    {
        return;
    }

    makemove(pos, (Move){.from=from, .to=to});
}

void makemove(Position &pos, const Move &n)
{
    uint64_t to_bb = (1ULL)<<(n.to);
    uint64_t from_bb = (1ULL)<<(n.from);
    uint64_t captured = single_jump_sq(n.to) & pos.pieces[!pos.turn];

    // If there's a piece on the from square, remove it
    pos.pieces[pos.turn] &= ~from_bb;

    // Put a piece on the to square
    pos.pieces[pos.turn] ^= to_bb;

    // Flip any captured pieces
    pos.pieces[pos.turn] ^= captured;
    pos.pieces[!pos.turn] ^= captured;

    pos.turn = !pos.turn;
    pos.ply++;

    assert(invalid(pos) == false);
}

bool makemoves(Position &pos, const std::string &moves_string)
{
    std::vector<std::string> tokens = split(moves_string, ' ');

    for(unsigned int n = 0; n < tokens.size(); ++n)
    {
        if(legal_move(pos, tokens[n]) == false) {return false;}
        makemove(pos, tokens[n]);
    }

    return true;
}
