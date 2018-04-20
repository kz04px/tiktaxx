#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include "ataxx.hpp"

struct Move
{
    int from;
    int to;

    Move() : from(0), to(0)
    {
    }

    Move(int from, int to) : from(from), to(to)
    {
    }

    bool operator==(const Move &n) const
    {
        return (from == n.from && to == n.to);
    }

    bool operator!=(const Move &n) const
    {
        return (from != n.from || to != n.to);
    }
};

#define NO_MOVE ((Move){.from=50, .to=50})

std::string move_string(const Move &n);
void print_moves(const Position &pos);
int count_captures(const Position &pos, const Move &n);
bool legal_move(const Position &pos, std::string move);
bool legal_move(const Position &pos, const Move &move);
bool is_single(const Move &move);
bool is_double(const Move &move);

#endif
