#include "score.hpp"
#include "other.hpp"

int score(const Position &pos)
{
    int num_cross = popcountll(pos.pieces[PIECE::CROSS]);
    int num_nought = popcountll(pos.pieces[PIECE::NOUGHT]);
    int num_blockers = popcountll(pos.blockers);
    int num_empty = 49 - (num_cross + num_nought + num_blockers);

    int score = num_cross - num_nought;

    if(pos.turn == SIDE::CROSS)
    {
        return score;
    }
    else
    {
        return -score;
    }
}
