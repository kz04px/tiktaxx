#include "next-move.hpp"
#include <cassert>
#include <climits>

bool next_move(libataxx::Move *moves,
               const int num_moves,
               libataxx::Move &move,
               int *scores,
               const int cur_move) {
    assert(moves != NULL);
    assert(scores != NULL);
    assert(cur_move <= num_moves);

    if (cur_move == num_moves) {
        return false;
    }

    if (cur_move < 12) {
        int best_index;
        int best_score = INT_MIN;

        for (int n = cur_move; n < num_moves; ++n) {
            if (scores[n] > best_score) {
                best_score = scores[n];
                best_index = n;
            }
        }

        move = moves[best_index];
        moves[best_index] = moves[cur_move];
        scores[best_index] = scores[cur_move];
    } else  {
        move = moves[cur_move];
    }

    return true;
}
