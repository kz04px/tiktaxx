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

#define NO_MOVE ((Move){.from=50, .to=50})

std::string moveString(const Move n);
void printMoves(const Position& pos);
int countCaptures(const Position& pos, const Move n);
bool legalMove(const Position& pos, std::string move);
bool legalMove(const Position& pos, const Move& move);
bool isSingle(const Move& move);
bool isDouble(const Move& move);

#endif
