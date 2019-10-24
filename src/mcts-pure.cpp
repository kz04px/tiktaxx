/*
 * Monte Tree libataxx::Position Search -- Pure
 * -- Each legal move from the root position has the same number of simulations
 * -- The move that lead to the highest number of wins is chosen
 * --
 *
 *
 */

#include <cassert>
#include <climits>
#include <iostream>
#include <libataxx/move.hpp>
#include "rollout.hpp"
#include "search.hpp"

void mcts_pure(const libataxx::Position &pos, int max_nodes, int movetime) {
    libataxx::Move moves[256];
    int num_moves = pos.legal_moves(moves);

    if (num_moves == 0) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    } else if (num_moves == 1) {
        std::cout << "bestmove " << moves[0] << std::endl;
        return;
    }

    float scores[256] = {0.0};
    int games[256] = {0};
    clock_t start = clock();
    clock_t end_target = clock();

    if (max_nodes == 0) {
        max_nodes = INT_MAX;
        end_target = start + ((double)movetime / 1000.0) * CLOCKS_PER_SEC;
    } else if (movetime == 0) {
        end_target = INT_MAX;
    }

    int index = 0;
    int nodes = 0;
    while (nodes < max_nodes && clock() < end_target) {
        // Set position
        libataxx::Position new_pos = pos;
        new_pos.makemove(moves[index]);

        // Do rollouts
        scores[index] += 1.0 - rollout(new_pos, 400);
        games[index]++;
        nodes++;

        // Next move
        index++;
        index = index % num_moves;

        // Update
        if (nodes % 10000 == 0) {
            double time_taken = (double)(clock() - start) / CLOCKS_PER_SEC;

            // Find best move
            int best_index = 0;
            double best_score = -INF;
            for (int n = 0; n < num_moves; ++n) {
                if (scores[n] > best_score) {
                    best_score = scores[n];
                    best_index = n;
                }
            }

            std::cout << "info"
                      << " nodes " << nodes << " winrate "
                      << (double)scores[best_index] / games[best_index] << "%";
            if (time_taken > 0.0) {
                std::cout << " nps " << (uint64_t)((double)nodes / time_taken);
            }
            std::cout << " time " << 1000.0 * time_taken << " pv "
                      << moves[best_index] << std::endl;
        }
    }

    // Find best move
    int best_index = 0;
    double best_score = -INF;
    for (int n = 0; n < num_moves; ++n) {
        if (scores[n] > best_score) {
            best_score = scores[n];
            best_index = n;
        }
    }

    std::cout << "bestmove " << moves[best_index] << std::endl;
}
