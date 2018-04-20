#include <cassert>

#include "sorting.hpp"

#define INF 999999

void sort_moves(const Position &pos, Move *moves, const int num_moves, const Move &tt_move)
{
    assert(moves != NULL);

    int scores[256] = {0};

    // Score
    for(int n = 0; n < num_moves; ++n)
    {
        if(moves[n] == tt_move)
        {
            scores[n] = 1000;
        }
        else
        {
            scores[n] = count_captures(pos, moves[n]);
        }

        scores[n] += (is_single(moves[n]) ? 1 : 0);
    }

    // Sort
    for(int a = 0; a < num_moves; ++a)
    {
        int best_index = a;
        int best_score = -INF;

        for(int b = a; b < num_moves; ++b)
        {
            if(scores[b] > best_score)
            {
                best_score = scores[b];
                best_index = b;
            }
        }

        Move temp_move = moves[best_index];
        moves[best_index] = moves[a];
        moves[a] = temp_move;

        int temp_score = scores[best_index];
        scores[best_index] = scores[a];
        scores[a] = temp_score;
    }

#ifndef NDEBUG
    for(int i = 0; i < num_moves-1; ++i)
    {
        assert(scores[i] >= scores[i+1]);
    }
#endif
}
