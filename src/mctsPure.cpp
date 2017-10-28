/*
 * Monte Tree Position Search -- Pure
 * -- Each legal move from the root position has the same number of simulations
 * -- The move that lead to the highest number of wins is chosen
 * -- 
 *
 *
 */

#include <iostream>

#include "ataxx.hpp"
#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "rollout.hpp"

void mctsPure(const Position& pos)
{
    int win[256] = {0};
    int draw[256] = {0};
    int loss[256] = {0};

    Move moves[256];
    int numMoves = movegen(pos, moves);

    int simulationsPer = NUM_SIMULATIONS/numMoves;

    clock_t start = clock();

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        for(int r = 0; r < simulationsPer; ++r)
        {
            int result = -rollout(newPos, 400);

            if(result == 1)
            {
                win[n]++;
            }
            else if(result == -1)
            {
                loss[n]++;
            }
            else
            {
                draw[n]++;
            }
        }
    }

    clock_t end = clock();

    int best = 0;
    for(int n = 0; n < numMoves; ++n)
    {
        // Track best move
        if(win[n] >= win[best])
        {
            best = n;
        }

        std::cout << n+1 << ":  ";
        if(n+1 < 10) {std::cout << " ";}

        std::cout << moveString(moves[n]);
        if(moves[n].to == moves[n].from)
        {
            std::cout << "  ";
        }

        std::cout << "  " << 100.0*(double)win[n]/(win[n] + loss[n] + draw[n]) << "%"
                  << "  (" << win[n]
                  << "-" << loss[n]
                  << "-" << draw[n]
                  << ")"
                  << std::endl;
    }

    std::cout << "bestmove " << moveString(moves[best]) << std::endl;
    std::cout << "percentage " << 100.0*(double)win[best]/(win[best] + loss[best] + draw[best]) << "%" << std::endl;
    std::cout << "time " << 1000.0*(double)(end-start)/CLOCKS_PER_SEC << std::endl;
    std::cout << "simulations " << simulationsPer * numMoves << std::endl;
}
