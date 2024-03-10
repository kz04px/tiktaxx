#include <cassert>
#include <climits>
#include <iostream>
#include <libataxx/move.hpp>
#include "options.hpp"
#include "search.hpp"

void alphabeta(Hashtable *tt,
               Options *options,
               const libataxx::Position &pos,
               bool *stop,
               int depth,
               int movetime,
               int btime,
               int wtime,
               int binc,
               int winc) {
    assert(tt != NULL);
    assert(stop != NULL);

    uint64_t node_total = 0ULL;

    PV last_pv;

    search_stack ss[MAX_DEPTH];
    for (int n = 0; n < MAX_DEPTH; ++n) {
        ss[n].ply = n;
#ifdef KILLER_MOVES
        ss[n].killer = NO_MOVE;
#endif
#ifdef NULLMOVE
        ss[n].nullmove = true;
#endif
    }

    search_info info;
    info.start = clock();
    info.end = clock();
    info.stop = stop;
    info.tt = tt;
    info.options = options;

    if (movetime > 0) {
        depth = MAX_DEPTH;
        info.end = info.start + ((double)movetime / 1000.0) * CLOCKS_PER_SEC;
    } else if (btime > 0 || wtime > 0 || binc > 0 || winc > 0) {
        depth = MAX_DEPTH;
        double search_time = pos.get_turn() == libataxx::Side::Black
                                 ? (btime / 30.0)
                                 : (wtime / 30.0);
        info.end = info.start + (search_time / 1000.0) * CLOCKS_PER_SEC;
    } else if (depth > 0) {
        info.end = INT_MAX;
    }

    int last_score = 0;
    for (int d = 1; d <= depth; ++d) {
        PV pv;

#ifdef ASPIRATION_WINDOWS
        int score = 0;
        if (depth < 3) {
            score = alphabeta_search(pos, info, ss, pv, -INF, INF, d);
        } else {
            for (auto r : {50, 200, INF}) {
                int lower = last_score - r;
                int upper = last_score + r;
                score = alphabeta_search(pos, info, ss, pv, lower, upper, d);

                if (lower < score && score < upper) {
                    break;
                }
            }
        }
#else
        int score = alphabeta_search(pos, info, ss, pv, -INF, INF, d);
#endif
        last_score = score;

        clock_t end = clock();
        double time_spent = (double)(end - info.start) / CLOCKS_PER_SEC;

        // Throw away the result if we ran out of time or were asked to stop
        if (d > 1 && (*info.stop == true || end >= info.end)) {
            break;
        }

        assert(pv.num_moves > 0);
        assert(legal_pv(pos, pv) == true);

        node_total += info.nodes;

        std::cout << "info";
        std::cout << " depth " << d;
        std::cout << " nodes " << info.nodes;
        std::cout << " score cp " << score;
        std::cout << " time " << (uint64_t)(1000.0 * time_spent);

        if (pv.num_moves > 0) {
            std::cout << " pv " + get_pv_string(pv);
        }

        std::cout << std::endl;

        last_pv = pv;
    }

#ifndef NDEBUG
    std::cout << std::endl;

    uint64_t total = 0ULL;
    for (int i = 0; i < 256; ++i) {
        total += info.cutoffs[i];
    }
    for (int i = 0; i < 8; ++i) {
        std::cout << "info "
                  << "move " << i + 1 << " "
                  << "cutoffs " << info.cutoffs[i] << " ";
        if (total > 0ULL) {
            std::cout << "(" << 100.0 * info.cutoffs[i] / total << "%)";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < 9; ++i) {
        std::cout << "info "
                  << "captured " << i << " "
                  << "cutoffs " << info.capture_cutoffs[i] << " ";
        if (total > 0ULL) {
            std::cout << "(" << 100.0 * info.capture_cutoffs[i] / total << "%)";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Single cutoffs: " << info.single_cutoffs << std::endl;
    std::cout << "Double cutoffs: " << info.double_cutoffs << std::endl;
    std::cout << std::endl;

    std::cout << "Hash table hits: " << info.hash_hits << std::endl;
    std::cout << "Collisions: " << info.hash_collisions << std::endl;
    if (info.hash_hits > 0ULL) {
        std::cout << "Percent: "
                  << 100.0 * (double)info.hash_collisions / info.hash_hits
                  << "%" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "tt->num_entries: " << tt->num_entries << std::endl;
    std::cout << "tt->max_entries: " << tt->max_entries << std::endl;
    std::cout << std::endl;
#endif

    if (last_pv.num_moves == 0) {
        // If we somehow don't have a move from the PV, try play a random one
        libataxx::Move moves[256];
        int num_moves = pos.legal_moves(moves);

        if (num_moves == 0) {
            std::cout << "bestmove 0000" << std::endl;
        } else {
            int n = rand() % num_moves;
            assert(pos.is_legal_move(moves[n]) == true);
            std::cout << "bestmove " << moves[n] << std::endl;
        }
    } else {
        std::cout << "bestmove " << last_pv.moves[0] << std::endl;
    }
}
