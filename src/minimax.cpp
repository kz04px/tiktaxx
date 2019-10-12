#include <cassert>
#include <cstring>
#include <ctime>
#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "eval.hpp"
#include "score.hpp"
#include "search.hpp"

int minimax(const libataxx::Position &pos,
            search_info &info,
            search_stack *ss,
            PV &pv,
            const int depth) {
    assert(ss != NULL);

    if (depth == 0 || info.depth >= MAX_DEPTH) {
        info.leaf_nodes++;
        return eval(pos);
    }

    if (info.nodes != 0) {
        // Stop searching if we've ran out of time
        if (clock() >= info.end) {
            return 0;
        }

        // Send an update on what we're doing
        if (info.nodes % 500000 == 0) {
            double time_spent = (double)(clock() - info.start) / CLOCKS_PER_SEC;
            std::cout << "info"
                      << " nps " << (uint64_t)(info.nodes / time_spent)
                      << std::endl;
        }
    }

    int best_score = -INF;

    PV new_pv;
    libataxx::Move moves[256];
    int num_moves = pos.legal_moves(moves);

    for (int n = 0; n < num_moves; ++n) {
        libataxx::Position new_pos = pos;

        new_pos.makemove(moves[n]);

        info.nodes++;

        int score = -minimax(new_pos, info, ss + 1, new_pv, depth - 1);

        if (score > best_score) {
            best_score = score;
            pv.moves[0] = moves[n];
            memcpy(pv.moves + 1,
                   new_pv.moves,
                   new_pv.num_moves * sizeof(libataxx::Move));
            pv.num_moves = new_pv.num_moves + 1;
        }
    }

    if (num_moves == 0) {
        const int val = score(pos);

        if (val > 0) {
            return INF - ss->ply;
        } else if (val < 0) {
            return -INF + ss->ply;
        } else {
            return 0;
        }
    }

    return best_score;
}
