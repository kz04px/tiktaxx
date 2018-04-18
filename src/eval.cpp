#include <iostream>

#include "eval.hpp"
#include "bitboards.hpp"
#include "phase.hpp"
#include "other.hpp"

#define INF (1000000)
#define TURN_BONUS (300)
#define PIECE_VALUE (100)
#define MOBILITY_VALUE (10)

const int PST[49] =
{
  30,  20,  10,  10,  10,  20,  30,
  20,  10,  10,   5,  10,  10,  20,
  10,  10,   5,   0,   5,  10,  10,
  10,   5,   0,   0,   0,   5,  10,
  10,  10,   5,   0,   5,  10,  10,
  20,  10,  10,   5,  10,  10,  20,
  30,  20,  10,  10,  10,  20,  30
};

void splitEval(const Position& pos)
{
    int numFriendly = popcountll(pos.pieces[PIECE::CROSS]);
    int numUnfriendly = popcountll(pos.pieces[PIECE::NOUGHT]);

    std::cout << "Num friendly:   " << numFriendly << std::endl;
    std::cout << "Num unfriendly: " << numUnfriendly << std::endl;
}

int eval(const Position& pos)
{
    int numFriendly = popcountll(pos.pieces[PIECE::CROSS]);
    int numUnfriendly = popcountll(pos.pieces[PIECE::NOUGHT]);
    int ourMobility = 0;
    int theirMobility = 0;
    float p = phase(pos);

/*
    // Win condition
    if(numFriendly == 0 || numUnfriendly == 0)
    if((pieces[PIECE::CROSS] | pieces[PIECE::NOUGHT] | blockers) == U64_BOARD)
    {
        int score = 0;
        if(numFriendly > numUnfriendly)
        {
            score =  INF;
        }
        else if(numFriendly < numUnfriendly)
        {
            score =  -INF;
        }

        if(turn == SIDE::CROSS)
        {
            return score;
        }
        else
        {
            return -score;
        }
    }
*/

    int ourPST = 0;
    int theirPST = 0;

    uint64_t copy = pos.pieces[PIECE::CROSS];
    while(copy)
    {
        int sq = lsb(copy);

        ourPST += PST[sq];

        copy &= copy - 1;
    }

    copy = pos.pieces[PIECE::NOUGHT];
    while(copy)
    {
        int sq = lsb(copy);

        theirPST += PST[sq];

        copy &= copy - 1;
    }

    int score =   (p*TURN_BONUS + 200)*(pos.turn == SIDE::CROSS ? 1 : -1)
                + PIECE_VALUE*numFriendly
                - PIECE_VALUE*numUnfriendly
                + MOBILITY_VALUE*ourMobility
                - MOBILITY_VALUE*theirMobility
                + ourPST
                - theirPST;

    if(pos.turn == SIDE::CROSS)
    {
        return score;
    }
    else
    {
        return -score;
    }
}