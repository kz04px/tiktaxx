#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include "ataxx.hpp"

struct Move
{
    int from;
    int to;

    bool operator==(const Move& n) const
    {
        return (from == n.from && to == n.to);
    }

    bool operator!=(const Move& n) const
    {
        return (from != n.from || to != n.to);
    }
};

std::string moveString(const Move n);
void printMoves(const Position& pos);
int countCaptures(const Position& pos, const Move n);

#endif
