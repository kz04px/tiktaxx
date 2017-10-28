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

int rollout(const Position& pos, const int maxDepth)
{
    Position newPos = pos;
    Move moves[256];

    int d = 0;
    while(d < maxDepth)
    {
        int numMoves = movegen(newPos, moves);

        if(numMoves == 0)
        {
            int r = score(newPos);

                 if(r > 0) {r = 1;}
            else if(r < 0) {r = -1;}

            if(pos.turn == newPos.turn)
            {
                return r;
            }
            else
            {
                return -r;
            }
        }

        int n = rand() % numMoves;
        makemove(newPos, moves[n]);
        d++;
    }

    return 0;
}
