/*
 * Monte Tree Position Search -- Pure
 * -- Each legal move from the root position has the same number of simulations
 * -- The move that lead to the highest number of wins is chosen
 * -- 
 *
 *
 */

#include <iostream>
#include <climits>
#include <cassert>

#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "rollout.hpp"

int find_best_move(const int num_moves, const int *wins, const int *draws, const int *losses)
{
    assert(num_moves >= 0);
    assert(wins != NULL);
    assert(draws != NULL);
    assert(losses != NULL);

    int best_index = 0;
    double best_score = -INF;

    for(int n = 0; n < num_moves; ++n)
    {
        if(wins[n] + draws[n] + losses[n] == 0) {continue;}

        double score = (double)wins[n]/(wins[n] + draws[n] + losses[n]);

        if(score >= best_score)
        {
            best_score = score;
            best_index = n;
        }
    }

    return best_index;
}

void mcts_pure(const Position &pos, int num_simulations, int movetime)
{
    int win[256] = {0};
    int draw[256] = {0};
    int loss[256] = {0};

    Move moves[256];
    int num_moves = movegen(pos, moves);

    if(num_moves == 0)
    {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    clock_t start = clock();
    clock_t end_target = clock();

    if(num_simulations == 0)
    {
        num_simulations = INT_MAX;
        end_target = start + ((double)movetime/1000.0)*CLOCKS_PER_SEC;
    }
    else if(movetime == 0)
    {
        end_target = INT_MAX;
    }

    int n = 0;
    int sims = 0;
    while(sims < num_simulations && clock() < end_target)
    {
        // Set position
        Position new_pos = pos;
        makemove(new_pos, moves[n]);

        // Do rollouts
        int result = -rollout(new_pos, 400);
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
        if(n >= num_moves)
        {
            n = 0;
        }

        // Update
        if(sims % 10000 == 0)
        {
            double time = (double)(clock() - start)/CLOCKS_PER_SEC;
            int best_index = find_best_move(num_moves, win, draw, loss);

            std::cout << "info"
                      << " sims " << sims
                      << " score winchance " << (double)win[best_index]/(win[best_index] + draw[best_index] + loss[best_index])
                      << " sps " << (uint64_t)((double)sims/time)
                      << " time " << 1000.0*time
                      << " pv " << move_string(moves[best_index])
                      << std::endl;
        }
    }

    // Find best move
    int best_index = find_best_move(num_moves, win, draw, loss);

    std::cout << "bestmove " << move_string(moves[best_index]) << std::endl;
}
