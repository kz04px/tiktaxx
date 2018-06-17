/*
 * Random game simulation
 * -- Play a game of Ataxx choosing moves at random
 * -- return 1.0 for a win
 * -- return 0.5 for a draw
 * -- return 0.0 for a loss
 * --
 *
 */

#include <cmath>
#include "rollout.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "score.hpp"

float rollout(const Position &pos, const int max_depth)
{
    Position new_pos = pos;
    Move moves[256];

    int d = 0;
    while(d < max_depth)
    {
        int num_moves = movegen(new_pos, moves);

        if(num_moves == 0)
        {
            float r = score(new_pos);

                 if(r > 0.0) {r = 1.0;}
            else if(r < 0.0) {r = 0.0;}

            if(pos.turn == new_pos.turn)
            {
                return r;
            }
            else
            {
                return 1.0 - r;
            }
        }

        int n = rand() % num_moves;
        makemove(new_pos, moves[n]);
        d++;
    }

    return 0.5;
}

float rollout_heavy(const Position &pos, const int max_depth)
{
    Position new_pos = pos;
    Move moves[256];

    int d = 0;
    while(d < max_depth)
    {
        int num_moves = movegen(new_pos, moves);

        float r = score(new_pos);

        if(num_moves == 0 || fabs(r) >= 5.0)
        {
                 if(r > 0.0) {r = 1.0;}
            else if(r < 0.0) {r = 0.0;}

            if(pos.turn == new_pos.turn)
            {
                return r;
            }
            else
            {
                return 1.0 - r;
            }
        }

        int n = rand() % num_moves;
        makemove(new_pos, moves[n]);
        d++;
    }

    return 0.5;
}
