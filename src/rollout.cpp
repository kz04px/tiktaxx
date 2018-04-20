/*
 * Random pos simulation
 * -- Play a pos of Ataxx choosing moves at random
 * -- return  1 for a win
 * -- return  0 for a draw
 * -- return -1 for a loss
 * --
 *
 */

#include "rollout.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "score.hpp"

int rollout(const Position &pos, const int max_depth)
{
    Position new_pos = pos;
    Move moves[256];

    int d = 0;
    while(d < max_depth)
    {
        int num_moves = movegen(new_pos, moves);

        if(num_moves == 0)
        {
            int r = score(new_pos);

                 if(r > 0) {r = 1;}
            else if(r < 0) {r = -1;}

            if(pos.turn == new_pos.turn)
            {
                return r;
            }
            else
            {
                return -r;
            }
        }

        int n = rand() % num_moves;
        makemove(new_pos, moves[n]);
        d++;
    }

    return 0;
}

int rollout_heavy(const Position &pos, const int max_depth)
{
    Position new_pos = pos;
    Move moves[256];

    int d = 0;
    while(d < max_depth)
    {
        int num_moves = movegen(new_pos, moves);

        int r = score(new_pos);

        if(num_moves == 0 || abs(r) > 5)
        {
                 if(r > 0) {r = 1;}
            else if(r < 0) {r = -1;}

            if(pos.turn == new_pos.turn)
            {
                return r;
            }
            else
            {
                return -r;
            }
        }

        int n = rand() % num_moves;
        makemove(new_pos, moves[n]);
        d++;
    }

    return 0;
}
