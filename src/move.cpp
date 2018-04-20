#include <iostream>

#include "movegen.hpp"
#include "move.hpp"
#include "bitboards.hpp"
#include "other.hpp"

std::string move_string(const Move &n)
{
    if(is_single(n))
    {
        return squares[n.to];
    }
    else
    {
        return squares[n.from] + squares[n.to];
    }
}

void print_moves(const Position &pos)
{
    Move moves[256];
    int num_moves = movegen(pos, moves);

    for(int n = 0; n < num_moves; ++n)
    {
        std::cout << n+1
                  << ") "
                  << move_string(moves[n])
                  << " "
                  << (is_single(moves[n]) ? "single" : "double")
                  << std::endl;
    }
}

int count_captures(const Position &pos, const Move &n)
{
    uint64_t near = single_jump_sq(n.to);
    uint64_t captured = near & pos.pieces[!pos.turn];
    return popcountll(captured);
}

bool legal_move(const Position &pos, std::string move)
{
    Move moves[256];
    int num_moves = movegen(pos, moves);

    for(int n = 0; n < num_moves; ++n)
    {
        if(move_string(moves[n]) == move)
        {
            return true;
        }
    }

    return false;
}

bool legal_move(const Position &pos, const Move &move)
{
    Move moves[256];
    int num_moves = movegen(pos, moves);

    for(int n = 0; n < num_moves; ++n)
    {
        if(moves[n] == move)
        {
            return true;
        }
    }

    return false;
}

bool is_single(const Move &move)
{
    return (move.from == move.to);
}

bool is_double(const Move &move)
{
    return (move.from != move.to);
}
