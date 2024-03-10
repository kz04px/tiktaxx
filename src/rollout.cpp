/*
 * Random game simulation
 * -- Play a game of Ataxx choosing moves at random
 * -- return 1.0 for a win
 * -- return 0.5 for a draw
 * -- return 0.0 for a loss
 * --
 *
 */

#include "rollout.hpp"
#include <cmath>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "score.hpp"

float rollout(const libataxx::Position &pos, const int max_depth) {
    libataxx::Position new_pos = pos;
    libataxx::Move moves[256];

    int d = 0;
    while (d < max_depth) {
        int num_moves = new_pos.legal_moves(moves);

        if (num_moves == 0) {
            float r = score(new_pos);

            if (r > 0.0) {
                r = 1.0;
            } else if (r < 0.0) {
                r = 0.0;
            }

            if (pos.get_turn() == new_pos.get_turn()) {
                return r;
            } else {
                return 1.0 - r;
            }
        }

        int n = rand() % num_moves;
        new_pos.makemove(moves[n]);
        d++;
    }

    return 0.5;
}

float rollout_heavy(const libataxx::Position &pos, const int max_depth) {
    libataxx::Position new_pos = pos;
    libataxx::Move moves[256];

    int d = 0;
    while (d < max_depth) {
        int num_moves = new_pos.legal_moves(moves);

        float r = score(new_pos);

        if (num_moves == 0 || fabs(r) >= 5.0) {
            if (r > 0.0) {
                r = 1.0;
            } else if (r < 0.0) {
                r = 0.0;
            }

            if (pos.get_turn() == new_pos.get_turn()) {
                return r;
            } else {
                return 1.0 - r;
            }
        }

        int n = rand() % num_moves;
        new_pos.makemove(moves[n]);
        d++;
    }

    return 0.5;
}
