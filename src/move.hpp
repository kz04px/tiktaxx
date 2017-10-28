#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include "ataxx.hpp"

struct Move
{
    int from;
    int to;

    bool operator==(const Move& a) const
    {
        return (from == a.from && to == a.to);
    }
};

std::string moveString(const Move n);
void printMoves(const Position& pos);

#endif
