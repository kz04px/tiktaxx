#include <iostream>

#include "eval.hpp"
#include "bitboards.hpp"
#include "phase.hpp"
#include "other.hpp"

#define INF 1000000
#define TURN_BONUS 300
#define PIECE_VALUE 100
#define MOBILITY_VALUE 1

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

void split_eval(const Position &pos)
{
    int num_friendly = popcountll(pos.pieces[PIECE::CROSS]);
    int num_unfriendly = popcountll(pos.pieces[PIECE::NOUGHT]);

    std::cout << "Num friendly:   " << num_friendly << std::endl;
    std::cout << "Num unfriendly: " << num_unfriendly << std::endl;
}

int eval(const Position &pos)
{
    int num_friendly = popcountll(pos.pieces[PIECE::CROSS]);
    int num_unfriendly = popcountll(pos.pieces[PIECE::NOUGHT]);
    int our_mobility = 0;
    int their_mobility = 0;
    uint64_t empty = ~(pos.pieces[PIECE::CROSS] | pos.pieces[PIECE::NOUGHT] | pos.blockers);
    float p = phase(pos);

/*
    // Win condition
    if(empty == 0ULL)
    {
        int score = 0;
        if(num_friendly > num_unfriendly)
        {
            score =  INF;
        }
        else if(num_friendly < num_unfriendly)
        {
            score =  -INF;
        }

        if(pos.turn == SIDE::CROSS)
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
                + PIECE_VALUE*num_friendly
                - PIECE_VALUE*num_unfriendly
                + MOBILITY_VALUE*our_mobility
                - MOBILITY_VALUE*their_mobility
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
