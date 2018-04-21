#include <iostream>

#include "eval.hpp"
#include "bitboards.hpp"
#include "phase.hpp"
#include "score.hpp"
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
    uint64_t empty = U64_BOARD & ~(pos.pieces[PIECE::CROSS] | pos.pieces[PIECE::NOUGHT] | pos.blockers);
    float p = phase(pos);

    uint64_t cross_moves = single_jump_bb(pos.pieces[PIECE::CROSS]);
    cross_moves = single_jump_bb(cross_moves);
    cross_moves &= empty;

    uint64_t nought_moves = single_jump_bb(pos.pieces[PIECE::NOUGHT]);
    nought_moves = single_jump_bb(nought_moves);
    nought_moves &= empty;

    // Win condition
    if(cross_moves == 0ULL || nought_moves == 0ULL)
    {
        int num_empty = popcountll(empty);

        int score = num_friendly - num_unfriendly;

        if(cross_moves == 0ULL)
        {
            score -= num_empty;
        }
        else if(nought_moves == 0ULL)
        {
            score += num_empty;
        }

        if(score > 0)
        {
            score = INF;
        }
        else
        {
            score = -INF;
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

    int our_pst = 0;
    int their_pst = 0;

    uint64_t copy = pos.pieces[PIECE::CROSS];
    while(copy)
    {
        int sq = lsb(copy);

        our_pst += PST[sq];

        copy &= copy - 1;
    }

    copy = pos.pieces[PIECE::NOUGHT];
    while(copy)
    {
        int sq = lsb(copy);

        their_pst += PST[sq];

        copy &= copy - 1;
    }

    int score =   (p*TURN_BONUS + 200)*(pos.turn == SIDE::CROSS ? 1 : -1)
                + PIECE_VALUE*num_friendly
                - PIECE_VALUE*num_unfriendly
                + MOBILITY_VALUE*our_mobility
                - MOBILITY_VALUE*their_mobility
                + our_pst
                - their_pst;

    if(pos.turn == SIDE::CROSS)
    {
        return score;
    }
    else
    {
        return -score;
    }
}
