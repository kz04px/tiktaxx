#include "score.hpp"
#include "other.hpp"

int score(const Position& pos)
{
    int numCross = popcountll(pos.pieces[PIECE::CROSS]);
    int numNought = popcountll(pos.pieces[PIECE::NOUGHT]);
    int numBlockers = popcountll(pos.blockers);
    int numFree = 49 - (numCross + numNought + numBlockers);

    int score = numCross - numNought;

    if(pos.turn == SIDE::CROSS)
    {
        return score;
    }
    else
    {
        return -score;
    }
}