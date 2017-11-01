/*
 * Monte Tree Position Search -- Pure
 * -- Each legal move from the root position has the same number of simulations
 * -- The move that lead to the highest number of wins is chosen
 * -- 
 *
 *
 */

#include <iostream>

#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "rollout.hpp"

int findBestmove(const int numMoves, const int *wins, const int *draws, const int *losses)
{
    int bestIndex = 0;
    double bestScore = -INF;

    for(int n = 0; n < numMoves; ++n)
    {
        double score = (double)wins[n]/(wins[n] + draws[n] + losses[n]);

        if(score >= bestScore)
        {
            bestScore = score;
            bestIndex = n;
        }
    }

    return bestIndex;
}

void mctsPure(const Position& pos, int numSimulations, int movetime)
{
    int win[256] = {0};
    int draw[256] = {0};
    int loss[256] = {0};

    Move moves[256];
    int numMoves = movegen(pos, moves);

    if(numMoves == 0)
    {
        std::cout << "bestmove pass" << std::endl;
        return;
    }

    clock_t start = clock();
    clock_t endTarget = clock();

    if(numSimulations == 0)
    {
        numSimulations = INT_MAX;
        endTarget = start + ((double)movetime/1000.0)*CLOCKS_PER_SEC;
    }
    else if(movetime == 0)
    {
        endTarget = INT_MAX;
    }

    int n = 0;
    int sims = 0;
    while(sims < numSimulations && clock() < endTarget)
    {
        // Set position
        Position newPos = pos;
        makemove(newPos, moves[n]);

        // Do rollouts
        int result = -rollout(newPos, 400);
        sims++;

        // Score
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

        // Next move
        n++;
        if(n >= numMoves)
        {
            n = 0;
        }

        // Update
        if(sims % 10000 == 0)
        {
            double time = (double)(clock() - start)/CLOCKS_PER_SEC;
            int bestIndex = findBestmove(numMoves, win, draw, loss);

            std::cout << "info"
                      << " sims " << sims
                      << " score " << 100.0*(double)win[bestIndex]/(win[bestIndex] + draw[bestIndex] + loss[bestIndex]) << "%"
                      << " sps " << (uint64_t)((double)sims/time)
                      << " time " << 1000.0*time
                      << " pv " << moveString(moves[bestIndex])
                      << std::endl;
        }
    }

    // Find best move
    int bestIndex = findBestmove(numMoves, win, draw, loss);

    std::cout << "bestmove " << moveString(moves[bestIndex]) << std::endl;
}
