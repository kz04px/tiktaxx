#include "next-move.hpp"
#include <cassert>
#include <climits>

bool next_move(const libataxx::Move *moves,
               const int num_moves,
               libataxx::Move &move,
               int *scores) {
    assert(moves != NULL);
    assert(scores != NULL);

    int best_index = 0;
    int best_score = INT_MIN;

    for (int n = 0; n < num_moves; ++n) {
        if (scores[n] > best_score) {
            best_score = scores[n];
            best_index = n;
        }
    }

    scores[best_index] = INT_MIN;
    move = moves[best_index];

    return (best_score != INT_MIN);
}
