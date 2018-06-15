#include <cassert>
#include "score.hpp"
#include "bitboards.hpp"
#include "other.hpp"

int score(const Position &pos)
{
    uint64_t empty = U64_BOARD & ~(pos.pieces[PIECE::CROSS] | pos.pieces[PIECE::NOUGHT] | pos.blockers);

    int num_cross = popcountll(pos.pieces[PIECE::CROSS]);
    int num_nought = popcountll(pos.pieces[PIECE::NOUGHT]);
    int num_blockers = popcountll(pos.blockers);
    int num_empty = popcountll(empty);

    assert(num_cross + num_nought + num_blockers + num_empty == 49);

    uint64_t cross_moves = single_jump_bb(pos.pieces[PIECE::CROSS]);
    cross_moves = single_jump_bb(cross_moves);
    cross_moves &= empty;

    uint64_t nought_moves = single_jump_bb(pos.pieces[PIECE::NOUGHT]);
    nought_moves = single_jump_bb(nought_moves);
    nought_moves &= empty;

    if(cross_moves == 0ULL && nought_moves != 0ULL)
    {
        num_nought += num_empty;
    }
    else if(cross_moves != 0ULL && nought_moves == 0ULL)
    {
        num_cross += num_empty;
    }

    int score = num_cross - num_nought;

    assert(-49 + num_blockers <= score);
    assert(score <= 49 - num_blockers);

    if(pos.turn == SIDE::CROSS)
    {
        return score;
    }
    else
    {
        return -score;
    }
}
